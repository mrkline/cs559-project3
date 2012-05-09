#include "StdAfx.hpp"
#include "ArticulatedCrane.hpp"
#include "CgSingleton.hpp"


ArticulatedCrane::ArticulatedCrane(SceneRenderer* sr, Vector3 location)
{
	// set the variables for movement
	dtheta = 5.0f;
	dx = 0.5f;
	dy = 0.5f;
	basevector = location;
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
	//loadvector = Vector3(-3.0f, 9.5f, 0.0f);
	loadvector = Vector3(1.0f, 1.0f, 1.0f);
	loadsn = make_shared<SceneNode>(loadvector);
	loadsn->setParent(basesn);
	loadsn->addRenderable(loadmodel);
	// add to the sr tree
	sr->getSceneNodes().push_back(basesn);
}

void ArticulatedCrane::animate(double dt)
{
	this->rotateArm();
}

void ArticulatedCrane::rotateArm()
{
	armangle += dtheta;
	while(armangle > 360)
		armangle -= 360;
	
	auto& xlat = loadsn->getTransform();
	xlat.translate(-loadvector);
	xlat.rotateDegrees(Vector3(0, dtheta, 0));
	xlat.translate(loadvector);
}

void ArticulatedCrane::extendLoad()
{

}

void ArticulatedCrane::retractLoad()
{

}

void ArticulatedCrane::raiseLoad()
{

}

void ArticulatedCrane::lowerLoad()
{

}
