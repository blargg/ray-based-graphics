#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"
#include "film.h"
#include "easypng.h"
#include "ray.h"
#include "func.h"

/**
 * allocates space for an image, then renders the image, returning a pointer
 *
 * render the ratracer to get image data from
 * imgSize the number of pixels (width and height) in the final image
 * worldSize the size of the square in the world for the image to render
 * position the position and direction of the image to render
 * up a vector pointing to the images' up direction
 */
PNG* renderImage(Raytracer render, int imgSize, double worldSize,
                 ray<3> position, vectre<3> up);

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
void pathtraceImage(Film *imageFilm, Raytracer render, int imgSize, double worldSize,
                 ray<3> position, vectre<3> up, int numSamples);

#endif // CAMERA_H
