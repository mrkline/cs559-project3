#pragma once

#include "Transform.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

class CgNamedParameter
{
public:
	//! Constructor should only be called by CgProgram
	CgNamedParameter(CGprogram prog, const char* name)
		: param(cgGetNamedParameter(prog, name))
	{ throwCgExceptions(__FUNCTION__); }

	void setTransform(const Transform& t)
	{
		//! \todo Should this be cgGLSetMatrixParameterfc?
		cgGLSetMatrixParameterfr(param, t.getArray());
		throwCgExceptions(__FUNCTION__);
	}

	void setStateMatrix(CGGLenum matrix,
	                    CGGLenum transform = CG_GL_MATRIX_IDENTITY)
	{
		cgGLSetStateMatrixParameter(param, matrix, transform);
		throwCgExceptions(__FUNCTION__);
	}

	void set1f(float f)
	{
		cgGLSetParameter1f(param, f);
		throwCgExceptions(__FUNCTION__);
	}

	void set3fv(const float* v)
	{
		cgGLSetParameter3fv(param, v);
		throwCgExceptions(__FUNCTION__);
	}

	void set3f(float x, float y, float z)
	{
		cgGLSetParameter3f(param, x, y, z);
		throwCgExceptions(__FUNCTION__);
	}

	void setVector3(const Vector3& v)
	{
		cgGLSetParameter3f(param, v.X, v.Y, v.Z);
		throwCgExceptions(__FUNCTION__);
	}

	void set2fv(const float* v)
	{
		cgGLSetParameter2fv(param, v);
		throwCgExceptions(__FUNCTION__);
	}

	void set2f(float x, float y)
	{
		cgGLSetParameter2f(param, x, y);
		throwCgExceptions(__FUNCTION__);
	}

	void setVector2(const Vector2& v)
	{
		cgGLSetParameter2f(param, v.X, v.Y);
		throwCgExceptions(__FUNCTION__);
	}

private:
	CGparameter param;
};
