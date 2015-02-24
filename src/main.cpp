#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "render/raytracer.h"
#include "render/pathtracer.h"
#include "render/camera.h"
#include "render/metropolis_light_transport.h"
#include "file_loader/obj_loader.h"
#include "file_loader/assimp_loader.h"
#include "file_loader/cam_loader.h"

#include "util/log.h"

using std::vector;
using std::string;
using std::min;
using std::max;


int main(int argc, char **argv) {
    // set default log level to debug and higher
    LOG_level = 1;

    int optChar;
    int numSamples = 10;
    string outputFileName = "dump/out.png";

    enum {
        raytrace, path_trace, progressive_p, metropolis
    } render_algorithm;
    render_algorithm = progressive_p;

    static struct option long_options[] = {
        {"raytrace",      no_argument,        0,  'r'},
        {"pathtrace",     required_argument,  0,  'p'},
        {"progressive",   required_argument,  0,  'g'},
        {"metropolis",    no_argument,        0,  'm'},
        {"output",        required_argument,  0,  'o'},
        {"log-level",     required_argument,  0,  0}
    };
    while (1) {
        int option_index = 0;
        optChar = getopt_long(argc, argv, "rp:g:o:",
                long_options, &option_index);
        if (optChar == -1)
            break;
        switch (optChar) {
            case 0:
                if (strcmp(long_options[option_index].name, "log-level") == 0)
                    LOG_level = atoi(optarg);
                break;

            case 'r':
                render_algorithm = raytrace;
                break;

            case 'p':
                render_algorithm = path_trace;
                numSamples = atoi(optarg);
                break;

            case 'm':
                render_algorithm = metropolis;
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
    int imgWidth = 500;
    int imgHeight = 500;

    vector<Drawable *> allObj;
    vector<Drawable *> lights;

    if (optind >= argc) {
        printf("No scene files selected. Exiting.\n");
        exit(1);
    }

    while (optind < argc) {
        string filename = string(argv[optind++]);
        const aiScene* sc = getScene(filename.c_str());
        assimp_append(sc, allObj, lights);

        string basename = filename.substr(0, filename.find_last_of('.'));
        cam = loadCameraFromFile(basename + ".cam");
    }

    if (render_algorithm == raytrace) {
        Raytracer rayTracer;
        rayTracer.objTree.rebuildTree(allObj);
        PNG *pic = renderImage(rayTracer, cam, imgWidth, imgHeight);
        pic->writeToFile(outputFileName);
        delete pic;
    }

    if (render_algorithm == path_trace) {
        PathTracer pathTracer;
        pathTracer.setObjects(allObj);
        Film myFilm(imgWidth, imgHeight);
        pathtraceImage(&myFilm, pathTracer, cam, numSamples);
        PNG pic = myFilm.writeImage();
        pic.writeToFile(outputFileName);
    }

    if (render_algorithm == progressive_p) {
        PathTracer pathTracer;
        pathTracer.setObjects(allObj);
        progressiveRender(outputFileName, pathTracer, cam, imgWidth, imgHeight, numSamples);
    }

    if (render_algorithm == metropolis) {
        MetropolisRenderer met;
        met.setCamera(cam);
        met.setObjects(allObj);
        met.setLights(lights);
        Film myFilm(imgWidth, imgHeight);
        met.sampleImage(&myFilm);
        PNG pic = myFilm.writeImage();
        pic.writeToFile(outputFileName);
    }

    for (unsigned int i = 0; i < allObj.size(); i++) {
        delete allObj[i];
    }
}
