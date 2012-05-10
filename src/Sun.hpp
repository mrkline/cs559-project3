#pragma once
#include "Sphere.hpp"

class Sun : public Sphere
{
public:
	Sun(const std::shared_ptr<Material>& m = nullptr) : mat(m) { };
	RenderableType getRenderableType() const { return RT_NORMAL; }

private:
	private:
	std::shared_ptr<Material> mat;
};

