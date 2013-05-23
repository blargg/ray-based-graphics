#include "camera.h"
#include <math.h>
#include <omp.h>
#include "common.h"

#define BLOCK_SIZE 32

using std::min;
using std::max;

PNG* renderImage(Raytracer render, int imgSize, double worldSize,
                 ray position, Vector4d up) {

    up.normalize();
    position.dir.normalize();
    Vector4d right = cross(up, position.dir);
    right.normalize();

    double fov = (45 * M_PI) / 180;
    double tmp = worldSize / (2 * tan(fov));
    Vector4d start = position(-tmp);

    Vector4d bot_right_corner = position.orig -
        (worldSize / 2.0) * right -
        (worldSize / 2.0) * up;
    PNG *image = new PNG(imgSize, imgSize);
    double dx = (worldSize / imgSize);
    double dy = (worldSize / imgSize);

    omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp for
    for(int x = 0; x <= imgSize / BLOCK_SIZE; x++) {
        for(int y = 0; y <= imgSize / BLOCK_SIZE; y++) {
            for(int xoff = 0; xoff < BLOCK_SIZE && x * BLOCK_SIZE + xoff < imgSize; xoff++){
                for(int yoff = 0; yoff < BLOCK_SIZE && y * BLOCK_SIZE + yoff < imgSize; yoff++){
                    Vector4d screen_point = bot_right_corner +
                        dx * (BLOCK_SIZE * x + xoff) * right +
                        dy * (BLOCK_SIZE * y + yoff) * up;

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

void pathtraceImage(Film *imageFilm, Raytracer render, int imgSize, double worldSize,
                 ray position, Vector4d up, int numSamples) {

    up.normalize();
    position.dir.normalize();
    Vector4d right = cross(up, position.dir);
    right.normalize();

    double fov = (45 * M_PI) / 180;
    double tmp = worldSize / (2 * tan(fov));
    Vector4d start = position(-tmp);

    Vector4d bot_right_corner = position.orig
        -(worldSize / 2) * right
        - (worldSize / 2) * up;

    //PNG *image = new PNG(imgSize, imgSize);
    double dx = (worldSize / imgSize);
    double dy = (worldSize / imgSize);

    for(int count = 0; count < numSamples; count++) {
        for(int x = 0; x <= imgSize / BLOCK_SIZE; x++) {
            for(int y = 0; y <= imgSize / BLOCK_SIZE; y++) {
                for(int xoff = 0; xoff < BLOCK_SIZE && x * BLOCK_SIZE + xoff < imgSize; xoff++){
                    for(int yoff = 0; yoff < BLOCK_SIZE && y * BLOCK_SIZE + yoff < imgSize; yoff++){
                        Vector4d screen_point = bot_right_corner
                            + dx * (BLOCK_SIZE * x + xoff) * right
                            + dy * (BLOCK_SIZE * y + yoff) * up;
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

void progressiveRender(string const file_base, Raytracer render, int imgSize, double worldSize,
        ray position, Vector4d up, int sampleInterval) {
    int sampleNumber = 1;

    Film myFilm(imgSize, imgSize);
    while(1) {
        pathtraceImage(&myFilm, render, imgSize, worldSize, position, up, sampleInterval);
        PNG pic = myFilm.writeImage();
        pic.writeToFile(file_base + std::to_string(sampleNumber) + ".png");

        sampleNumber++;
    }
}
