#pragma once

#include "CgErrors.hpp"

//! An RAII wrapper around CGcontext
class CgContext
{
public:
	CgContext()
		: ctx(cgCreateContext())
	{ throwCgExceptions(__FUNCTION__); };

	~CgContext() { cgDestroyContext(ctx); }

	CGcontext getHandle() { return ctx; }

private:
	CGcontext ctx;
};
