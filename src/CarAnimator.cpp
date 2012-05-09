#include "StdAfx.hpp"
#include "CarAnimator.hpp"

#include <ctime>	// for srand

#include "CgSingleton.hpp"
#include "SceneNode.hpp"

CarAnimator::CarAnimator(shared_ptr<RoadMap> map, SceneRenderer* sr)
{
	this->map = map;
	this->sr = shared_ptr<SceneRenderer>(sr);
	srand((unsigned int)time(NULL));
}

void CarAnimator::createCar(shared_ptr<Model> model,shared_ptr<Texture> texture)
{
	const float CARHEIGHT = 5.0f;	// keep the cars off the ground just a tad
	const float CARSPEED = 10.0f;	// default speed in units per second
	
	auto tmp = make_shared<Car>(model, CARSPEED);
	
	// create the material and set properties
	auto mat = make_shared<Material>();
    mat->textures.push_back(texture);
	mat->setShaderSet(
			CgSingleton::getSingleton().shaderSetMap["deferredTexture"]);
	model->setMaterial(mat);

	// get a roadmapnode to set location and destination
	pair<shared_ptr<RoadMapNode>, shared_ptr<RoadMapNode>> mappoints =
		getRandomLocationAndDestination();	
	tmp->setOrigin(mappoints.first);
	tmp->setDestination(mappoints.second);
	auto loc = mappoints.first->getLocation();
	// add the scenenode
	auto sn = make_shared<SceneNode>(Vector3(loc.X, CARHEIGHT, loc.Y));
    sn->addRenderable(tmp);
	sr->getSceneNodes().push_back(sn);

	tmp->updateRotation();
	// car animator reference to car
	cars.push_back(tmp);
	
}

pair<shared_ptr<RoadMapNode>, shared_ptr<RoadMapNode>> 
		CarAnimator::getRandomLocationAndDestination()
{
	srand((unsigned int)time(NULL));

	auto rtn = pair<shared_ptr<RoadMapNode>, shared_ptr<RoadMapNode>>
		(nullptr, nullptr);
	
	// Get a random node as the location
	auto mapsize = map->getSize();
	if(mapsize > 0)
	{
		auto locidx = rand() % mapsize;
		rtn.first = map->getNodeAt(locidx);
	}

	// get a random connection as the destination
	if(rtn.first)
	{
		auto connsize = rtn.first->getNumConnections();
		if(connsize > 0)
		{
			auto connidx = rand() % connsize;
			rtn.second = rtn.first->getConnection(connidx);
		}
	}
	return rtn ;
}

//! update the position of all car nodes based on dt, the time in milliseconds
//! since the last time animate was called.
void CarAnimator::animate(double dt)
{
	const float PROXTOLER = 1;	// how close the destination the car should
									// be before picking a new destination
	// loop through all the cars
	for(auto iter = cars.begin(); iter != cars.end(); iter++)
	{
		auto car = iter->get();	// ha! auto, car, yeah, it's late...

		// get info about this car
		auto speed = car->getSpeed();
		auto curloc = car->getLocation();
		auto destLoc = car->getDestination()->getLocation();
		auto dest = Vector3(destLoc.X, curloc.Y, destLoc.Y);
		
		// check if we are within tolerance of the destination point and should
		// update destinations
		if((curloc - dest).getLength() <= PROXTOLER)
		{
			// we're at the destination, so move the destination to the origin
			// and randomly pick one of the (new) origin's connections as the
			// destination
			auto oldorigin = car->getOrigin();
			car->setOrigin(car->getDestination());
			do
			{
				auto idx = rand() % car->getOrigin()->getNumConnections();
				car->setDestination(car->getOrigin()->getConnection(idx));
			}
			while(car->getOrigin()->getNumConnections() > 1 && car->getDestination() == oldorigin);
			destLoc = car->getDestination()->getLocation();
			// update dest for the next step
			dest = Vector3(destLoc.X, curloc.Y, destLoc.Y);
			// update the rotation before moving, otherwise there can
			// be a backward-looking step
			car->updateRotation();
		}
		// move the car some distance towards the destination
		double totaldistance = (speed / 1000.0) * dt; // yada yada, windows 49day bug
		totaldistance = min(speed, (float)totaldistance); // clamp the distance
		auto movevector = dest - curloc;

		auto mvleng = movevector.getLength();
		
		auto delta = movevector / mvleng * (float)totaldistance;
		
		// actually move the car
		car->setLocation(curloc + delta);

		// tell the car to fix its rotation
		car->updateRotation();
	}
}

CarAnimator::~CarAnimator(void)
{
}
