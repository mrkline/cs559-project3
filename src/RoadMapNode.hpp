#pragma once
#include "Vector2.hpp"
#include <vector>

using namespace std;

//! Represents a piece of road on a map and holds the links to connected nodes.
class RoadMapNode
{
private:
	Vector2 location;
	vector<shared_ptr<RoadMapNode>> connections;

public:
	RoadMapNode(Vector2 location){this->location = location;};
	Vector2 getLocation(){return this->location;};
	void addConnection(shared_ptr<RoadMapNode> conn);
	shared_ptr<RoadMapNode> getConnection(size_t idx);
	size_t getNumConnections();
	~RoadMapNode(void);
};

