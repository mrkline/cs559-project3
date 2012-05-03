#pragma once

#include "CgContext.hpp"
#include "CgProfile.hpp"

//! A simple singleton class to provide global access to our shader context and
//! profiles
class CgSingleton
{
public:
	static CgSingleton& getSingleton()
	{
		static CgSingleton instance;
		return instance;
	}

	CgContext& getContext() { return *context; }

	CgProfile& getVertexProfile() { return *vp; }

	CgProfile& getFragmentProfile() { return *fp; }

private:
	CgSingleton()
	{
		context = new CgContext;
		vp = new CgProfile(*context, CG_GL_VERTEX);
		printf("Cg vertex profile: %s\n", vp->getName());
		fp = new CgProfile(*context, CG_GL_FRAGMENT);
		printf("Cg fragment profile: %s\n", fp->getName());
	}

	CgContext* context;
	CgProfile* vp;
	CgProfile* fp;
};
