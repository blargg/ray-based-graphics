#include <vector>
#include <iostream>

#include "easypng.h"

#include "raytracer.h"
#include "loader.h"
#include "camera.h"
#include "drawable.h"

#include "bumpy_sphere.h"

#include "simpleObject.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/triangle.h"

#include "material.h"
#include "materials/solidColor.h"
#include "materials/turbulent.h"
#include "materials/sphere_texture.h"

#include "AreaLight.h"
#include "bounding_shape.h"
#include "light.h"
#include "properties.h"
#include "ray.h"

using std::vector;
using std::min;
using std::max;
using namespace std;

int main()
{
    Raytracer renderer;
    renderer = loadScene("Scene.dat");
    renderer.boundries = vector<BoundingShape>();

    Properties p;
    p.color = Color(0,0,0);
    p.emittance = Color(100,100,100);
    Sphere lightShape(point<3>(-20,20,0), 5);
    //renderer.objList.push_back(new SimpleObject(lightShape, SolidColor(p)));

    Sphere s(point<3>(0,0,0), 10);
    p.color = Color(1,0.5,0.5);
    p.emittance = Color(0,0,0);
    renderer.objList.push_back(new SimpleObject(s, SolidColor(p)));
    //renderer.objList.push_back(new SimpleObject(s, SphereTexture("texture2.png")));

    ray<3> orientation;
    orientation.dir = vectre<3>(1,0,0).unit_vectre();
    orientation.orig = point<3>(-25,0,0);
    vectre<3> up(0,1,0);
    PNG *pic = renderImage(renderer, 500, 1.0, orientation, up);
    pic->writeToFile("output.png");
    delete pic;

    //PNG pic = pathtraceImage(renderer, 500, 1.0, orientation, up, 200);
    //pic.writeToFile("output.png");
    //renderer.clear_objects();
    return 0;
}
