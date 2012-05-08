#pragma once

#include <map>
#include "Material.hpp"
#include "Vector3.hpp"
#include "Model.hpp"

#include <memory>

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
	const std::shared_ptr<Model> getModel() { return model; }

private:
	std::shared_ptr<Model> model;
};

