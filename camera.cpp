#include "camera.h"
#include <math.h>
#include <omp.h>

#define BLOCK_SIZE 32

using std::min;
using std::max;

PNG* renderImage(Raytracer render, int imgSize, double worldSize,
                 ray<3> position, vectre<3> up) {

    up = up.unit_vectre();
    position.dir = position.dir.unit_vectre();
    vectre<3> right = up.cross_prod(position.dir);
    right = right.unit_vectre();

    double fov = (45 * M_PI) / 180;
    double tmp = worldSize / (2 * tan(fov));
    point<3> start = position(-tmp);

    point<3> bot_right_corner = position.orig;
    bot_right_corner = sub(bot_right_corner, (worldSize / 2) * right);
    bot_right_corner = sub(bot_right_corner, (worldSize / 2) * up);

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
                    point<3> screen_point = bot_right_corner;
                    screen_point = add(screen_point, dx * (BLOCK_SIZE * x + xoff) * right);
                    screen_point = add(screen_point, dy * (BLOCK_SIZE * y + yoff) * up);

                    vectre<3> currentDir(start, screen_point);
                    currentDir = currentDir.unit_vectre();
                    ray<3> viewRay;
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
                 ray<3> position, vectre<3> up, int numSamples) {

    up = up.unit_vectre();
    position.dir = position.dir.unit_vectre();
    vectre<3> right = up.cross_prod(position.dir);
    right = right.unit_vectre();

    double fov = (45 * M_PI) / 180;
    double tmp = worldSize / (2 * tan(fov));
    point<3> start = position(-tmp);

    point<3> bot_right_corner = position.orig;
    bot_right_corner = sub(bot_right_corner, (worldSize / 2) * right);
    bot_right_corner = sub(bot_right_corner, (worldSize / 2) * up);

    //PNG *image = new PNG(imgSize, imgSize);
    double dx = (worldSize / imgSize);
    double dy = (worldSize / imgSize);

    for(int count = 0; count < numSamples; count++) {
        for(int x = 0; x <= imgSize / BLOCK_SIZE; x++) {
            for(int y = 0; y <= imgSize / BLOCK_SIZE; y++) {
                for(int xoff = 0; xoff < BLOCK_SIZE && x * BLOCK_SIZE + xoff < imgSize; xoff++){
                    for(int yoff = 0; yoff < BLOCK_SIZE && y * BLOCK_SIZE + yoff < imgSize; yoff++){
                        point<3> screen_point = bot_right_corner;
                        screen_point = add(screen_point, dx * (BLOCK_SIZE * x + xoff) * right);
                        screen_point = add(screen_point, dy * (BLOCK_SIZE * y + yoff) * up);

                        vectre<3> currentDir(start, screen_point);
                        currentDir = currentDir.unit_vectre();
                        ray<3> viewRay;
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
