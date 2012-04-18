#include "StdAfx.hpp"
#include "CgErrors.hpp"

void throwCgExceptions(const char* callingFunctionName)
{
	CGerror err;
	const char* msg = cgGetLastErrorString(&err);
	if (err != CG_NO_ERROR) {
		throw Exceptions::CgException(msg, callingFunctionName);
	}
}
