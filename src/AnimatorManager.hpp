#pragma once

#include <list>

#include "Animator.hpp"

//! Maintains a list of Animator objects and keeps track of the last time its
//! animate method was called.
class AnimatorManager
{
public:
	AnimatorManager(void);

	void animate();

	void addanimator(std::shared_ptr<Animator> in)
	{ this->animators.push_back(in); }

private:
	double timelast, timenow;

	std::list<std::shared_ptr<Animator>> animators;
};

