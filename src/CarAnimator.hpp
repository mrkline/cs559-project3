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
	//! Initialize the CarAnimator which will add and manage Car objects.
	//! \param map The RoadMap used to keep Cars in valid locations.
	//! \param sr The SceneRender to which we should add the SceneNodes
	CarAnimator(shared_ptr<RoadMap> map, SceneRenderer* sr);

	//! create and add a car with at a random, valid location on the map.
	//! \param model The model to use to render the new Car
	//! \param mat The material used on this car
	void createCar(const shared_ptr<Model>& model,
			const shared_ptr<Material>& mat);
	
	//! update based on the amount of time passed, dt.
	//! \param dt The amount of time since this was last called, in seconds.
	void animate(double dt);

	//! returns a postion that is sure to be on the roadmap and a random
	//! connection to that node to set as destination.
	pair<shared_ptr<RoadMapNode>, shared_ptr<RoadMapNode>> 
		getRandomLocationAndDestination();

	~CarAnimator(void);
};

