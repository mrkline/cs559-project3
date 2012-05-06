#pragma once
#include "Renderable.hpp"
#include "Animator.hpp"
#include "Model.hpp"
#include "Vector3.hpp"

class Car: Animator, Renderable
{
private:
	std::shared_ptr<Model> model;
	Vector3 location;
	Vector3 destination;	// need to check if past destination and deal with it
	float rotation;
	const static int dx = 1; // distance to go each tick

public:
	Car(const char* filename);
	void render();
	void animate(float dt);
	void setdirection(Vector3 dir);
	void setlocation(Vector3 loc);
	void setrotation(float rot);
	~Car(void);
};

