#pragma once

#include <list>
#include <memory>

#include "FrameBuffer.hpp"

class Camera;
struct Material;
class SceneNode;
class Texture;

class SceneRenderer
{
public:
	SceneRenderer(size_t screenWidth, size_t screenHeight);

	std::list<std::shared_ptr<SceneNode>>& getSceneNodes() { return sceneNodes; }

	const std::list<std::shared_ptr<SceneNode>>& getSceneNodes() const { return sceneNodes; }

	void renderScene();

	void setActiveCamera(std::shared_ptr<Camera>& cam) { activeCamera = cam; }

private:
	FrameBuffer fb;
	//! This texture contains unlit and emissive color data
	std::shared_ptr<Texture> unlit;
	//! This texture contains world-space normals and depth data
	std::shared_ptr<Texture> normAndDepth;
	//! This texture contains light colors and specular coefficients
	std::shared_ptr<Texture> lit;
	//! Material used for displaying textures on fullscreen quads
	std::shared_ptr<Material> screenMat;
	std::list<std::shared_ptr<SceneNode>> sceneNodes;
	std::shared_ptr<Camera> activeCamera;
};
