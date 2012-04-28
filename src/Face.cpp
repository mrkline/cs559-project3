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
		texcoordarray = new GLfloat[2 * texturecoords.size()];
		for(int i = 0; i < vertices.size(); i++)
		{
			vertexarray[i*3] = vertices[i].X;
			vertexarray[i*3+1] = vertices[i].Y;
			vertexarray[i*3+2] = vertices[i].Z;
			normalarray[i*3] = normals[i].X;
			normalarray[i*3+1] = normals[i].Y;
			normalarray[i*3+2] = normals[i].Z;
		}
		for(int i = 0; i < texturecoords.size(); i++)
		{
			texcoordarray[i*2] = texturecoords[i].X;
			texcoordarray[i*2+1] = texturecoords[i].Y;
		}

		// initialize the VBOs:
		// create the buffer
		glGenBuffers(1, &vertVboId);
		glGenBuffers(1, &normVboId);
		glGenBuffers(1, &texcoordVboId);
		// bind GL to it
		glBindBuffer(GL_ARRAY_BUFFER, vertVboId);
		// actually copy the data over
		glBufferData(GL_ARRAY_BUFFER, numcoords * sizeof(GLfloat), vertexarray, 
			GL_STATIC_DRAW);
		// repeat for normals and texture coordinates:
		glBindBuffer(GL_ARRAY_BUFFER, normVboId);
		glBufferData(GL_ARRAY_BUFFER, numcoords * sizeof(GLfloat), normalarray, 
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordVboId);
		glBufferData(GL_ARRAY_BUFFER, texturecoords.size() * 2 * sizeof(GLfloat),
			texcoordarray, GL_STATIC_DRAW);
		// clear the binding (ugh, this was hard fought)
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		createdVBOs = true;
	}
			
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// rebind to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertVboId);	
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// rinse and repeat for the normal  and texture coord data
	glBindBuffer(GL_ARRAY_BUFFER, normVboId);	
	glNormalPointer(GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, texcoordVboId);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);


	// draw the polygon
	if(vertices.size() == 3)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	else
	{
		glDrawArrays(GL_QUADS, 0, vertices.size());
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

Face::~Face(void)
{
	// give back the vram
	glDeleteBuffers(1, &vertVboId);
	glDeleteBuffers(1, &normVboId);
	glDeleteBuffers(1, &texcoordVboId);
}
