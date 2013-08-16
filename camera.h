#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <string>
#include "raytracer.h"
#include "pathtracer.h"
#include "common.h"
#include "film.h"
#include "easypng.h"
#include "ray.h"

using std::string;

/**
 * Represents the data to define a Camera.
 * Takes the position, orientation, and sizes.
 */
struct Camera
{
    /// The height of the final image (eg a PNG) of the render.
    int imgHeight;
    /// The width of the final image of the render.
    int imgWidth;
    /// The height of the camera in the world.
    double worldHeight;
    /// The height of the camera in the world.
    double worldWidth;
    /// The position of the center and the direction to point.
    ray position;
    /// The "up" direction for the camera.
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
PNG* renderImage(Raytracer &render, Camera cam);

/**
 * takes a pointer to a film object, and adds color values to the film based
 * on the renderer and parameters.
 *
 * imgeFilm the pointer to the film to operate on
 * render the renderer for the scene
 * cam the specification for the camera to render
 * numSamples the number of samples for each pixel in the film
 */
void pathtraceImage(Film *imageFilm, PathTracer &render, Camera cam, int numSamples);

/**
 * Repeatedly renders a number of samples, then writes the current image to a file
 * based on the file_base name
 */
void progressiveRender(string const file_base, PathTracer &render, Camera cam, int sampleInterval);

#endif // CAMERA_H
