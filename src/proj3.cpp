#include "StdAfx.hpp"
#include <ctime>
#include <memory>

// CEGUI styles
#include "CEGUIStyleManager.hpp"

// Engine core
#include "Material.hpp"
#include "SceneRenderer.hpp"
#include "SceneNode.hpp"
#include "Texture.hpp"
#include "FrameBuffer.hpp"
#include "AnimatorManager.hpp"
#include "Animator.hpp"
#include "ShaderSet.hpp"

// renderables
#include "Camera.hpp"
#include "Cube.hpp"
#include "DirectionalLight.hpp"
#include "SkyBox.hpp"
#include "Sphere.hpp"
#include "Teapot.hpp"
#include "OBJFile.hpp"
#include "Model.hpp"
#include "MAPFile.hpp"
#include "Sun.hpp"

// Animated things
#include "RoadMap.hpp"
#include "CarAnimator.hpp"
#include "CrateAnimator.hpp"

// Articulated Objects
#include "ArticulatedCrane.hpp"
#include "ArticulatedRadio.hpp"

// Cg support
#include "CgSingleton.hpp"
#include "CgProgram.hpp"

using namespace std;

static const int kWindowWidth = 1024;
static const int kWindowHeight = 768;

static SceneRenderer* sr;
static AnimatorManager* am;

static bool animate = false;
static shared_ptr<Camera> freeCam;
static shared_ptr<SceneNode> freeCamNode;
static shared_ptr<Camera> topCam;
static shared_ptr<SceneNode> topCamNode;
static shared_ptr<Camera> landingCam;
static shared_ptr<SceneNode> landingCamNode;
static shared_ptr<Camera> powerCam;
static shared_ptr<SceneNode> powerCamNode;
static struct {
	CEGUI::Checkbox* chkEnableGUI;
	CEGUI::Window* lblCam;
	CEGUI::RadioButton* radFree;
	CEGUI::RadioButton* radTop;
	CEGUI::RadioButton* radLanding;
	CEGUI::RadioButton* radPower;
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
static float cameraPitch = Math::kPi / 4.0f;

//! Delta camera movment per frame, in radians
static const float deltaMovement = 0.3f;
//! Delta camera angule per frame, in radians
static const float deltaTheta = Math::kPi * 2.0f  / 240.0f;

// Event handler for the show GUI checkbox
bool onShowGUIClicked(const CEGUI::EventArgs& e)
{
	bool selected = controls.chkEnableGUI->isSelected();
	controls.lblCam->setVisible(selected);
	controls.radFree->setVisible(selected);
	controls.radTop->setVisible(selected);
	controls.radLanding->setVisible(selected);
	controls.radPower->setVisible(selected);
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
		// Check for FBO support
		if (!GLEW_EXT_framebuffer_object) {
			throw Exceptions::Exception("Frame buffer objects not supported",
			                            __FUNCTION__);
		}
		// CHeck for multitexture support
		if (!GLEW_ARB_multitexture) {
			throw Exceptions::Exception("Multitexture not supported",
			                            __FUNCTION__);
		}

		// Start up Cg
		auto& cgs = CgSingleton::getSingleton();
		auto& cgContext = cgs.getContext();
		auto& cgVertProfile = cgs.getVertexProfile();
		auto& cgFragProfile = cgs.getFragmentProfile();

		// Start up our scene renderer
		sr = new SceneRenderer(kWindowWidth, kWindowHeight);

		// and animator manager
		am = new AnimatorManager();

		glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
		// Avoid stupid problems with OGL and RGB formats
		// (since OGL tries to read textures to the nearest 4-byte boundary)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Create and place our cameras
		freeCam = make_shared<Camera>();
		freeCam->setPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.3f, 200.0f);
		sr->setActiveCamera(freeCam);
		freeCamNode = make_shared<SceneNode>(Vector3(0.0f, 6.0f, -10.0f));
		freeCamNode->addRenderable(freeCam);
		sr->getSceneNodes().push_back(freeCamNode);

