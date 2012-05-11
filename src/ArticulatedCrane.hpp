#pragma once

#include "Model.hpp"
#include "SceneRenderer.hpp"
#include "SceneNode.hpp"
#include "Vector3.hpp"
#include "Renderable.hpp"
#include "Animator.hpp"
#include "OBJFile.hpp"

using namespace std;


//! An articulated object that allows rotating the arm about the base,
//! moving the load in and out along the arm, and raising and lowering
//! the load.
class ArticulatedCrane: public Animator
{
private:
	Vector3 basevector;
	Vector3 armvector;
	Vector3 loadvector;
	float loaddistance;
	float loadheight;
	float dtheta;
	float dx;
	float dy;
	float outmax;
	float downmax;
	bool loaddirout;
	bool loaddirdown;
	shared_ptr<Model> basemodel;
	shared_ptr<Model> armmodel;
	shared_ptr<Model> loadmodel;
	shared_ptr<SceneNode> basesn;
	shared_ptr<SceneNode> armsn;
	shared_ptr<SceneNode> loadsn;

public:
	//! create the crane at the specified location and add the constiuent nodes
	//! to the scenerender
	//! \param sr The SceneRender to which we should add the SceneNodes
	//! \param location The Vector3 representing the location of the base
	ArticulatedCrane(SceneRenderer* sr, Vector3 location);
	
	//! update based on the amount of time passed, dt.
	//! \param dt The amount of time since this was last called, in seconds.
	void animate(double dt);
	
	//! rotate the arm of the crane a preset number of degrees.
	void rotateArm();
	
	//! move the load object out a preset amount along the arm.
	void extendLoad();
	
	//! retract the load object in a preset amount along the arm.
	void retractLoad();
	
	//! raise the load object a preset amount.
	void raiseLoad();
	
	//! lower the load object a preset amount.
	void lowerLoad();
};

