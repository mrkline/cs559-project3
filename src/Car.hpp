#pragma once

#include <memory>

#include "Renderable.hpp"
#include "Vector3.hpp"

class Model;
class RoadMapNode;

class Car: public Renderable
{
public:
	Car(const std::shared_ptr<Model>& model, float speed);
	void render();
	float getSpeed() const { return speed; }
	void setSpeed(float speed) { this->speed = speed; }
	Vector3 getLocation() const;
	const std::shared_ptr<RoadMapNode>& getDestination() const
		{ return destination; }
	const std::shared_ptr<RoadMapNode>& getOrigin() const { return origin; }
	void setLocation(Vector3 newLoc);
	void setDestination(const std::shared_ptr<RoadMapNode>& newDest);
	void setOrigin(const std::shared_ptr<RoadMapNode>& newOrig);
	// adjusts the rotation based on the current location and the destination
	void updateRotation();
	std::weak_ptr<Model> getModel() { return std::weak_ptr<Model>(model); }
	~Car(void);
	
private:
	std::shared_ptr<Model> model;
	std::shared_ptr<RoadMapNode> origin, destination;
	float speed;
};

