#include "StdAfx.hpp"
#include "RoadMap.hpp"
#include <memory>
#include <sstream>
#include "Vector2.hpp"


RoadMap::RoadMap(const char* filename)
{
	if(filename != nullptr)
	{
		nodes = make_shared<vector<shared_ptr<RoadMapNode>>>();
		ifstream mapfile;
		mapfile.open(filename);
		if(mapfile.fail())
		{
			// nodes stays empty...
		}
		else
		{
			string key;
			float tmpx, tmpy;
			int idx;	
			while(!mapfile.eof() && mapfile >> key)
			{
				if(key == "#" )
				{
					// ignoring comments and smooth shading and groups
					continue;
				}
				else if(key == "n")
				{
					// node index and location
					mapfile >> idx >> tmpx >> tmpy;
					nodes->push_back(make_shared<RoadMapNode>(Vector2(tmpx, tmpy)));
				}
				else if(key == "e")
				{
					// node index and location
					string buff;
					getline(mapfile, buff);
					stringstream line(buff);
					int conns[4];
					line >> idx >> conns[0];
					int numconns = 1;
					// collect the indicies of all the connections
					while(line.good())
					{
						line >> conns[numconns];
						numconns++;
					}
					// add the connections to this node
					for(int i = 0; i < numconns; i++)
					{
						(*nodes)[idx]->addConnection(
							shared_ptr<RoadMapNode>((*nodes)[conns[i]]));
					}
				}
				else
				{
					// unknown or unhandled keyword
					continue;
				}
			}
			mapfile.close();
		}
	}
}

const shared_ptr<RoadMapNode>& RoadMap::getNodeAt(size_t loc)
{
	if(loc < nodes->size())
		return (*nodes)[loc];
	else
		return nullptr;
}
