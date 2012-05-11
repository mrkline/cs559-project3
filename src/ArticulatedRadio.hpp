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
	ArticulatedRadio(SceneRenderer* sr, Vector3 location);
	void animate(double dt);
	void rotateDish();
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

