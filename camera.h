#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <string>
#include "raytracer.h"
#include "common.h"
#include "film.h"
#include "easypng.h"
#include "ray.h"

using std::string;

struct Camera
{
    int imgSize;
    double worldSize;
    ray position;
    Vector4d up;
};

/**
 * allocates space for an image, then renders the image, returning a pointer
 *
 * render the ratracer to get image data from
 * imgSize the number of pixels (width and height) in the final image
 * worldSize the size of the square in the world for the image to render
 * position the position and direction of the image to render
 * up a vector pointing to the images' up direction
 */
PNG* renderImage(Raytracer render, Camera cam);

/**
 * takes a pointer to a film object, and adds color values to the film based
 * on the renderer and parameters.
 *
 * imgeFilm the pointer to the film to operate on
 * render the renderer for the scene
 * imgSize the size of the image to render
 * worldSize the size of the viewing window in the scene
 * position the location and direction of the camera
 * up the up direction for the camera
 * numSamples the number of samples for each pixel in the film
 */
void pathtraceImage(Film *imageFilm, Raytracer render, Camera cam, int numSamples);

/**
 * Repeatedly renders a number of samples, then writes the current image to a file
 * based on the file_base name
 */
void progressiveRender(string const file_base, Raytracer render, Camera cam, int sampleInterval);

#endif // CAMERA_H
