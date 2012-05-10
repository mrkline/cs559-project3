#include "StdAfx.hpp"
#include "Crate.hpp"


#include "Animator.hpp"
#include "Model.hpp"
#include "SceneNode.hpp"

Crate::Crate(const std::shared_ptr<Model>& model, float ds)
{
	this->model = model;
	this->rotationspeed = ds;
}

void Crate::render()
{
	model->render();
}

Vector3 Crate::getLocation() const
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

void Crate::updateRotation(double dt)
{
	if(auto& ptr = this->owner.lock())
	{
		// get hold of the current transform and rotate based on 
		// the amount of time passed
		auto& currXfrm = ptr->getTransform();
		double rotate = rotationspeed * dt;
		currXfrm.rotateRadians(Vector3(0, (float) rotate, 0));
	}
}


Crate::~Crate(void)
{
}
