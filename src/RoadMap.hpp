#pragma once
#include <vector>
#include "RoadMapNode.hpp"

//! represents the intersections of a road map in a collection of RoadMapNodes.
//! the collection is built up from the file passed in to constructor.
class RoadMap
{
private:
	shared_ptr<vector<shared_ptr<RoadMapNode>>> nodes;

public:
	RoadMap(char* filename);
	
	~RoadMap(void);
};


