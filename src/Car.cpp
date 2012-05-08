#include "StdAfx.hpp"
#include "Car.hpp"

#include "Animator.hpp"
#include "Model.hpp"
#include "RoadMap.hpp"
#include "RoadMapNode.hpp"
#include "SceneNode.hpp"

Car::Car(const shared_ptr<Model>& model, float speed)
{
	this->model = model;
	this->speed = speed;
}

void Car::render()
{
	model->render();
}

Vector3 Car::getLocation() const
{
	if(auto ptr = this->owner.lock())
	{
		return ptr->getTransform().getTranslation();
	}
	else
	{
		return Vector3(0,0,0);
	}
}

void Car::setDestination(const shared_ptr<RoadMapNode>& newDest)
{
	destination = newDest;
}

void Car::setOrigin(const shared_ptr<RoadMapNode>& newOrig)
{
	origin = newOrig;
}

void Car::setLocation(Vector3 newLoc)
{
	if(auto ptr = this->owner.lock())
	{
		ptr->getTransform().setTranslation(newLoc);
	}
}

void Car::updateRotation()
{
	if(auto& ptr = this->owner.lock())
	{
		// compute which way the car should be facing
		auto& currXfrm = ptr->getTransform();
		Vector2 currloc = Vector2(currXfrm.getTranslation().X, currXfrm.getTranslation().Z);
		Vector2 dest = this->getDestination()->getLocation();
		Vector2 toface = dest - currloc;
		toface.normalize();
		auto desiredangle = Vector3(0, acos(Vector2::dotProduct(Vector2(0,1), toface)), 0);
		auto currentangle = currXfrm.getRotationRadians();
		auto rotangle = desiredangle - currentangle;
		currXfrm.rotateRadians(rotangle);
	}
}

Car::~Car(void)
{
}
