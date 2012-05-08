#include "StdAfx.hpp"
#include "CrateAnimator.hpp"

#include "CgSingleton.hpp"
#include "SceneNode.hpp"

CrateAnimator::CrateAnimator(SceneRenderer* sr)
{
	this->sr = shared_ptr<SceneRenderer>(sr);
}

void CrateAnimator::createCrate(shared_ptr<Model> model,shared_ptr<Texture> texture,
								Vector3 location, float rotationspeed)
{
	auto tmp = make_shared<Crate>(model, rotationspeed);
	
	// create the material and set properties
	auto mat = make_shared<Material>();
    mat->textures.push_back(texture);
	mat->setShaderSet(
			CgSingleton::getSingleton().shaderSetMap["deferredTexture"]);
	model->setMaterial(mat);

	auto sn = make_shared<SceneNode>(nullptr, location);
    sn->addRenderable(tmp);
	sr->getSceneNodes().push_back(sn);
	
	// animator reference to crate
	crates.push_back(tmp);	
}

//! update the position of all car nodes based on dt, the time in milliseconds
//! since the last time animate was called.
void CrateAnimator::animate(double dt)
{
	// loop through all the crates, calling update rotation
	for(auto iter = crates.begin(); iter != crates.end(); iter++)
	{
		(*iter)->updateRotation(dt);
	}
}

CrateAnimator::~CrateAnimator(void)
{
}
