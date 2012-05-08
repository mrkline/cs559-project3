#pragma once

#include <map>

#include "Material.hpp"
#include "Vector3.hpp"

//! A class for parsing .mtl files
class MTLFile
{
public:
	MTLFile(char* filename);
	std::map<char*, Material> getMaterials();
};

