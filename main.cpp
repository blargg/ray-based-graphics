#include <vector>
#include <iostream>

#include "easypng.h"

#include "raytracer.h"
#include "film.h"
#include "loader.h"
#include "camera.h"
#include "drawable.h"

#include "simpleObject.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/triangle.h"

#include "material.h"
#include "materials/solidColor.h"
#include "materials/turbulent.h"
#include "materials/sphere_texture.h"

#include "AreaLight.h"
#include "light.h"
#include "properties.h"
#include "ray.h"
#include "common.h"

using std::vector;
using std::min;
using std::max;
using namespace std;

int main()
{
    Raytracer renderer;
    renderer = loadScene("Scene.dat");

    Properties p;
    p.color = Color(0,0,0);
    p.emittance = Color(100,100,100);
    Sphere lightShape(Vector4d(-20,20,0, 1), 5);
    //renderer.objList.push_back(new SimpleObject(lightShape, SolidColor(p)));

    Sphere s(Vector4d(0,0,0,1), 10);
    p.color = Color(1,0.5,0.5);
    p.emittance = Color(0,0,0);
    renderer.objList.push_back(new SimpleObject(s, SolidColor(p)));
    //renderer.objList.push_back(new SimpleObject(s, SphereTexture("texture2.png")));

    Camera cam;
    cam.imgSize = 500;
    cam.position.dir = Vector4d(1,0,0,0);
    cam.position.orig = Vector4d(-25,0,0,1);
    cam.up = Vector4d(0,1,0,0);
    cam.worldSize = 1.0;

    PNG *pic = renderImage(renderer, cam);
    pic->writeToFile("output.png");
    delete pic;

    //Film myFilm(cam.imgSize,cam.imgSize);
    //pathtraceImage(&myFilm, renderer, cam, 50);
    //PNG pic = myFilm.writeImage();
    //pic.writeToFile("output.png");

    //progressiveRender("dump/out", renderer, cam, 50);

    renderer.clear_objects();
    return 0;
}
