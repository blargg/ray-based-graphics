#include "raytracer.h"

void Raytracer::clear_scene()
{
	clear_objects();
	clear_lights();
}

void Raytracer::clear_objects()
{
	while(!objList.empty())
	{
		delete objList[0];
		objList.pop_back();
	}
}

void Raytracer::clear_lights()
{
	lightList.clear();
}

// ********************* Main Rendering function ****************************
Color Raytracer::getColor(const ray<3>& viewRay, int depth)
{
	// index of the closest object in objList
	int closestObj = -1;
	// Time for the ray to reach the closest intersection (since the viewRay should be unit lenght, this should be the distance)
	double bestTime = -1.0;
	// Evaluates what the closest intersection for the ray, then
	// sets closestObject and bestTime to their appropriate values.
	getClosestObject(viewRay, closestObj, bestTime);

	// no object was intersected and the ray has left the scene. return this color.
	if(closestObj < 0)
		return DEBUG_COLOR; //TODO eventually change this to something more meaningful

	// Initialize the return value to black (no light).
	Color retColor(0.0, 0.0, 0.0);
	Properties objProp = objList[closestObj]->getProperties( viewRay(bestTime) );
	// named unit_normal to signify that it is a unit vector (otherwise i keep forgetting)
	vectre<3> unit_normal = objList[closetObj]->normal_vectre( viewRay(bestTime), point<3>() ).unit_vectre();

	// For each light in the scene
	for(unsigned int i = 0; i < lightList.size(); ++i)
	{
	}


	return Color(0.0,0.0,0.0);
}


//********************** Private Helper Functions **************************
void Raytracer::getClosestObject(const ray<3>& viewRay, int& closestObj, double& bestTime)
{
	for(unsigned int i = 0; i < objList.size(); ++i)
	{
		if( objList[i]->intersects(viewRay) )
		{
			double currentTime = objList[i]->intersection(viewRay);
			if(bestTime < 0.0 || currentTime < bestTime)
			{
				// Store the time of intersection and the object intersected. 
				bestTime = currentTime; 
				closestObj = i;
			}
		}
	}
}

//*********************** Static Constant Vairables **********************
const Color Raytracer::DEBUG_COLOR = Color(1.0, 0.0, 0.4);
