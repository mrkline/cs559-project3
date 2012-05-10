#include "StdAfx.hpp"
#include "Sun.hpp"

#include "Material.hpp"
#include "Transform.hpp"

Sun::Sun(const std::shared_ptr<Material>& m)
	: mat(m), quadric(gluNewQuadric())
{
	gluQuadricTexture(quadric, GL_TRUE);
}

void Sun::render()
{
	// Ignore the current transform and just render around the camera
	glPopMatrix(); // This returns us to the camera's transform
	Transform camTrans; // Retrieve it
	glGetFloatv(GL_MODELVIEW_MATRIX, camTrans.getArray());
	camTrans.setTranslation(Vector3());

	// Draw the sky box directly around the camera
	glPushMatrix();
	glLoadMatrixf(camTrans.getArray());
	glTranslatef(0, 1, 0);
	setActiveMaterial(mat);
	gluSphere(quadric, 0.1f, 50, 50);
	glPopMatrix();

	// Restore our previous stack level so the engine doesn't get hosed
	glPushMatrix();
}