#pragma once

#include "CgErrors.hpp"

//! A wrapper around Cg profiles for easy initialization
class CgProfile
{
public:
	CgProfile(CGGLenum profClass, bool setOptimalOpts = true)
		: prof(cgGLGetLatestProfile(profClass))
	{
		if (setOptimalOpts)
			cgGLSetOptimalOptions(prof);

		throwCgExceptions(__FUNCTION__);
	}

	void enable()
	{ cgGLEnableProfile(prof); throwCgExceptions(__FUNCTION__); }

	void disable()
	{ cgGLDisableProfile(prof); throwCgExceptions(__FUNCTION__); }

	CGprofile getHandle() { return prof; }

private:
	CGprofile prof;
};
