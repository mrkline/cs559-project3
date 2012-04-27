#include "StdAfx.hpp"
#include "Face.hpp"
#include "Material.hpp"
#include "GL/freeglut_ext.h"


Face::Face(void)
{
	createdVBO = false;
}

void Face::render()
{
	
	setActiveMaterial(getDefaultMaterial());
	
	if(!createdVBO)
	{
		numcoords = 3 * vertices.size();
		// copy the vertices into an array to store in the VBO
		vertexarray = new GLfloat[numcoords];
		for(int i = 0; i < vertices.size(); i++)
		{
			vertexarray[i*3] = vertices[i].X;
			vertexarray[i*3+1] = vertices[i].Y;
			vertexarray[i*3+2] = vertices[i].Z;
		}

		// initialize the VBO:
		// create the buffer
		glGenBuffers(1, &vboId);
		// bind GL to it
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		// actually copy the data over
		glBufferData(GL_ARRAY_BUFFER, numcoords * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);
		// clear the binding (ugh, this was hard fought)
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		createdVBO = true;
	}
	
	// rebind to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	// we're using these as position data
	glEnableVertexAttribArray(0);
	// tell GL what this buffer is
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// draw the polygon
	if(vertices.size() == 3)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	else
	{
		glDrawArrays(GL_QUADS, 0, vertices.size());
	}
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

Face::~Face(void)
{
}
