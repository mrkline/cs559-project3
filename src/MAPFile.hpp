#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Material.hpp"
#include "Model.hpp"
#include "OBJFile.hpp"
#include "SceneNode.hpp"
#include "Texture.hpp"
#include "Vector3.hpp"

using namespace std;

static const string TEXTUREPATH = "./resources/textures/";
static const string MODELPATH = "./resources/models/";

//! Parses a MAPFile, which is a whitespace delimited list of textures and
//! models, as well as tiles, which are made up of the other two. Tiles have
//! a location, size, and rotation. 
class MAPFile
{
public:
	//! Parses the provided file for texture and model files, and turns the tile
	//! lines into SceneNodes
	MAPFile(char* filename);
	
	//! return the collection of SceneNodes
	vector<shared_ptr<SceneNode>>& getNodes() { return nodes; }
	
private:
	vector<shared_ptr<SceneNode>> nodes;
};

