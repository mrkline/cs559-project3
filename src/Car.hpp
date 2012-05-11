#pragma once

#include <memory>

#include "Renderable.hpp"
#include "Vector3.hpp"

class Model;
class RoadMapNode;

//! a vehicle that moves around a map, between it's constantly updating origin
//! and destination.
class Car: public Renderable
{
public:

	//! create a Car with the given model and speed
	//! \param model The model to use when rendering this Car
	//! \param speed The speed of the car, in units per second
	Car(const std::shared_ptr<Model>& model, float speed);

	//! implement Renderable.
	void render();

	//! the current speed used for animating this car
	float getSpeed() const { return speed; }
	
	//! set the speed used for animating this car
	//! \param speed The new speed
	void setSpeed(float speed) { this->speed = speed; }

	//! the current location of this car
	Vector3 getLocation() const;
	
	//! the RoadMapNode this car is currently headed to
	const std::shared_ptr<RoadMapNode>& getDestination() const
		{ return destination; }
	
	//! the RoadMapNode this car is departing from
	const std::shared_ptr<RoadMapNode>& getOrigin() const { return origin; }

	//! update this Car's location
	//! \param newLoc this Car's new location
	void setLocation(Vector3 newLoc);
	
	//! update this Car's destination
	//! \param newDest this Car's new destination
	void setDestination(const std::shared_ptr<RoadMapNode>& newDest);
	
	//! update this Car's origin
	//! \param newOrig this Car's new new origin
	void setOrigin(const std::shared_ptr<RoadMapNode>& newOrig);

	//! adjusts the rotation based on the current location and the destination
	void updateRotation();
	
	//! return the Model this Car uses to render
	std::weak_ptr<Model> getModel() { return std::weak_ptr<Model>(model); }

private:
	std::shared_ptr<Model> model;
	std::shared_ptr<RoadMapNode> origin, destination;
	float speed;
};

