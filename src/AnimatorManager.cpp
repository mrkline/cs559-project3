#include "StdAfx.hpp"
#include "AnimatorManager.hpp"


AnimatorManager::AnimatorManager(void)
{
	timelast = 0;
}

void AnimatorManager::animate()
{
	double dt = 0;
	timenow = timeGetTime();
	if(timelast != 0)
		dt = timenow - timelast;
	timelast = timenow;
	if(animators.size() > 0)
	{
		for(auto i = animators.begin(); i != animators.end(); i++)
		{
			(*i)->animate(dt);
		}
	}	
}

AnimatorManager::~AnimatorManager(void)
{
}
