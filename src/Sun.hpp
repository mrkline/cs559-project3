#pragma once
#include "Sphere.hpp"

#include <memory>

#include "Material.hpp"
#include "Renderable.hpp"

class Sun : public Sphere
{
public:
	Sun(const std::shared_ptr<Material>& m = nullptr);

	~Sun() { gluDeleteQuadric(quadric); }

	RenderableType getRenderableType() const { return RT_BACKGROUND; }

	void render();

	const std::shared_ptr<Material>& getMaterial() { return mat; }

	void setMaterial(const std::shared_ptr<Material>& m) { mat = m; }

private:
	std::shared_ptr<Material> mat;
	GLUquadric* quadric;
};

