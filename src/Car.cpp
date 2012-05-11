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
		// get hold of the current transform and the 2D map location and destination
		auto& currXfrm = ptr->getTransform();
		Vector2 currloc = Vector2(currXfrm.getTranslation().X, currXfrm.getTranslation().Z);
		Vector2 dest = this->getDestination()->getLocation();
		// figure out which way we should be facing
		Vector2 toface = dest - currloc;
		float rotate = 0.0f;
		toface.normalize();
		float desiredangle = 0.0f;
		
		// hacky - set the rotation angle explictly to avoid wobbling because of
		// small changes in the rotation angle
		if(toface.X >= 0.95f && toface.X <= 1.05f)
			rotate = -90.0f;
		else if(toface.X <= -0.95f && toface.X >= -1.05f)
			rotate = 90.0f;
		else if(toface.X <= .05f && toface.X >= -0.05f)
		{
			if(toface.Y >= 0.95f && toface.Y <= 1.05f)
				rotate = 180.0f;
			else if(toface.Y <= -0.95f && toface.Y >= -1.05f)
				rotate = 0.0f;
		}

		// save off the current translation, reset the matrix (to clean any current
		// rotation), put the xlat back, and rotate the matrix properly
		auto xlat = currXfrm.getTranslation();
		currXfrm.setToIdentity();
		currXfrm.setTranslation(xlat);
		currXfrm.rotateDegrees(Vector3(0, rotate, 0));
	}
}
