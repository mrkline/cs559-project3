#pragma once

#include <list>

class Camera;
class SceneNode;
class Renderable;

class SceneManager
{
public:
	std::list<SceneNode*>& getSceneNodes() { return sceneNodes; }

	const std::list<SceneNode*>& getSceneNodes() const { return sceneNodes; }

	void renderScene() const;

	void setActiveCamera(Camera* cam) { activeCamera = cam; }

private:
	std::list<SceneNode*> sceneNodes;
	Camera* activeCamera;
};
