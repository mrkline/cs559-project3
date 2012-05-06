#include "StdAfx.hpp"
#include "Car.hpp"

Car::Car(shared_ptr<Model> model, float speed)
{
	this->model = model;
	this->speed = speed;
}

void Car::render()
{
	model->render();
}

Vector3 Car::getLocation()
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

void Car::setLocation(Vector3 newLoc)
{
	if(auto ptr = this->owner.lock())
	{
		ptr->getTransform().setTranslation(newLoc);
	}
}

void Car::updateRotation()
{
	if(auto ptr = this->owner.lock())
	{
		// compute which way the car should be facing
		Vector2 north = Vector2(0,1);
		Vector3 currloc3 = ptr->getTransform().getTranslation();
		Vector2 currloc = Vector2(currloc3.X, currloc3.Z);
		Vector2 dest = this->getDestination()->getLocation();
		Vector2 face = dest - currloc;
		face.normalize();
		float angle = acos(Vector2::dotProduct(north, face));
		Vector3 rotation = Vector3(0, angle, 0);	
	
		// get the current rotation, undo it, and set the new rotation
		auto rot = ptr->getTransform().getRotationDegrees();
		ptr->getTransform().rotateDegrees(Vector3(-rot.X, -rot.Y, -rot.Z));
		ptr->getTransform().rotateDegrees(rotation);
	}
}

Car::~Car(void)
{
}
