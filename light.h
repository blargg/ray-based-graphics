#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "point.h"

/**
 * @brief Simple point light for the ray tracer.
 *
 * Stores the location of the light and the intensities
 * of the red green and blue components of the light.
 */
struct Light
{
	/// The location of the light.
	point<3> location;

	/// The color of the light
	Color color;

	/// Default consturctor. Puts it at the origin with a white light.
	Light():location(point<3>(0.0,0.0,0.0)),color(1.0,1.0,1.0) { /* do nothing */ }
};

#endif
