#include "StdAfx.hpp"
#include "RoadMapNode.hpp"

void RoadMapNode::addConnection(shared_ptr<RoadMapNode> conn)
{
	connections.push_back(conn);
}

shared_ptr<RoadMapNode> RoadMapNode::getConnection(size_t idx)
{
	if(idx >= 0 && idx < connections.size())
	{
		return connections[idx];
	}
	else
	{
		return nullptr;
	}
}
size_t RoadMapNode::getNumConnections()
{
	return connections.size();
}

RoadMapNode::~RoadMapNode(void)
{
}
