#pragma once

#include "Model.hpp"
#include "SceneRenderer.hpp"
#include "SceneNode.hpp"
#include "Vector3.hpp"
#include "Renderable.hpp"
#include "Animator.hpp"
#include "OBJFile.hpp"

using namespace std;


//! An articulated object that allows rotating the dish about the base,
//! and moving the mast to different angles.
class ArticulatedRadio: public Animator
{
public:
	//! create the radio at the specified location and add the constiuent nodes
	//! to the scenerender
	//! \param sr The SceneRender to which we should add the SceneNodes
	//! \param location The Vector3 representing the location of the base
	ArticulatedRadio(SceneRenderer* sr, Vector3 location);
	
	//! update based on the amount of time passed, dt.
	//! \param dt The amount of time since this was last called, in seconds.
	void animate(double dt);
	
	//! rotate the dish around the base a preset number of degrees.
	void rotateDish();
	
	//! move the mast back and forth, around the base.
	void waveMast();

private:
	Vector3 basevector;
	Vector3 mastvector;
	Vector3 dishvector;
	float dtheta;
	float mastangle;
	bool radioforward;
	shared_ptr<Model> basemodel;
	shared_ptr<Model> mastmodel;
	shared_ptr<Model> dishmodel;
	shared_ptr<SceneNode> basesn;
	shared_ptr<SceneNode> mastsn;
	shared_ptr<SceneNode> dishsn;
};

