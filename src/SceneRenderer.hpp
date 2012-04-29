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
	std::shared_ptr<Texture> screen;
	std::shared_ptr<Material> screenMat;
	std::list<std::shared_ptr<SceneNode>> sceneNodes;
	std::shared_ptr<Camera> activeCamera;
};
