#include "StdAfx.hpp"
#include "Face.hpp"
#include "Material.hpp"
#include "GL/freeglut_ext.h"


Face::Face(void)
{
	createdVBOs = false;
}

void Face::render()
{
	
	if(!createdVBOs)
	{
		numcoords = 3 * vertices.size();
		// copy the vertices into an array to store in the VBO
		vertexarray = new GLfloat[numcoords];
		normalarray  = new GLfloat[numcoords];
		for(int i = 0; i < vertices.size(); i++)
		{
			vertexarray[i*3] = vertices[i].X;
			vertexarray[i*3+1] = vertices[i].Y;
			vertexarray[i*3+2] = vertices[i].Z;
			normalarray[i*3] = normals[i].X;
			normalarray[i*3+1] = normals[i].Y;
			normalarray[i*3+2] = normals[i].Z;
		}

		// initialize the VBOs:
		// create the buffer
		glGenBuffers(1, &vertVboId);
		glGenBuffers(1, &normVboId);
		// bind GL to it
		glBindBuffer(GL_ARRAY_BUFFER, vertVboId);
		// actually copy the data over
		glBufferData(GL_ARRAY_BUFFER, numcoords * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);
		// repeat for normals:
		glBindBuffer(GL_ARRAY_BUFFER, normVboId);
		glBufferData(GL_ARRAY_BUFFER, numcoords * sizeof(GLfloat), normalarray, GL_STATIC_DRAW);
		// clear the binding (ugh, this was hard fought)
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		createdVBOs = true;
	}
	
	// rebind to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertVboId);
	// we're using these as position data
	glEnableVertexAttribArray(0);
	// tell GL what this buffer is
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// rinse and repeat for the normal data
	glBindBuffer(GL_ARRAY_BUFFER, normVboId);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

Face::~Face(void)
{
	// give back the vram
	glDeleteBuffers(1, &vertVboId);
	glDeleteBuffers(1, &normVboId);
}
