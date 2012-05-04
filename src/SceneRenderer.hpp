#pragma once

#include <list>
#include <memory>

#include "FrameBuffer.hpp"

class Camera;
struct Material;
class SceneNode;
class Texture;
class CgProgram;

class SceneRenderer
{
public:
	enum DisplayMode {
		DM_UNLIT, //!< Display unlit and emissive light
		DM_NORMALS, //!< Display view-space normals
		DM_DEPTH, //!< Display normalized depth
		DM_LIT, //!< Lit color
		DM_SPECULAR //!< Specular coefficient
	};

	SceneRenderer(size_t screenWidth, size_t screenHeight);

	std::list<std::shared_ptr<SceneNode>>& getSceneNodes() { return sceneNodes; }

	const std::list<std::shared_ptr<SceneNode>>& getSceneNodes() const { return sceneNodes; }

	void renderScene();

	const std::shared_ptr<Camera>& getActiveCamera() { return activeCamera; }

	void setActiveCamera(std::shared_ptr<Camera>& cam) { activeCamera = cam; }

	void setDisplayMode(DisplayMode dm);

private:
	//! The frame buffer used to render to multiple target textures
	FrameBuffer fb;

	//! This texture contains unlit and emissive color data
	std::shared_ptr<Texture> unlit;

	//! This texture contains world-space normals and depth data
	std::shared_ptr<Texture> normAndDepth;

	//! This texture contains light colors and specular coefficients
	std::shared_ptr<Texture> lit;

	//! Material used for displaying textures on fullscreen quads
	std::shared_ptr<Material> screenMat;

	//! A shader used to strip the alpha channel from a texture in order
	//! to display only its RGB components
	std::shared_ptr<CgProgram> stripAlphaShader;

	//! A shader used to show the alpha channel fo a texture
	std::shared_ptr<CgProgram> alphaOnlyShader;

	//! The list of scene nodes in the scene
	std::list<std::shared_ptr<SceneNode>> sceneNodes;

	//! The camera we're currently rendering from
	std::shared_ptr<Camera> activeCamera;
};
