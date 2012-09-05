#include <vector>
#include <iostream>

#include "easypng.h"

#include "raytracer.h"
#include "drawable.h"

#include "simpleObject.h"
#include "sphere.h"
#include "plane.h"

#include "material.h"
#include "solidColor.h"
#include "turbulent.h"

#include "light.h"
#include "properties.h"
#include "ray.h"

using std::vector;
using std::min;
using std::max;
using namespace std;

/// Hard coded test list of objects for the renderer.
void makeObjList(vector<Drawable*>& objList)
{
	//OBJECT 1
	Sphere s(point<3>(100.5, 100.1, 300), 30);
	Properties p;
	p.color = Color(0.85, 0.85, 0.85);
	p.reflect = 0.0;
	p.specular = Color(0.0,0.0,0.0);
	p.spec_power = 0.0;
	SolidColor c(p);
	SimpleObject* ptr = new SimpleObject(s,c);
	objList.push_back(ptr);

	//OBJECT 2
	Sphere s2(point<3>(300, 200, 300), 100);
	Properties blend1;
		blend1.color = Color(0.0, 0.0, 0.9);
		blend1.reflect = 0.3;
	
	Properties blend2;
		blend2.color = Color(0.0, 0.7, 0.0);
		blend2.specular = Color(0.0, 0.0, 0.0);
	Turbulent turbTex(blend1, blend2);
	objList.push_back(new SimpleObject(s2, turbTex));

	//OBJECT 3 (REMOVED FROM SCENE)
	Plane pl1(point<3>(100,100,500), vectre<3>(0,0,1));
	p.color = Color(0, 0.7, 0);
	//objList.push_back(new SimpleObject(pl1, SolidColor(p)));
}

/// List of lights for the renderer.
void makeLights(vector<Light>& lightList)
{
	Light temp;
	temp.color = Color(0.9, 0.9, 0.9);
	temp.location = point<3>(75, 75, 75);
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
			*pic(i,j) = RGBAPixel( max(min( (int) (c.red * 255), 255), 0),
					               max(min( (int) (c.green * 255), 255), 0),
								   max(min( (int) (c.blue * 255), 255), 0) );
		}
	pic.writeToFile("output.png");
	// Clean up memory.
	renderer.clear_objects();
	return 0;
}
