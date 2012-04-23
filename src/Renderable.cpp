#include "Renderable.hpp"

#include "SceneNode.hpp"

using namespace std;

void Renderable::setOwner(const weak_ptr<SceneNode>& newOwner)
{
	auto o = owner.lock();
	if (o)
		o->removeRenderable(shared_from_this());

	owner = newOwner;
}
