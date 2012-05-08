#pragma once

#include <vector>

#include "RoadMapNode.hpp"

//! represents the intersections of a road map in a collection of RoadMapNodes.
//! the collection is built up from the file passed in to constructor.
class RoadMap
{
public:
	RoadMap(const char* filename);
	size_t getSize(){return this->nodes->size();};
	shared_ptr<RoadMapNode> getNodeAt(size_t loc);

private:
	shared_ptr<vector<shared_ptr<RoadMapNode>>> nodes;
};


