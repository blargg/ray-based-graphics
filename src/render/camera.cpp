#include <math.h>
#include <string>
#include <algorithm>

#include "core/common.h"
#include "render/camera.h"
#include "util/log.h"

#define BLOCK_SIZE 32

using std::min;
using std::max;

PNG* renderImage(Raytracer &render, Camera cam, int imgWidth, int imgHeight) {

    cam.up.normalize();
    cam.position.dir.normalize();
    Vector4d right = cross(cam.up, cam.position.dir);
    right.normalize();

    double fov = (45 * M_PI) / 180;
    double tmp = cam.worldWidth / (2 * tan(fov));
    Vector4d start = cam.position(-tmp);

    Vector4d bot_right_corner = cam.position.orig -
        (cam.worldWidth / 2.0) * right -
        (cam.worldHeight / 2.0) * cam.up;
    PNG *image = new PNG(imgWidth, imgHeight);
    double dx = (cam.worldWidth / imgWidth);
    double dy = (cam.worldHeight / imgHeight);

    for (int x = 0; x <= imgWidth / BLOCK_SIZE; x++) {
        for (int y = 0; y <= imgHeight / BLOCK_SIZE; y++) {
            for (int xoff = 0;
                 xoff < BLOCK_SIZE && x * BLOCK_SIZE + xoff < imgWidth;
                 xoff++) {
                for (int yoff = 0;
                     yoff < BLOCK_SIZE && y * BLOCK_SIZE + yoff < imgHeight;
                     yoff++) {
                    Vector4d screen_point = bot_right_corner +
                        dx * (BLOCK_SIZE * x + xoff) * right +
                        dy * (BLOCK_SIZE * y + yoff) * cam.up;

                    Vector4d currentDir = screen_point - start;
                    currentDir.normalize();
                    ray viewRay;
                    viewRay.dir = currentDir;
                    viewRay.orig = screen_point;
                    Color c = render.trace(viewRay);
                    *((*image)(BLOCK_SIZE * x + xoff, BLOCK_SIZE * y + yoff)) =
                        RGBAPixel(max(min((int) (c.red * 255), 255), 0),
                                  max(min((int) (c.green * 255), 255), 0),
                                  max(min((int) (c.blue * 255), 255), 0) );
                }
            }
        }
    }

    return image;
}

void pathtraceImage(Film *imageFilm, PathTracer &render,
        Camera cam, int numSamples) {

    cam.up.normalize();
    cam.position.dir.normalize();
    Vector4d right = cross(cam.position.dir, cam.up);
    right.normalize();

    double fov = (45 * M_PI) / 180;
    double tmp = cam.worldWidth / (2 * tan(fov));
    Vector4d start = cam.position(-tmp);

    Vector4d bot_right_corner = cam.position.orig
        - (cam.worldWidth / 2) * right
        - (cam.worldHeight / 2) * cam.up;

    double dx = (cam.worldWidth / imageFilm->getWidth());
    double dy = (cam.worldHeight / imageFilm->getHeight());

    for (int count = 0; count < numSamples; count++) {
        for (int x = 0; x <= imageFilm->getWidth() / BLOCK_SIZE; x++) {
            for (int y = 0; y <= imageFilm->getHeight() / BLOCK_SIZE; y++) {
                for (int xoff = 0;
                     xoff < BLOCK_SIZE && x * BLOCK_SIZE + xoff < imageFilm->getWidth();
                     xoff++) {
                    for (int yoff = 0;
                         yoff < BLOCK_SIZE && y * BLOCK_SIZE + yoff < imageFilm->getHeight();
                         yoff++) {
                        double xRand = (double)rand() / (double)RAND_MAX;
                        double yRand = (double)rand() / (double)RAND_MAX;
                        Vector4d screen_point = bot_right_corner
                            + dx * (BLOCK_SIZE * x + xoff + xRand) * right
                            + dy * (BLOCK_SIZE * y + yoff + yRand) * cam.up;
                        Vector4d currentDir = screen_point - start;
                        currentDir.normalize();
                        ray viewRay;
                        viewRay.dir = currentDir;
                        viewRay.orig = screen_point;
                        Color c = render.trace(viewRay);
                        imageFilm->addColor(c,
                                BLOCK_SIZE * x + xoff,
                                BLOCK_SIZE * y + yoff);
                    }
                }
            }
        }
    }
}

void progressiveRender(string const file_base, PathTracer &render,
        Camera cam, int width, int height, int sampleInterval) {
    int sampleNumber = 1;

    size_t split_point = file_base.find_last_of('/');
    string baseDir;
    string filename;
    if (split_point == string::npos) {
        baseDir = "";
        filename = file_base;
    } else {
        baseDir = file_base.substr(0, split_point + 1);
        filename = file_base.substr(split_point + 1);
    }

    Film myFilm(width, height);
    while (1) {
        pathtraceImage(&myFilm, render, cam, sampleInterval);
        PNG pic = myFilm.writeImage();
        LOG("writting out to file");
        pic.writeToFile(baseDir + std::to_string(sampleNumber) + "_" + filename);

        sampleNumber++;
    }
}
