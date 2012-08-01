#include <vector>
#include <iostream>

#include "easypng.h"

#include "raytracer.h"
#include "drawable.h"
#include "simpleObject.h"
#include "sphere.h"
#include "material.h"
#include "solidColor.h"
#include "light.h"
#include "structs.h"
#include "ray.h"

using std::vector;

/// Hard coded test list of objects for the renderer.
void makeObjList(vector<Drawable*>& objList)
{
	//OBJECT 1
	Sphere s(point<3>(100.5, 100.1, 300), 100);
	Properties p;
	p.color = Color(0.0, 0.0, 0.6);
	SolidColor c(p);
	SimpleObject* ptr = new SimpleObject(s,c);
	objList.push_back(ptr);
}

/// List of lights for the renderer.
void makeLights(vector<Light>& lightList)
{
	Light temp;
	temp.location = point<3>(0.0, 0.0, 0.0);
	lightList.push_back(temp);
}

int main()
{
	Raytracer renderer;
	makeObjList(renderer.objList);
	makeLights(renderer.lightList);

	//TODO This is a basic test.
	PNG pic(400,400);
	for(int i = 0; i < 400; ++i)
		for(int j = 0; j < 400; ++j)
		{
			ray<3> viewRay;
			viewRay.orig = point<3>(i, j, 0);
			viewRay.dir = vectre<3>(0,0,1);
			Color c = renderer.getColor(viewRay, 0);
			*pic(i,j) = RGBAPixel( c.red * 255,
					               c.green * 255,
								   c.blue * 255);
		}
	pic.writeToFile("output.png");
	return 0;
}
