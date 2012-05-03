#include "StdAfx.hpp"
#include "SceneRenderer.hpp"

#include <deque>

#include "SceneNode.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Vector3.hpp"
#include "CgSingleton.hpp"
#include "CgProgram.hpp"

using namespace std;

SceneRenderer::SceneRenderer(size_t screenWidth, size_t screenHeight)
	: fb(screenWidth, screenHeight)
{
	unlit = make_shared<Texture>(nullptr, 4, screenWidth, screenHeight,
	                              GL_RGBA, GL_UNSIGNED_BYTE, false);

	normAndDepth = make_shared<Texture>(nullptr, 4, screenWidth, screenHeight,
			GL_RGBA, GL_UNSIGNED_BYTE, false);

	lit = make_shared<Texture>(nullptr, 4, screenWidth, screenHeight,
			GL_RGBA, GL_UNSIGNED_BYTE, false);
	fb.attachTexture(unlit, 0);
	fb.attachTexture(normAndDepth, 1);
	fb.attachTexture(lit, 2);
	fb.setNumRenderTargets(2); //!< \todo Set to 3

	auto& cgContext = CgSingleton::getSingleton().getContext();
	auto& fragProfile = CgSingleton::getSingleton().getFragmentProfile();
	stripAlphaShader = make_shared<CgProgram>(cgContext, false,
			"./resources/shaders/StripAlpha.cg",
			fragProfile, "main");
	alphaOnlyShader = make_shared<CgProgram>(cgContext, false,
			"./resources/shaders/AlphaOnly.cg",
			fragProfile, "main");

	screenMat = make_shared<Material>();
	screenMat->textures.push_back(normAndDepth);
	screenMat->fragmentShader = stripAlphaShader;
	screenMat->depthTest = false;
}

void SceneRenderer::renderScene()
{
	// There's no point in drawing a scene if we don't have an active camera
	if (!activeCamera)
		return;

	// List of renderables to render
	list<Renderable*> lights;
	list<Renderable*> bg;
	list<Renderable*> normals;

	// Update the transforms of all scene nodes via bfs and add their
	// renderables to the various queues
	deque<SceneNode*> q;
	for (auto it = sceneNodes.begin(); it != sceneNodes.end(); ++it)
		q.push_back(it->get());

	while (!q.empty()) {
		auto curr = q.front();
		q.pop_front();

		curr->updateAbsoluteTransform();
		const auto& renderables = curr->getRenderables();
		for (auto it = renderables.begin(); it != renderables.end(); ++it) {
			switch ((*it)->getType()) {
			case Renderable::RT_LIGHT:
				lights.push_back(it->get());
				break;

			case Renderable::RT_BACKGROUND:
				bg.push_back(it->get());
				break;

			case Renderable::RT_NORMAL:
				normals.push_back(it->get());
				break;
			}
		}

		// Enqueue all the node's children
		const auto& currChildren = curr->getChildren();
		for (auto it = currChildren.begin(); it != currChildren.end(); ++it)
			q.push_back(it->get());
	}

	// Render to the screen texture
	fb.setupRender();

	// Draw our camera first
	activeCamera->render();

	// Draw all lights next
	for (auto it = lights.begin(); it != lights.end(); ++it)
		if ((*it)->isVisible()) (*it)->render();

	// Draw all background objects
	for (auto it = bg.begin(); it != bg.end(); ++it) {
		if ((*it)->isVisible()) {
			glPushMatrix();
			glMultMatrixf((*it)->getOwner().lock()->
			              getAbsoluteTransform().getArray());
			(*it)->render();
			glPopMatrix();
		}
	}

	// Clear the depth buffer, allowing normal objects to always be drawn
	// over the background ones
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw all normal objects
	for (auto it = normals.begin(); it != normals.end(); ++it) {
		if ((*it)->isVisible()) {
			glPushMatrix();
			glMultMatrixf((*it)->getOwner().lock()->
			              getAbsoluteTransform().getArray());
			(*it)->render();
			glPopMatrix();
		}
	}

	fb.cleanupRender();

	// Draw our rendered texture on a quad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw at the back of the frustum (so we can use the vertices for depth)
	Transform invView;
	glGetFloatv(GL_MODELVIEW_MATRIX, invView.getArray());
	invView.setToInverse();
	Vector3 ul(-1.0f, 1.0f, 0.99f);
	Vector3 ll(-1.0f, -1.0f, 0.99f);
	Vector3 lr(1.0f, -1.0f, 0.99f);
	Vector3 ur(1.0f, 1.0f, 0.99f);
	invView.transformPoint(ul);
	invView.transformPoint(ll);
	invView.transformPoint(lr);
	invView.transformPoint(ur);
	setActiveMaterial(screenMat);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(ul.X, ul.Y, ul.Z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(ll.X, ll.Y, ll.Z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(lr.X, lr.Y, lr.Z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(ur.X, ur.Y, ur.Z);
	glEnd();
}


void SceneRenderer::setDisplayMode(DisplayMode dm)
{
	switch (dm) {
	case DM_UNLIT:
		screenMat->textures[0] = unlit;
		screenMat->fragmentShader = stripAlphaShader;
		break;

	case DM_NORMALS:
		screenMat->textures[0] = normAndDepth;
		screenMat->fragmentShader = stripAlphaShader;
		break;

	case DM_DEPTH:
		screenMat->textures[0] = normAndDepth;
		screenMat->fragmentShader = alphaOnlyShader;
		break;

	case DM_LIT:
		break;

	case DM_SPECULAR:
		break;
	}
}
