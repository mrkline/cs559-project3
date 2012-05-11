#pragma once

#include <vector>

#include "Animator.hpp"
#include "Car.hpp"
#include "Model.hpp"
#include "RoadMap.hpp"
#include "RoadMapNode.hpp"
#include "SceneRenderer.hpp"

//! Holds on to the cars in a scene and knows how to animate them them by
//! moving the scene nodes around.
class CarAnimator: public Animator
{
private:
	// hold onto each car
	vector<shared_ptr<Car>> cars;
	shared_ptr<RoadMap> map;
	shared_ptr<SceneRenderer> sr;

public:
	CarAnimator(shared_ptr<RoadMap> map, SceneRenderer* sr);

	//! create a car with the given model at a random, valid locationon the map.
	void createCar(const shared_ptr<Model>& model,
			const shared_ptr<Material>& mat);

	void animate(double dt);

	//! returns a postion that is sure to be on the roadmap and a random
	//! connection to that node to set as destination.
	pair<shared_ptr<RoadMapNode>, shared_ptr<RoadMapNode>> 
		getRandomLocationAndDestination();

	~CarAnimator(void);
};

