#include "StdAfx.hpp"
#include "ArticulatedRadio.hpp"
#include "CgSingleton.hpp"


ArticulatedRadio::ArticulatedRadio(SceneRenderer* sr, Vector3 location)
{
	// set the variables for movement
	dtheta = 2.0f;
	mastangle = 0;
	basevector = location;
	radioforward = true;
	// get the renderable models
	basemodel = OBJFile("./resources/models/radio_base.obj").getModel();
	mastmodel = OBJFile("./resources/models/radio_mast.obj").getModel();
	dishmodel = OBJFile("./resources/models/radio_dish.obj").getModel();
	// create the material and set properties
	auto mat = make_shared<Material>();
    mat->setShaderSet(
			CgSingleton::getSingleton().shaderSetMap["deferredTexture"]);
	basemodel->setMaterial(mat);
	mastmodel->setMaterial(mat);
	dishmodel->setMaterial(mat);
	// create the base at the base location
	basesn = make_shared<SceneNode>(basevector);
	basesn->addRenderable(basemodel);
	// create the mast with some offset from the base
	mastvector = Vector3(0.0f, 0.0f, 0.0f);
	mastsn = make_shared<SceneNode>(mastvector);
	mastsn->setParent(basesn);
	mastsn->addRenderable(mastmodel);
	// create the dish with an offset from the mast
	dishvector = Vector3(0.0f, 3.5f, 0.0f);
	dishsn = make_shared<SceneNode>(dishvector);
	dishsn->setParent(mastsn);
	dishsn->addRenderable(dishmodel);
	// add to the sr tree
	sr->getSceneNodes().push_back(basesn);
}

void ArticulatedRadio::animate(double dt)
{
	this->rotateDish();
	this->waveMast();
}

void ArticulatedRadio::rotateDish()
{
	auto& dishxfrm = dishsn->getTransform();
	dishxfrm.translate(-dishvector);
	dishxfrm.rotateDegrees(Vector3(0, dtheta, 0));
	dishxfrm.translate(dishvector);
}

void ArticulatedRadio::waveMast()
{
	auto& mastxfrm = mastsn->getTransform();
	auto rot = mastxfrm.getRotationDegrees().X;
	if(mastangle < 45 && radioforward)
	{
		mastangle += dtheta;
		mastxfrm.rotateDegrees(Vector3(dtheta, 0, 0));
		if(mastangle >= 45)
			radioforward = false;
	}
	else if(mastangle > 0-45 && !radioforward)
	{
		mastangle -= dtheta;
		mastxfrm.rotateDegrees(Vector3(-dtheta, 0, 0));
		if(mastangle <= -45)
			radioforward = true;
	}
}
