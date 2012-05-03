#pragma once

#include <map>
#include "Material.hpp"
#include "Vector3.hpp"
#include "Model.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//! A class for parsing .obj files
class OBJFile
{
public:
	OBJFile(const char* filename);
	Model* getModel(){return model;};
	~OBJFile(void);

private:
	Model* model;
};

