#include "raytracer.h"

void Raytracer::clear_scene()
{
	clear_objects();
	clear_lights();
}

void Raytracer::clear_objects()
{
	for(unsigned int i = 0; i < objList.size(); ++i)
		delete objList[i];
	objList.clear();
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
		return Color(0,0,0); /// \todo eventually change this to something more meaningful

	// Initialize the return value to black (no light).
	Color retColor(0.0, 0.0, 0.0);
	Properties objProp = objList[closestObj]->getProperties( viewRay(bestTime) );
	// named unit_normal to signify that it is a unit vector (otherwise i keep forgetting)
	vectre<3> unit_normal = objList[closestObj]->normal_vectre( viewRay(bestTime), point<3>() ).unit_vectre();

	// For each light in the scene
	for(unsigned int i = 0; i < lightList.size(); ++i)
	{
		vectre<3> lightDir(viewRay(bestTime), lightList[i].location);

		// skip if the normal points completely away from the object
		if(unit_normal.dot_prod(lightDir) <= 0.0)
			continue;
		/// @todo store length_sq() to speed getting the unit vectre later
		// skip if the light is on the point of intersection
		if(lightDir.length_sq() <= 0.0) /// \todo make this Episilon?
			continue;

		lightDir = lightDir.unit_vectre();
		/// \todo store viewRay(bestTime) to prevent recalculation.
		// Test if any objects are between the intersection and the light.
		ray<3> lightRay(viewRay(bestTime), lightDir);
		bool inShadow = false;
		for(unsigned int j = 0; j < objList.size(); ++j)
		{
			if(objList[j]->intersects(lightRay))
			{
				inShadow = true;
				break;
			}
		}
		
		// If not in the shadow of another object, to the lighting
		if(!inShadow)
		{
			/*
			 * labert shading
			 * multiply the light RGB by the color's RGB and the nby the cosine of the 
			 * angle between the surface normal vector and the vector that points to 
			 * the light.
			 *
			 */
			/// \todo Multiply Labmbert by the inverse square of the distance to the light.
			double lambert = (lightRay.dir.dot_prod(unit_normal));
			retColor += lambert * lightList[i].color * objProp.color;
		}
	}

	return retColor;


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
