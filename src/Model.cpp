#include "StdAfx.hpp"
#include "Model.hpp"

#include "Transform.hpp"

/* 
collection of vertices, normals, faces
handle to a material (which has a handle to a texture)
*/

Model::Model()
{
}

Vector3 Model::getVertex(int vertidx)
{
	if(vertidx < (int) vertices.size()) 
		return vertices[vertidx];
	else
		return Vector3(0,0,0);
}

Vector3 Model::getNormal(int normidx)
{
	if(normidx < (int) normals.size()) 
		return normals[normidx];
	else
		return Vector3(0,0,0);
}

void Model::render()
{
	for(auto iter = faces.begin(); iter != faces.end(); iter++)
	{
		iter->render();
	}

}

Model::~Model(void)
{
}

