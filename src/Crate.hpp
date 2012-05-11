#pragma once

#include <memory>

#include "Renderable.hpp"
#include "Vector3.hpp"

class Model;

//! A bobble that sits in one location, spinning.
class Crate: public Renderable
{
public:
	
	//! Create a Crate that rotates with the given speed.
	//! \param model The model this crate should use to render.
	//! \param ds The number of radians to rotate, per second.
	Crate(const std::shared_ptr<Model>& model, float ds);

	//! implement Renderable.
	void render();

	//! return this Crate's current world location.
	Vector3 getLocation() const;
	
	//! set this Crate's current world location.
	//! \param newLoc The Vector3 representing new world coords of this crate.
	void setLocation(Vector3 newLoc);

	//! update based on the amount of time passed, dt.
	//! \param dt The amount of time since this was last called, in seconds.
	void updateRotation(double dt);

	//! return the Model used to render this Crate
	std::weak_ptr<Model> getModel() { return std::weak_ptr<Model>(model); }

private:
	std::shared_ptr<Model> model;
	float rotationspeed;	// radians to rotate per second
};

