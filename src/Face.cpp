#include "StdAfx.hpp"
#include "Face.hpp"
#include "Material.hpp"
#include "GL/freeglut_ext.h"


Face::Face(void)
{
}

void Face::render()
{
	setActiveMaterial(getDefaultMaterial());
	///*
	int numcoords = 3 * vertices.size();
	// check if the vertex array has been built before.
	if(vertexarray == nullptr)
	{		
		vertexarray = new GLfloat[numcoords];
		for(int i = 0; i < vertices.size(); i += 3)
		{
			vertexarray[i] = vertices[i].X;
			vertexarray[i+1] = vertices[i].Y;
			vertexarray[i+2] = vertices[i].Z;
		}		
	}

	//glBindT
	
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexarray);
	if(vertices.size() == 3)
		glDrawArrays(GL_TRIANGLES, 0, numcoords);
	else
		glDrawArrays(GL_QUADS, 0, numcoords);

	glDisableClientState(GL_VERTEX_ARRAY);
	*/
	
	/*	
	if(vertices.size() == 3)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_QUADS);
	for(auto i = vertices.begin(); i != vertices.end(); i++)
	{
		glVertex3f(i->X, i->Y, i->Z);
	}

	glEnd();
	*/
}

Face::~Face(void)
{
}
