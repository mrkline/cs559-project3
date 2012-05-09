#include "StdAfx.hpp"
#include "MAPFile.hpp"

#include "CgSingleton.hpp"
#include "ShaderSet.hpp"

MAPFile::MAPFile(char* filename)
{
	if(filename != nullptr)
	{
		vector<shared_ptr<Texture>> textures;
		vector<shared_ptr<Model>> models;
		ifstream matfile(filename);
		if(matfile.fail())
		{
			throw Exceptions::FileException("MAT file could not load",
			                                __FUNCTION__);
		}

		string key, temp;
		float tmpx, tmpy, tmpz;
		int tmpsize, tmprot, tmpmdl, tmptext;		
		while(!matfile.eof() && matfile >> key)
		{
			if(key == "#")
			{
				// ignoring comments and smooth shading and groups
				continue;
			}
			else if(key == "texture")
			{
				// texture index and file
				matfile >> temp >> temp; // throw away index
				string path = TEXTUREPATH + temp;
				textures.push_back(make_shared<Texture>(path.c_str()));

			}
			else if(key == "model")
			{
				// model index and file
				matfile >> temp >> temp; // throw away index
				string path = MODELPATH + temp;
				cout << "------------------------------" << endl;
				cout << "loading model: " << path << endl;
				OBJFile* objfile = new OBJFile(path.c_str());
				models.push_back(make_shared<Model>(*objfile->getModel()));
				cout << "done loading: " << path << endl;
				cout << "------------------------------" << endl;
			}
			else if(key == "tile")
			{
				// tile lines
				vector<int> tmptextures;
				string buff;
				getline(matfile, buff);
				stringstream line(buff);
				line >>  tmpx >> tmpy >> tmpz >> tmpsize >> tmprot >> tmpmdl;
				cout << "------------------------------" << endl;
				cout << "loading tile at x,z: " << tmpx << "," << tmpz << endl;
				while(line.good())
				{
					line >> tmptext;
					tmptextures.push_back(tmptext);
				}
				// make the material
				auto mat = make_shared<Material>();
				mat->setShaderSet(CgSingleton::getSingleton().
						shaderSetMap["deferredTexture"]);
				// get a reference to the model
				auto tile = make_shared<Model>(*models[tmpmdl]);
				// add the texture(s) to the material
				for(auto i = tmptextures.begin(); i != tmptextures.end(); i++)
				{	
					mat->textures.push_back(textures[*i]);

				}
				// set the material on the model
				tile->setMaterial(mat);
				// create the scene node, with the transform
				auto tilenode = make_shared<SceneNode>(Vector3(tmpx, tmpy, tmpz));
				tilenode->getTransform().rotateDegrees(Vector3(0, (float)tmprot, 0));
				// add the model to the scene node
				tilenode->addRenderable(tile);
				// store the scenenode
				try{
					nodes.push_back(tilenode);
				}
				catch(exception e)
				{
					cout << "error loading a node: " << e.what() << endl;
				}
				cout << "done loading tile" << endl;
				cout << "------------------------------" << endl;
			}
			else
			{
				// unknown or unhandled keyword
				continue;
			}
		}
	}
}
