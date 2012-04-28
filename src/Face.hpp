#pragma once

#include "Vector3.hpp"
#include "Renderable.hpp"
#include <vector>

class Face : public Renderable
{

private:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> texturecoords;
	GLfloat * vertexarray;
	GLfloat * normalarray;
	GLfloat * texcoordarray;
	GLuint vertVboId;
	GLuint normVboId;
	GLuint texcoordVboId;
	bool createdVBOs;
	int numcoords;

public:
	Face(void);
	void render();
	const GLvoid* getVertexArray();
	const GLvoid* getNormalArray();
	const GLvoid* getTexCoordArray();
	void addVertex(Vector3 vertex){vertices.push_back(vertex);};
	void addNormal(Vector3 normal){normals.push_back(normal);};
	void addTexCoord(Vector2 texcoord){texturecoords.push_back(texcoord);};
	~Face(void);

};

