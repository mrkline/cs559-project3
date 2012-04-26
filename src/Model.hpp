#pragma once

#include <vector>
#include "Material.hpp"
#include "Vector3.hpp"
#include "Renderable.hpp"
#include "Face.hpp"
#include <string>

//! An object created from an OBJ file (likely exported from Blender) that 
//	knows how to draw itself.
class Model: public Renderable
{
private:
	std::string objName;
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Face> faces;
	Material* objMaterial;

public:
	Model();
	void render();
	void setName(std::string name){objName = name;};
	std::string getName(){return this->objName;};
	void addVertex(Vector3 vertex){vertices.push_back(vertex);};
	void addNormal(Vector3 normal){normals.push_back(normal);};
	void addFace(Face face){faces.push_back(face);};
	Vector3 getVertex(int vertidx);
	Vector3 getNormal(int normidx);
	void setMaterial(Material& material){*objMaterial = material;};
	~Model(void);


};

