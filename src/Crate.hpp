#pragma once

#include <memory>

#include "Renderable.hpp"
#include "Vector3.hpp"

class Model;

//! A bobble that sits in one location, spinning.
class Crate: public Renderable
{
public:
	Crate(const std::shared_ptr<Model>& model, float ds);

	void render();

	Vector3 getLocation() const;

	void setLocation(Vector3 newLoc);

	void updateRotation(double dt);

	std::weak_ptr<Model> getModel() { return std::weak_ptr<Model>(model); }

private:
	std::shared_ptr<Model> model;
	float rotationspeed;	// radians to rotate per second
};

