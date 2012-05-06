#pragma once

//! This is an interface for an object that is animated (i.e. may change over
//! time)
class Animator
{
public:
	//! update based on the amount of time passed, dt.
	virtual void animate(double dt) = 0;
};

