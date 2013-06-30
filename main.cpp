#include <vector>
#include <iostream>
#include <string>

#include "easypng.h"

#include "raytracer.h"
#include "film.h"
#include "camera.h"
#include "drawable.h"
#include "obj_loader.h"

#include "simpleObject.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/triangle.h"
#include "shapes/perturb_normals.h"

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
using std::string;
using std::min;
using std::max;
using namespace std;

int main()
{
    Raytracer renderer;
    ObjLoader loader;
    loader.load_to_list(renderer.objList, "room.obj");

    //Properties p;
    //p.color = Color(0,0,0);
    //p.emittance = Color(1,1,1);
    //Sphere lightShape(Vector4d(-20,10,0, 1), 5);
    //renderer.objList.push_back(new SimpleObject(lightShape, SolidColor(p)));

    //Sphere s(Vector4d(0,0,0,1), 10);
    //p.color = Color(1,0.5,0.5);
    //p.specular = Color(0.5,0.5,0.5);
    //p.emittance = Color(0,0,0);
    //renderer.objList.push_back(new SimpleObject(PerturbNormals(s,M_PI/6.0), SolidColor(p)));

    Camera cam;
    cam.imgSize = 500;
    cam.position.dir = Vector4d(-1,0,0,0);
    cam.position.orig = Vector4d(10,0,0,1);
    cam.up = Vector4d(0,1,0,0);
    cam.worldSize = 1.0;

    //PNG *pic = renderImage(renderer, cam);
    //pic->writeToFile("output.png");
    //delete pic;

    //Film myFilm(cam.imgSize,cam.imgSize);
    //pathtraceImage(&myFilm, renderer, cam, 50);
    //PNG pic = myFilm.writeImage();
    //pic.writeToFile("output.png");

    progressiveRender("dump/out", renderer, cam, 10);

    renderer.clear_objects();
    return 0;
}
