#include "camera.h"
#include <math.h>
#include <omp.h>
#include "common.h"

#define BLOCK_SIZE 32

using std::min;
using std::max;

PNG* renderImage(Raytracer render, Camera cam) {

    cam.up.normalize();
    cam.position.dir.normalize();
    Vector4d right = cross(cam.up, cam.position.dir);
    right.normalize();

    double fov = (45 * M_PI) / 180;
    double tmp = cam.worldSize / (2 * tan(fov));
    Vector4d start = cam.position(-tmp);

    Vector4d bot_right_corner = cam.position.orig -
        (cam.worldSize / 2.0) * right -
        (cam.worldSize / 2.0) * cam.up;
    PNG *image = new PNG(cam.imgSize, cam.imgSize);
    double dx = (cam.worldSize / cam.imgSize);
    double dy = (cam.worldSize / cam.imgSize);

    omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp for
    for(int x = 0; x <= cam.imgSize / BLOCK_SIZE; x++) {
        for(int y = 0; y <= cam.imgSize / BLOCK_SIZE; y++) {
            for(int xoff = 0; xoff < BLOCK_SIZE && x * BLOCK_SIZE + xoff < cam.imgSize; xoff++){
                for(int yoff = 0; yoff < BLOCK_SIZE && y * BLOCK_SIZE + yoff < cam.imgSize; yoff++){
                    Vector4d screen_point = bot_right_corner +
                        dx * (BLOCK_SIZE * x + xoff) * right +
                        dy * (BLOCK_SIZE * y + yoff) * cam.up;

                    Vector4d currentDir = screen_point - start;
                    currentDir.normalize();
                    ray viewRay;
                    viewRay.dir = currentDir;
                    viewRay.orig = screen_point;
                    Color c = render.getColor(viewRay, 3);
                    *((*image)(BLOCK_SIZE * x + xoff, BLOCK_SIZE * y + yoff)) =
                        RGBAPixel(max(min( (int) (c.red * 255), 255), 0),
                                  max(min( (int) (c.green * 255), 255), 0),
                                  max(min( (int) (c.blue * 255), 255), 0) );
                }
            }
        }
    }

    }

    return image;
}

void pathtraceImage(Film *imageFilm, Raytracer render, Camera cam, int numSamples) {

    cam.up.normalize();
    cam.position.dir.normalize();
    Vector4d right = cross(cam.up, cam.position.dir);
    right.normalize();

    double fov = (45 * M_PI) / 180;
    double tmp = cam.worldSize / (2 * tan(fov));
    Vector4d start = cam.position(-tmp);

    Vector4d bot_right_corner = cam.position.orig
        -(cam.worldSize / 2) * right
        - (cam.worldSize / 2) * cam.up;

    //PNG *image = new PNG(cam.imgSize, cam.imgSize);
    double dx = (cam.worldSize / cam.imgSize);
    double dy = (cam.worldSize / cam.imgSize);

    for(int count = 0; count < numSamples; count++) {
        for(int x = 0; x <= cam.imgSize / BLOCK_SIZE; x++) {
            for(int y = 0; y <= cam.imgSize / BLOCK_SIZE; y++) {
                for(int xoff = 0; xoff < BLOCK_SIZE && x * BLOCK_SIZE + xoff < cam.imgSize; xoff++){
                    for(int yoff = 0; yoff < BLOCK_SIZE && y * BLOCK_SIZE + yoff < cam.imgSize; yoff++){
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
                        Color c = render.pathtraceColor(viewRay, 3);
                        imageFilm->addColor(c, BLOCK_SIZE * x + xoff, BLOCK_SIZE * y + yoff);
                    }
                }
            }
        }
    }

}

void progressiveRender(string const file_base, Raytracer render, Camera cam, int sampleInterval) {
    int sampleNumber = 1;

    Film myFilm(cam.imgSize, cam.imgSize);
    while(1) {
        pathtraceImage(&myFilm, render, cam, sampleInterval);
        PNG pic = myFilm.writeImage();
        pic.writeToFile(file_base + std::to_string(sampleNumber) + ".png");

        sampleNumber++;
    }
}
