#ifndef LIGHT_H
#define LIGHT_H

/**
 * @brief Simple point light for the ray tracer.
 *
 * Stores the location of the light and the intensities
 * of the red green and blue components of the light.
 */
struct light
{
	/// The location of the light.
	point<3> location;

	/// The intensity of the red light.
	double red;

	/// The intensity of the green light.
	double green;

	/// The intensity of the blue light.
	double blue;

	/// Default consturctor. Puts it at the origin with a white light.
	light():location(point<3>(0.0,0.0,0.0)),red(1.0),green(1.0),blue(1.0) { /* do nothing */ }
};

#endif
