#include "StdAfx.hpp"
#include <ctime>
#include <memory>

// CEGUI styles
#include "CEGUIStyleManager.hpp"

// Engine core
#include "Material.hpp"
#include "SceneManager.hpp"
#include "SceneNode.hpp"
#include "Texture.hpp"
#include "FrameBuffer.hpp"

// renderables
#include "Camera.hpp"
#include "Cube.hpp"
#include "Light.hpp"
#include "Plane.hpp"
#include "SkyBox.hpp"
#include "Sphere.hpp"
#include "Teapot.hpp"
#include "Tree.hpp"

// Cg support
#include "CgContext.hpp"
#include "CgProfile.hpp"
#include "CgProgram.hpp"

using namespace std;

static const int kWindowWidth = 800;
static const int kWindowHeight = 600;

// We can't initialize these here since OpenGL needs to be initialized first
static CgContext* cgContext;
static CgProfile* cgVertexProfile;
static CgProfile* cgFragmentProfile;

static FrameBuffer* testBuffer;

static SceneManager sm;
static bool animate = false;
static shared_ptr<Camera> freeCam;
static shared_ptr<SceneNode> freeCamNode;
static shared_ptr<Camera> topCam;
static shared_ptr<SceneNode> topCamNode;
static shared_ptr<Plane> rttPlane;

static struct {
	CEGUI::Checkbox* chkEnableGUI;
	CEGUI::Window* lblCam;
	CEGUI::RadioButton* radFree;
	CEGUI::RadioButton* radTop;
} controls;

//! Instead of being a simple enum, this struct is used to hold
//! direction-related key presses since multiple keys can be down.
struct KeyDirections {
	bool up;
	bool down;
	bool forward;
	bool backward;
	bool left;
	bool right;

	KeyDirections() : up(false), down(false), left(false), right(false) { }
};

//! Contains instructions for the movement of the camera. Filled in by keyboard
//! functions and used before each frame
static struct {
	KeyDirections movement;
	KeyDirections rotation;
} cameraMovement;

//! Current yaw of the camera, in radians
static float cameraYaw = 0.0f;
//! Current pitch of the camera, in radians
static float cameraPitch = Math::kPi / 6.0f;

//! Delta camera movment per frame, in radians
static const float deltaMovement = 0.1f;
//! Delta camera angule per frame, in radians
static const float deltaTheta = Math::kPi * 2.0f  / 240.0f;

// Event handler for the show GUI checkbox
bool onShowGUIClicked(const CEGUI::EventArgs& e)
{
	bool selected = controls.chkEnableGUI->isSelected();
	controls.lblCam->setVisible(selected);
	controls.radFree->setVisible(selected);
	controls.radTop->setVisible(selected);
	return true;
}

