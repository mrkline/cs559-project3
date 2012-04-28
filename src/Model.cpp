#include "StdAfx.hpp"
#include "Model.hpp"

#include "Transform.hpp"

/* 
collection of vertices, normals, faces
handle to a material (which has a handle to a texture)
*/

Model::Model()
{
	this->objMaterial = getDefaultMaterial();
}

Vector3 Model::getVertex(int vertidx)
{
	if(vertidx - 1 < (int) vertices.size()) 
		return vertices[vertidx - 1];
	else
		return Vector3(0,0,0);
}

Vector2 Model::getTexCoord(int texcoordidx)
{
	if(texcoordidx - 1 < (int) texcoords.size()) 
		return texcoords[texcoordidx - 1];
	else
		return Vector2(0,0);
}

Vector3 Model::getNormal(int normidx)
{
	if(normidx -1 < (int) normals.size()) 
		return normals[normidx - 1];
	else
		return Vector3(0,0,0);
}

void Model::render()
{	setActiveMaterial(objMaterial);
	
	for(auto iter = faces.begin(); iter != faces.end(); iter++)
	{
		iter->render();
	}

}

Model::~Model(void)
{
}

