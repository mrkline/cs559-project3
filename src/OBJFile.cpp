#include "StdAfx.hpp"
#include "OBJFile.hpp"
#include <sstream>

// for now, assumes a pretty valid obj file and only one object definition
// in the file. obviously this could use refining.

// TODO: texture coords, material library, material to use, face definition
// add face struct or class?

OBJFile::OBJFile(char* filename)
{
	if(filename != nullptr)
	{
		ifstream objfile;
		objfile.open(filename);
		if(objfile.fail())
		{
			model = nullptr;
		}
		else
		{
			model = new Model();
			string key, temp;
			float tmpx, tmpy, tmpz;
			while(!objfile.eof() && objfile >> key)
			{
				if(key == "#" || key == "s" || key == "g")
				{
					// ignoring comments and smooth shading and groups
					continue;
				}
				else if(key == "mtllib")
				{
					// material library file
				}
				else if(key == "o")
				{
					// object name
					objfile >> temp;
					model->setName(temp);
				}
				else if(key == "v")
				{
					// vertex
					objfile >> tmpx >> tmpy >> tmpz;
					model->addVertex(Vector3(tmpx, tmpy, tmpz));
				}
				else if(key == "vn")
				{
					// normals
					objfile >> tmpx >> tmpy >> tmpz;
					model->addNormal(Vector3(tmpx, tmpy, tmpz));
				}
				else if(key == "vt")
				{
					// texture coord
				}
				else if(key == "usemtl")
				{
					// material to use
				}
				else if(key == "f")
				{
					// face definition
					Face* face = new Face();
					string buff;
					getline(objfile, buff);
					stringstream line(buff);
					string verts[4];
					string v, vt, vn;
					int numverts = 3;

					line >> verts[0] >> verts[1]>> verts[2];
					if(line.good())
					{
						line >> verts[3];
						numverts = 4;
					}
					for(int i = 0; i < numverts; i++)
					{
						stringstream token(verts[i]);
						getline(token, v, '/');
						getline(token, vt, '/');
						token >> vn;
						face->addVertex(model->getVertex(atoi(v.c_str())));
						// need to add texture coord support
						face->addNormal(model->getNormal(atoi(vn.c_str())));
					}
					model->addFace(*face);

				}
				else
				{
					// unknown or unhandled keyword
					continue;
				}
			}
			
			objfile.close();
		}
	}
}

OBJFile::~OBJFile(void)
{

}
