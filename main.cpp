#include <vector>
#include <iostream>
#include <string>
#include <getopt.h>
#include <stdlib.h>

#include "easypng.h"

#include "raytracer.h"
#include "pathtracer.h"
#include "film.h"
#include "camera.h"
#include "drawable.h"
#include "obj_loader.h"

#include "simpleObject.h"
#include "shapes/sphere.h"
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

int main(int argc, char **argv)
{
    int optChar;
    int numSamples = 10;
    string outputFileName = "dump/out.png";

    enum {
        raytrace, path_trace, progressive_p
    } render_algorithm;
    render_algorithm = progressive_p;

    static struct option long_options[] = {
        {"raytrace",     no_argument,        0,  'r'},
        {"pathtrace",    required_argument,  0,  'p'},
        {"progressive",  required_argument,  0,  'g'},
        {"output",       required_argument,  0,  'o'}
    };
    while(1) {
        int option_index = 0;
        optChar = getopt_long(argc, argv, "rp:g:o:",
                long_options, &option_index);
        if(optChar == -1)
            break;
        switch(optChar) {
            case 'r':
                render_algorithm = raytrace;
                break;

            case 'p':
                render_algorithm = path_trace;
                numSamples = atoi(optarg);
                break;

            case 'g':
                render_algorithm = progressive_p;
                numSamples = atoi(optarg);
                break;

            case 'o':
                outputFileName = optarg;
                break;

            case '?':
                printf("unrecognized option\n");
                break;
            default:
                printf("getopt returned character code 0%o\n", optChar);
        }
    }

    ObjLoader loader;

    Camera cam;
    cam.imgWidth = 500;
    cam.imgHeight = 500;
    cam.position.dir = Vector4d(-1,0,0,0);
    cam.position.orig = Vector4d(10,0,0,1);
    cam.up = Vector4d(0,1,0,0);
    cam.worldWidth = 1.0;
    cam.worldHeight = 1.0;

    vector<Drawable *> allObj;

    if(optind < argc) {
        while (optind < argc) {
            loader.load_to_list(allObj, argv[optind++]);
        }
    }

    if(render_algorithm == raytrace) {
        Raytracer rayTracer;
        rayTracer.objTree.rebuildTree(allObj);
        PNG *pic = renderImage(rayTracer, cam);
        pic->writeToFile(outputFileName);
        delete pic;
    }

    if(render_algorithm == path_trace) {
        PathTracer pathTracer;
        pathTracer.setObjects(allObj);
        Film myFilm(cam.imgWidth,cam.imgHeight);
        pathtraceImage(&myFilm, pathTracer, cam, numSamples);
        PNG pic = myFilm.writeImage();
        pic.writeToFile(outputFileName);
    }

    if(render_algorithm == progressive_p) {
        PathTracer pathTracer;
        pathTracer.setObjects(allObj);
        progressiveRender(outputFileName, pathTracer, cam, numSamples);
    }

    for(unsigned int i = 0; i < allObj.size(); i++) {
        delete allObj[i];
    }

}