void init()
{
	// If any of this fails, we don't have sufficient hardware
	try {

		// Start GLEW and check for needed extensions
		if (glewInit() != GLEW_OK) {
			throw Exceptions::Exception("GLEW failed to initialize",
			                            __FUNCTION__);
		}
		if (!GLEW_EXT_framebuffer_object) {
			throw Exceptions::Exception("Frame buffer objects not supported",
			                            __FUNCTION__);
		}

		// Set up a frame buffer to do a test render to texture
		testBuffer = new FrameBuffer(512, 512);

		// Start up Cg
		cgContext = new CgContext;
		cgVertexProfile = new CgProfile(*cgContext, CG_GL_VERTEX);
		cgFragmentProfile = new CgProfile(*cgContext, CG_GL_FRAGMENT);

		glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
		// Avoid stupid problems with OGL and RGB formats
		// (since OGL tries to read textures to the nearest 4-byte boundary)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Create and place our cameras
		freeCam = make_shared<Camera>();
		freeCam->setPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.3f, 50.0f);
		sm.setActiveCamera(freeCam);
		freeCamNode = make_shared<SceneNode>(nullptr, Vector3(0.0f, 6.0f, -10.0f));
		freeCamNode->addRenderable(freeCam);
		sm.getSceneNodes().push_back(freeCamNode);

		topCam = make_shared<Camera>();
		topCam->setPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.3f, 30.0f);
		topCam->setTarget(Vector3());
		topCam->setUpDirection(Vector3(0.0f, 0.0f, 1.0f));
		topCamNode = make_shared<SceneNode>(nullptr, Vector3(0.0, 20.0f, 0.0f));
		topCamNode->addRenderable(topCam);
		sm.getSceneNodes().push_back(topCamNode);

		// Create and place our light.
		auto light = make_shared<Light>();
		auto lightNode = make_shared<SceneNode>(nullptr, Vector3(10.0f, 10.0f, -10.0f));
		lightNode->addRenderable(light);
		// Give the light a yellow sphere (sun?)
		auto lightMat =  make_shared<Material>();
		lightMat->lighting = false;
		lightMat->color[0] = 1.0f;
		lightMat->color[1] = 1.0f;
		lightMat->color[2] = 0.0f;
		auto sun = make_shared<Sphere>(lightMat);
		lightNode->addRenderable(sun);
		sm.getSceneNodes().push_back(lightNode);

		// Set up our sky box
		auto skyboxNode = make_shared<SceneNode>();
		skyboxNode->addRenderable(make_shared<SkyBox>());
		sm.getSceneNodes().push_back(shared_ptr<SceneNode>(skyboxNode));

		// Set up our "ground"
		auto groundNode = make_shared<SceneNode>();
		groundNode->getTransform().scale(Vector3(15.0f));
		auto groundMat = make_shared<Material>();
		groundMat->lighting = false;
		// Load the ground's texture
		groundMat->texture = make_shared<Texture>("./resources/textures/Awesome.png");
		// Load the ground's vertex shader
		groundMat->vertexShader = make_shared<CgProgram>(*cgContext, false,
		                          "./resources/shaders/TestVert.cg",
		                          *cgVertexProfile, "main");
		// Load the ground's pixel shader
		groundMat->fragmentShader = make_shared<CgProgram>(*cgContext, false,
		                            "./resources/shaders/TestFrag.cg",
		                            *cgFragmentProfile, "main");
		// Use a lambda function to set the ground's material callback
		groundMat->callback = [](const shared_ptr<Material>& mat) {
			// Retrieve then set modelViewProj inside the vertex shader
			auto mvp = mat->vertexShader->getNamedParameter("modelViewProj");
			mvp.setStateMatrix(CG_GL_MODELVIEW_PROJECTION_MATRIX);

			// Retrieve then set t for both the vertex and fragment shader
			float t = (float)(clock() % CLOCKS_PER_SEC) / (float)CLOCKS_PER_SEC
			          * Math::kPi * 2.0f;
			auto tVert = mat->vertexShader->getNamedParameter("t");
			tVert.set1f(t);
			auto tFrag = mat->fragmentShader->getNamedParameter("t");
			tFrag.set1f(t);
		};
		auto ground = make_shared<Plane>(groundMat);
		groundNode->addRenderable(ground);
		sm.getSceneNodes().push_back(groundNode);

		Transform pnt;
		pnt.rotateDegrees(Vector3(90.0f, 0.0f, 0.0f));
		pnt.scale(Vector3(6.0f));
		pnt.setTranslation(Vector3(0.0f, 10.0f, 5.0f));
		auto planeNode = make_shared<SceneNode>(nullptr, pnt);
		auto rttMat = make_shared<Material>();
		auto rtt = make_shared<Texture>(nullptr, 3, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, false);
		rttMat->texture = rtt;
		testBuffer->attachTexture(rtt);
		rttPlane = make_shared<Plane>(rttMat);
		planeNode->addRenderable(rttPlane);
		sm.getSceneNodes().push_back(planeNode);
	}
	catch (const Exceptions::Exception& ex) {
		MessageBox(GetActiveWindow(),
		           "This computer does not support graphics features necessary"
		           " for this demo.", "Insufficient graphics hardware",
		           MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Enable our first (and only) light
	//! \todo Support more lights later
	glEnable(GL_LIGHT0);

	// Set up shading model
	glShadeModel(GL_SMOOTH);

	// Enable transparent materials
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	try {
		// Set up CEGUI
		using namespace CEGUI;
		OpenGLRenderer::bootstrapSystem();

		// initialise the required dirs for the DefaultResourceProvider
		CEGUI::DefaultResourceProvider* rp =
		    static_cast<CEGUI::DefaultResourceProvider*>
		    (CEGUI::System::getSingleton().getResourceProvider());

		// Tell CEGUI where to find resources
		rp->setResourceGroupDirectory("schemes",
		                              "./resources/CEGUI/schemes/");
		rp->setResourceGroupDirectory("imagesets",
		                              "./resources/CEGUI/imagesets/");
		rp->setResourceGroupDirectory("fonts",
		                              "./resources/CEGUI/fonts/");
		rp->setResourceGroupDirectory("looknfeels",
		                              "./resources/CEGUI/looknfeel/");
		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");

		Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		Scheme::setDefaultResourceGroup("schemes");
		WidgetLookManager::setDefaultResourceGroup("looknfeels");
		SchemeManager::getSingleton().create(CEGUIStyleManager::getScheme());

		// Set up a root window
		WindowManager& wmgr = WindowManager::getSingleton();
		Window* root = wmgr.createWindow("DefaultWindow", "SelectZonesRoot");
		System::getSingleton().setGUISheet(root);

		std::string checkboxName = CEGUIStyleManager::getCheckboxName();
		std::string labelName = CEGUIStyleManager::getStaticTextName();
		std::string radioName = CEGUIStyleManager::getRadioButtonName();

		// Add our controls
		controls.chkEnableGUI = static_cast<Checkbox*>(wmgr.createWindow(
		                            checkboxName,
		                            "SelectZonesRoot/ShowGUI"));
		controls.chkEnableGUI->setPosition(
		    UVector2(UDim(0.8f, 0.0f), UDim(0.0f, 20.0f)));
		controls.chkEnableGUI->setSize(
		    UVector2(UDim(0.3f, 0), UDim(0.0f, 30.0f)));
		controls.chkEnableGUI->setText("Show Controls");
		controls.chkEnableGUI->setSelected(true);
		controls.chkEnableGUI->subscribeEvent(
		    Checkbox::EventCheckStateChanged,
		    Event::Subscriber(onShowGUIClicked));
		root->addChildWindow(controls.chkEnableGUI);

		controls.lblCam = wmgr.createWindow(
		                      labelName, "SelectZoneRoot/CameraLabel");
		controls.lblCam->setPosition(
		    UVector2(UDim(0.8f, 0.0f), UDim(0.0f, 190.0f)));
		controls.lblCam->setSize(
		    UVector2(UDim(0.18f, 0), UDim(0.0f, 30.0f)));
		controls.lblCam->setText("Camera Location:");
		root->addChildWindow(controls.lblCam);

		controls.radFree = static_cast<RadioButton*>(wmgr.createWindow(
		                       radioName,
		                       "SelectZoneRoot/Free"));
		controls.radFree->setPosition(
		    UVector2(UDim(0.8f, 0.0f), UDim(0.0f, 220.0f)));
		controls.radFree->setSize(
		    UVector2(UDim(0.18f, 0.0f), UDim(0.0f, 20.0f)));
		controls.radFree->setText("Free");
		controls.radFree->setGroupID(0);
		controls.radFree->setSelected(true);
		root->addChildWindow(controls.radFree);

		controls.radTop = static_cast<RadioButton*>(wmgr.createWindow(
		                      radioName,
		                      "SelectZoneRoot/Top"));
		controls.radTop->setPosition(
		    UVector2(UDim(0.8f, 0.0f), UDim(0.0f, 260.0f)));
		controls.radTop->setSize(
		    UVector2(UDim(0.18f, 0.0f), UDim(0.0f, 20.0f)));
		controls.radTop->setText("Top");
		controls.radTop->setGroupID(0);
		root->addChildWindow(controls.radTop);
	}
	catch (const CEGUI::Exception& ex) {
		fprintf(stderr,
		        "Fatal CEGUI startup error:\n%s\n", ex.getMessage().c_str());
		exit(1);
	}
}

void onDisplay()
{
	if (controls.radFree->isSelected()) {
		sm.setActiveCamera(freeCam);
		// Rotate the camera
		if (cameraMovement.rotation.up)
			cameraPitch -= deltaTheta;
		if (cameraMovement.rotation.down)
			cameraPitch += deltaTheta;
		if (cameraMovement.rotation.left)
			cameraYaw -= deltaTheta;
		if (cameraMovement.rotation.right)
			cameraYaw += deltaTheta;

		Transform camTrans;
		camTrans.rotateRadians(Vector3(cameraPitch, cameraYaw, 0.0f));
		Vector3 target(0.0f, 0.0f, 1.0f);
		Vector3 up(0.0f, 1.0f, 0.0f);
		camTrans.transformPoint(target);
		camTrans.transformPoint(up);

		// Move the camera. Up and down (Y movement) stay absolute, but
		// forward and back are based on your current camera direction.

		// Only take the horizontal component of the target
		Vector3 forward(target.X, 0.0f, target.Z);
		forward.normalize();
		// left is equal to the cross product of the target and up vectors
		Vector3 left = Vector3::crossProduct(target, up);
		Vector3 camPosChange;
		if (cameraMovement.movement.up)
			camPosChange.Y += deltaMovement;
		if (cameraMovement.movement.down)
			camPosChange.Y -= deltaMovement;
		if (cameraMovement.movement.left)
			camPosChange += left * deltaMovement;
		if (cameraMovement.movement.right)
			camPosChange -= left * deltaMovement;
		if (cameraMovement.movement.forward)
			camPosChange += forward * deltaMovement;
		if (cameraMovement.movement.backward)
			camPosChange -= forward * deltaMovement;

		freeCamNode->getTransform().setTranslation(
		    freeCamNode->getTransform().getTranslation() + camPosChange);

		// Translate the target so that it's relative to the camera
		target += freeCamNode->getTransform().getTranslation();

		freeCam->setTarget(target);
		freeCam->setUpDirection(up);
	}
	else if (controls.radTop->isSelected()) {
		sm.setActiveCamera(topCam);
	}

	// Enable depth testing and draw our scene
	try {
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rttPlane->setVisible(false);
		testBuffer->setupRender();
		sm.renderScene();
		testBuffer->cleanupRender();
		rttPlane->setVisible(true);
		sm.renderScene();
		// Disable lighting and depth tests for rendering the GUI
		setActiveMaterial(getDefaultMaterial());
		glDisable(GL_DEPTH_TEST);
		CEGUI::System::getSingleton().renderGUI();
		// Push our newly rendered frame to the screen
		glFlush();
		glutSwapBuffers();
	}
	catch (const Exceptions::Exception& ex) {
		string msg = "Exception in render loop: ";
		msg += ex.message;
		msg += "\nIn function: ";
		msg += ex.callingFunction;
		MessageBox(GetActiveWindow(), msg.c_str(), "Render Exception",
		           MB_OK | MB_ICONERROR);
		exit(1);
	}
}

void onTimer(int value)
{
	// Tell CEGUI how much time has passed
	CEGUI::System::getSingleton().injectTimePulse((float)0.016f);
	glutPostRedisplay(); // Render a frame
	// Render another frame in a 60th of a second
	glutTimerFunc(16, onTimer, 1);
}

void onMouse(int button, int state, int x, int y)
{
	auto& sys = CEGUI::System::getSingleton();
	if (state == 0) {   // State 0 = Button Pressed
		switch (button) {
		case 0:     // glut's left mouse button
			sys.injectMouseButtonDown(CEGUI::LeftButton);
			break;
		case 1:     // glut's middle mouse button
			sys.injectMouseButtonDown(CEGUI::MiddleButton);
			break;
		case 2:     // glut's right mouse button
			sys.injectMouseButtonDown(CEGUI::RightButton);
			break;
		case 3:     // glut's mouse wheel up
			sys.injectMouseWheelChange(2.0);
			break;
		case 4:     // glut's mouse wheen down
			sys.injectMouseWheelChange(-2.0);
			break;
		}
	}
	else if (state == 1) {  // State 1 = Button Released
		switch (button) {
		case 0:     // glut's left mouse button
			sys.injectMouseButtonUp(CEGUI::LeftButton);
			break;
		case 1:     // glut's middle mouse button
			sys.injectMouseButtonUp(CEGUI::MiddleButton);
			break;
		case 2:     // glut's right mouse button
			sys.injectMouseButtonUp(CEGUI::RightButton);
			break;
		}
	}
}

void onMotion(int x, int y)
{
	// Tell CEGUI where the mouse is now
	CEGUI::System::getSingleton().injectMousePosition((float)x, (float)y);
}

void onPassiveMotion(int x, int y)
{
	// Tell CEGUI where the mouse is now
	CEGUI::System::getSingleton().injectMousePosition((float)x, (float)y);
}

void onKeyboardDown(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		cameraMovement.movement.forward = true;
		break;

	case 's':
		cameraMovement.movement.backward = true;
		break;

	case 'a':
		cameraMovement.movement.left = true;
		break;

	case 'd':
		cameraMovement.movement.right = true;
		break;

	case 'e':
		cameraMovement.movement.down = true;
		break;

	case 'q':
		cameraMovement.movement.up = true;
		break;
	}
}

void onKeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		cameraMovement.movement.forward = false;
		break;

	case 's':
		cameraMovement.movement.backward = false;
		break;

	case 'a':
		cameraMovement.movement.left = false;
		break;

	case 'd':
		cameraMovement.movement.right = false;
		break;

	case 'e':
		cameraMovement.movement.down = false;
		break;

	case 'q':
		cameraMovement.movement.up = false;
		break;
	}
}

void onSpecialKeyDown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		cameraMovement.rotation.up = true;
		break;

	case GLUT_KEY_DOWN:
		cameraMovement.rotation.down = true;
		break;

	case GLUT_KEY_LEFT:
		cameraMovement.rotation.left = true;
		break;

	case GLUT_KEY_RIGHT:
		cameraMovement.rotation.right = true;
		break;
	}
}

void onSpecialKeyUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		cameraMovement.rotation.up = false;
		break;

	case GLUT_KEY_DOWN:
		cameraMovement.rotation.down = false;
		break;

	case GLUT_KEY_LEFT:
		cameraMovement.rotation.left = false;
		break;

	case GLUT_KEY_RIGHT:
		cameraMovement.rotation.right = false;
		break;
	}
}

void onReshape (int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	CEGUI::System::getSingleton().notifyDisplaySizeChanged(
	    CEGUI::Size((float)w, (float)h));
}

int main(int argc, char** argv)
{
	try {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(kWindowWidth, kWindowHeight);
		glutInitWindowPosition(100, 100);
		glutCreateWindow(argv[0]);
		init();
		glutReshapeFunc(onReshape);
		glutDisplayFunc(onDisplay);
		// We signal a render inside the timer function. 16 ms is approximately
		// one frame every 60th of a second
		glutTimerFunc(16, onTimer, 1);
		// Respond to mouse input by enabling and disabling animation
		glutMouseFunc(onMouse);
		glutMotionFunc(onMotion);
		glutPassiveMotionFunc(onPassiveMotion);
		glutKeyboardFunc(onKeyboardDown);
		glutKeyboardUpFunc(onKeyboardUp);
		glutSpecialFunc(onSpecialKeyDown);
		glutSpecialUpFunc(onSpecialKeyUp);
		glutMainLoop();
		return 0;
	}
	catch (const Exceptions::Exception& ex) {
		std::string msg = "The following Exception occurred: ";
		msg += ex.message;
		MessageBox(GetActiveWindow(), msg.c_str(),
		           "Unhandled Exception", MB_OK | MB_ICONERROR);
	}
}
