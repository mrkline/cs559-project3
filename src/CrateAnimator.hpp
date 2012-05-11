#pragma once

#include <vector>

#include "Animator.hpp"
#include "Crate.hpp"
#include "Model.hpp"
#include "SceneRenderer.hpp"
#include "Vector3.hpp"

using namespace std;

//! Holds on to the crates in a scene and knows how to animate them.
class CrateAnimator: public Animator
{
private:
	// hold onto each crate
	vector<shared_ptr<Crate>> crates;
	shared_ptr<SceneRenderer> sr;

public:
	CrateAnimator(SceneRenderer* sr);
	// create a crate
	void createCrate(const shared_ptr<Model>& model,
	                 const shared_ptr<Texture>& texture,
	                 const Vector3& location, float rotationspeed);
	void animate(double dt);
	~CrateAnimator(void);
};

