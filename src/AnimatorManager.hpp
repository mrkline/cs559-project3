#pragma once
#include "Animator.hpp"
#include <list>
#include <Windows.h>
#include <MMSystem.h>

//! Maintains a list of Animator objects and keeps track of the last time its
//! animate method was called.
class AnimatorManager
{
private:
	double timelast, timenow;
	std::list<std::shared_ptr<Animator>> animators;

public:
	AnimatorManager(void);
	void animate();
	void addanimator(std::shared_ptr<Animator> in){this->animators.push_back(in);};
	~AnimatorManager(void);

};

