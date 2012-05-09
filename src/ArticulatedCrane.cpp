#include "StdAfx.hpp"
#include "ArticulatedCrane.hpp"
#include "CgSingleton.hpp"


ArticulatedCrane::ArticulatedCrane(SceneRenderer* sr, Vector3 location)
{
	// set the variables for movement
	dtheta = 5.0f;
	dx = 0.5f;
	dy = 0.5f;
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
	basesn = make_shared<SceneNode>(nullptr, location);
	basesn->addRenderable(basemodel);
	// create the arm with an offset from the base
	armsn = make_shared<SceneNode>(nullptr, Vector3(0.0f, 0.0f, 0.0f));
	armsn->addRenderable(armmodel);
	basesn->addChild(armsn);
	// create the load with an offset from the base
	loadsn = make_shared<SceneNode>(nullptr, Vector3(0.0f, 0.0f, 0.0f));
	loadsn->addRenderable(loadmodel);
	basesn->addChild(loadsn);
	// add to the sr tree
	sr->getSceneNodes().push_back(basesn);
	sr->getSceneNodes().push_back(armsn);
	sr->getSceneNodes().push_back(loadsn);

}

void ArticulatedCrane::render()
{
	basemodel->render();
	armmodel->render();
	loadmodel->render();
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
	
	Vector3 zerov = Vector3(20.0f, 0.0f, 20.0f);
	/*auto armxlat = armsn->getTransform().getTranslation();
	armsn->getTransform().setTranslation(-zerov);
	armsn->getTransform().rotateDegrees(Vector3(0.0f, dtheta, 0.0f));
	armsn->getTransform().setTranslation(armxlat);*/
	
	auto loadxlat = armsn->getTransform().getTranslation();
	loadsn->getTransform().setTranslation(-zerov);
	loadsn->getTransform().rotateDegrees(Vector3(0.0f, dtheta, 0.0f));
	loadsn->getTransform().setTranslation(loadxlat);
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


ArticulatedCrane::~ArticulatedCrane(void)
{
}