		topCam = make_shared<Camera>();
		topCam->setPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.3f, 200.0f);
		topCam->setTarget(Vector3());
		topCam->setUpDirection(Vector3(0.0f, 0.0f, 1.0f));
		topCamNode = make_shared<SceneNode>(Vector3(0.0, 180.0f, 0.0f));
		topCamNode->addRenderable(topCam);
		sr->getSceneNodes().push_back(topCamNode);

		landingCam = make_shared<Camera>();
		landingCam->setPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.3f, 90.0f);
		landingCam->setTarget(Vector3(90.0f, 0.0f, -90.0f));
		landingCamNode = make_shared<SceneNode>(Vector3(30.0f, 20.0f, -30.0f));
		landingCamNode->addRenderable(landingCam);
		sr->getSceneNodes().push_back(landingCamNode);

		powerCam = make_shared<Camera>();
		powerCam->setPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.3f, 200.0f);
		powerCam->setTarget(Vector3());
		powerCamNode = make_shared<SceneNode>(Vector3(0.0f, 6.0f, -50.0f));
		powerCamNode->addRenderable(powerCam);
		sr->getSceneNodes().push_back(powerCamNode);

		// Set up commonly used texture sets
		auto deferredTextureSet = make_shared<ShaderSet>();
		deferredTextureSet->vertexShader =
		    make_shared<CgProgram>(cgContext, false,
		                           "./resources/shaders/DeferredTexture.cg",
		                           cgVertProfile, "VS_Main");
		deferredTextureSet->fragmentShader =
		    make_shared<CgProgram>(cgContext, false,
		                           "./resources/shaders/DeferredTexture.cg",
		                           cgFragProfile, "FS_Main");
		deferredTextureSet->callback = [&](const shared_ptr<Material>& mat) {
			mat->vertexShader->getNamedParameter("modelViewProj").
			setStateMatrix(CG_GL_MODELVIEW_PROJECTION_MATRIX);

			mat->vertexShader->getNamedParameter("modelView").
			setStateMatrix(CG_GL_MODELVIEW_MATRIX);

			mat->vertexShader->getNamedParameter("modelViewIT").setStateMatrix(
			    CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);

			mat->fragmentShader->getNamedParameter("diffuse").set3fv(
			    mat->diffuse);

			mat->fragmentShader->getNamedParameter("shininess").set1f(
			    mat->shininess);

			auto& cam = sr->getActiveCamera();
			mat->fragmentShader->getNamedParameter("zNear").set1f(
			    cam->getNear());
			mat->fragmentShader->getNamedParameter("zFar").set1f(
			    cam->getFar());
		};

		cgs.shaderSetMap["deferredTexture"] = deferredTextureSet;

		// Set up our sky box
		auto skybox = make_shared<SkyBox>();
		auto skyboxMat = make_shared<Material>();
		skyboxMat->fragmentShader = make_shared<CgProgram>(cgContext, false,
		                    "./resources/shaders/DeferredSkybox.cg",
		                    cgFragProfile, "main");
		skyboxMat->writeToDepth = false;
		skyboxMat->textures.push_back(make_shared<Texture>("./resources/textures/starfield.jpg"));
		for (int face = 0; face < 6; ++face)
			skybox->setFaceMaterial((SkyBox::Face)face, skyboxMat);

		auto skyboxNode = make_shared<SceneNode>();
		skyboxNode->addRenderable(skybox);
		sr->getSceneNodes().push_back(shared_ptr<SceneNode>(skyboxNode));

		// Create a directional light
		auto dirLight = make_shared<DirectionalLight>();
		auto lightSceneNode = make_shared<SceneNode>();
		lightSceneNode->addRenderable(dirLight);
		sr->getSceneNodes().push_back(lightSceneNode);

		// Create sun
		auto sunMat = make_shared<Material>();
		sunMat->depthTest = false;
		sunMat->fragmentShader = skyboxMat->fragmentShader;
		sunMat->textures.push_back(make_shared<Texture>("./resources/textures/Sun.png"));
		sunMat->textures[0]->intParams[GL_TEXTURE_WRAP_S] = GL_REPEAT;
		sunMat->textures[0]->intParams[GL_TEXTURE_WRAP_T] = GL_REPEAT;
		auto sun = make_shared<Sun>(dirLight, sunMat);
		auto sunNode = make_shared<SceneNode>();
		sunNode->addRenderable(sun);
		sr->getSceneNodes().push_back(sunNode);
		// Animate the sun
		am->addanimator(sun);

		// load the map file that will import all the building and road models
		// and textures. Add those to the SceneRenderer.
		MAPFile mapfile("./resources/mooncolony_map.txt");
		auto& nodes =  mapfile.getNodes();
		for (auto i = nodes.begin(); i != nodes.end(); ++i)
			sr->getSceneNodes().push_back(*i);

		cout << "--------------------------------------" << endl;
		cout << "parsing roadmap" << endl;
		auto roadmap = make_shared<RoadMap>("./resources/Moonroads.txt");
		cout << "done with roadmap" << endl;
		cout << "--------------------------------------" << endl;

		auto caranimator = make_shared<CarAnimator>(roadmap, sr);
		OBJFile carObj("./resources/models/sphere3.obj");
		auto numcars = 10;
		for (int i = 0; i < numcars; i++) {
			caranimator->createCar(
			    carObj.getModel(),
			    make_shared<Texture>("./resources/textures/Awesome.png"));
		}

		am->addanimator(caranimator);

		auto crateanimator = make_shared<CrateAnimator>(sr);
		OBJFile crateObj("./resources/models/sat_pod.obj");
		auto cratetext = make_shared<Texture>("./resources/textures/sat_pod.jpg");
		// crate 1
		crateanimator->createCrate(
			    crateObj.getModel(),
			    cratetext,
				Vector3(10, 0, 10),
				6.28f);

		// crate 2
		crateanimator->createCrate(
			    crateObj.getModel(),
			    cratetext,
				Vector3(10, 0, -10),
				12.56f);
		
		// crate 3
		crateanimator->createCrate(
			    crateObj.getModel(),
			    cratetext,
				Vector3(-10, 0, 10),
				3.14f);

		// crate 4
		crateanimator->createCrate(
			    crateObj.getModel(),
			    cratetext,
				Vector3(-10, 0, -10),
				9.42f);

		am->addanimator(crateanimator);


		// add articultaed objects
		auto& crane = make_shared<ArticulatedCrane>(sr, Vector3(60.0f, 0.0f, -60.0f));
		am->addanimator(crane);

		auto& radio = make_shared<ArticulatedRadio>(sr, Vector3(55.0f, 0.0f, -85.0f));
		am->addanimator(radio);

	}
	catch (const Exceptions::Exception& ex) {
		MessageBox(GetActiveWindow(),
		           (string("Initialization error: ") + ex.message).c_str(),
		           "Initialization Error",
		           MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Set up shading model
	glShadeModel(GL_SMOOTH);

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
		//std::string sliderName = CEGUIStyleManager::getSliderName();
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

		controls.radLanding = static_cast<RadioButton*>(wmgr.createWindow(
		                      radioName,
		                      "SelectZoneRoot/Landing"));
		controls.radLanding->setPosition(
		    UVector2(UDim(0.8f, 0.0f), UDim(0.0f, 300.0f)));
		controls.radLanding->setSize(
		    UVector2(UDim(0.18f, 0.0f), UDim(0.0f, 20.0f)));
		controls.radLanding->setText("Landing");
		controls.radLanding->setGroupID(0);
		root->addChildWindow(controls.radLanding);

		controls.radPower = static_cast<RadioButton*>(wmgr.createWindow(
		                      radioName,
		                      "SelectZoneRoot/Power"));
		controls.radPower->setPosition(
		    UVector2(UDim(0.8f, 0.0f), UDim(0.0f, 340.0f)));
		controls.radPower->setSize(
		    UVector2(UDim(0.18f, 0.0f), UDim(0.0f, 20.0f)));
		controls.radPower->setText("Power");
		controls.radPower->setGroupID(0);
		root->addChildWindow(controls.radPower);
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
		sr->setActiveCamera(freeCam);
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
		sr->setActiveCamera(topCam);
	}
	else if (controls.radLanding->isSelected()) {
		sr->setActiveCamera(landingCam);
	}
	else if (controls.radPower->isSelected()) {
		sr->setActiveCamera(powerCam);
	}

	// update any Animators
	am->animate();

	// Enable depth testing and draw our scene
	try {
		glEnable(GL_DEPTH_TEST);
		sr->renderScene();
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

	case '1':
		sr->setDisplayMode(SceneRenderer::DM_UNLIT);
		break;

	case '2':
		sr->setDisplayMode(SceneRenderer::DM_NORMALS);
		break;

	case '3':
		sr->setDisplayMode(SceneRenderer::DM_DEPTH);
		break;

	case '4':
		sr->setDisplayMode(SceneRenderer::DM_LIT);
		break;

	case '5':
		sr->setDisplayMode(SceneRenderer::DM_FINAL);
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
