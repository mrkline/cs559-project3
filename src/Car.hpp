#pragma once
#include "Renderable.hpp"
#include "Animator.hpp"
#include "Model.hpp"
#include "Vector3.hpp"
#include "RoadMap.hpp"
#include "RoadMapNode.hpp"
#include "SceneNode.hpp"

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
	void setDestination(shared_ptr<RoadMapNode> newDest){this->destination = newDest;};
	void setOrigin(shared_ptr<RoadMapNode> newOrig){this->origin = newOrig;};
	// adjusts the rotation based on the current location and the destination
	void updateRotation();
	weak_ptr<Model> getModel(){return weak_ptr<Model>(model);};
	~Car(void);
	
};

