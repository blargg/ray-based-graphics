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
	vectre<3> unit_normal = objList[closestObj]->normal_vectre( viewRay(bestTime)).unit_vectre();
	/// \todo transparency
	if( unit_normal.dot_prod(viewRay.dir) > 0.0 )
		unit_normal = unit_normal * -1.0;

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

			/*
			 * Blinn-Phong
			 * Specular lighting
			 *
			 * This technique is a quick way of getting glossy materials.
			 * This really isn't a perfect physical representation, but it
			 * looks really good.
			 */
			double viewProj = viewRay.dir.dot_prod(unit_normal);
			double lightProj = lightRay.dir.dot_prod(unit_normal);
			vectre<3> blinnDir = lightRay.dir - viewRay.dir;
			double temp = blinnDir.dot_prod(blinnDir);
			if(temp != 0.0)
			{
				double blinn = (1.0 / sqrt(temp)) * std::max(lightProj - viewProj, 0.0);
				blinn = pow(blinn, objProp.spec_power);
				retColor += blinn * objProp.specular * lightList[i].color;
			}
		}
	}

	/*
	 * if the object isn't reflective, or there have been
	 * too many rays cast, don't do anything.
	 */
	if(objProp.reflect > 0.0 && depth < 10)
	{
		ray<3> reflect_ray(viewRay(bestTime),
						   viewRay.dir - (2.0 * (viewRay.dir.dot_prod(unit_normal))) * unit_normal);
		Color reflect_color = getColor(reflect_ray, depth + 1);
		retColor += objProp.reflect * reflect_color;
//		retColor.r += objProp.reflect * reflect_color.r;
//		retColor.g += objProp.reflect * reflect_color.g;
//		retColor.b += objProp.reflect * reflect_color.b;
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
