#include "StdAfx.hpp"
#include "ArticulatedCrane.hpp"
#include "CgSingleton.hpp"


ArticulatedCrane::ArticulatedCrane(SceneRenderer* sr, Vector3 location)
{
	// set the variables for movement
	dtheta = 2.0f;
	dx = 0.2f;
	dy = 0.2f;
	outmax = 12;
	downmax = 8;
	basevector = location;
	loaddistance = loadheight = 0;
	loaddirout = loaddirdown = true;
	// get the renderable models
	basemodel = OBJFile("./resources/models/crane_base.obj").getModel();
	armmodel = OBJFile("./resources/models/crane_arm.obj").getModel();
	loadmodel = OBJFile("./resources/models/crane_load.obj").getModel();
	// create the material and set properties
	auto mat = make_shared<Material>();
    mat->setShaderSet(
			CgSingleton::getSingleton().shaderSetMap["deferredTexture"]);
	basemodel->setMaterial(mat);
	armmodel->setMaterial(mat);
	loadmodel->setMaterial(mat);
	// create the base at the base location
	basesn = make_shared<SceneNode>(basevector);
	basesn->addRenderable(basemodel);
	// create the arm with an offset from the base
	armvector = Vector3(5.0f, 12.0f, 0.0f);
	armsn = make_shared<SceneNode>(armvector);
	armsn->setParent(basesn);
	armsn->addRenderable(armmodel);
	// create the load with an offset from the base
	loadvector = Vector3(-10.0f, 9.5f, 0.0f);
	loadsn = make_shared<SceneNode>(loadvector);
	loadsn->setParent(basesn);
	loadsn->addRenderable(loadmodel);
	// add to the sr tree
	sr->getSceneNodes().push_back(basesn);
}

void ArticulatedCrane::animate(double dt)
{
	
	if(loaddirdown)
		lowerLoad();
	else
		raiseLoad();
	/*if(loaddirout)
		extendLoad();
	else
		retractLoad();*/
	this->rotateArm();
}

void ArticulatedCrane::rotateArm()
{
	auto& loadxfrm = loadsn->getTransform();
	loadxfrm.translate(-loadvector);
	loadxfrm.rotateDegrees(Vector3(0, dtheta, 0));
	loadxfrm.translate(loadvector);

	auto& armxfrm = armsn->getTransform();
	armxfrm.translate(-armvector);
	armxfrm.rotateDegrees(Vector3(0, dtheta, 0));
	armxfrm.translate(armvector);
}

void ArticulatedCrane::extendLoad()
{
	if(loaddistance + dx < outmax)
	{
		loaddistance += dx;
		loadsn->getTransform().translate(Vector3(-dx, 0, 0));
	}
	else
		loaddirout = false;
}

void ArticulatedCrane::retractLoad()
{
	if(loaddistance - dx  > 0)
	{
		loaddistance -= dx;
		loadsn->getTransform().translate(Vector3(dx, 0, 0));
	}
	else
		loaddirout = true;
}

void ArticulatedCrane::raiseLoad()
{
	if(loadheight - dy  > 0)
	{
		loadheight -= dy;
		loadsn->getTransform().translate(Vector3(0, dy, 0));
	}
	else
		loaddirdown = true;
}

void ArticulatedCrane::lowerLoad()
{
	if(loadheight + dy < downmax)
	{
		loadheight += dx;
		loadsn->getTransform().translate(Vector3(0, -dy, 0));
	}
	else
		loaddirdown = false;
}
