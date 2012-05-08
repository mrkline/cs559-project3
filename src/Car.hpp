#pragma once

#include "Animator.hpp"
#include "Model.hpp"
#include "Renderable.hpp"
#include "RoadMap.hpp"
#include "RoadMapNode.hpp"
#include "SceneNode.hpp"
#include "Vector3.hpp"

class Car: public Renderable
{
private:
	std::shared_ptr<Model> model;
	shared_ptr<RoadMapNode> origin, destination;
	float speed;

public:
	Car(shared_ptr<Model> model, float speed);
	void render();
	float getSpeed(){return this->speed;};
	void setSpeed(float speed){this->speed = speed;}; 
	Vector3 getLocation();
	shared_ptr<RoadMapNode> getDestination(){return this->destination;};
	shared_ptr<RoadMapNode> getOrigin(){return this->origin;};
	void setLocation(Vector3 newLoc);
	void setDestination(shared_ptr<RoadMapNode> newDest);
	void setOrigin(shared_ptr<RoadMapNode> newOrig);
	// adjusts the rotation based on the current location and the destination
	void updateRotation();
	weak_ptr<Model> getModel(){return weak_ptr<Model>(model);};
	~Car(void);
	
};

