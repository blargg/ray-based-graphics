#ifndef DRAWBLE_H
#define DRAWBLE_H

#include "ray.h"
#include "vectre.h"
#include "point.h"
#include "properties.h"


/**
 * @brief An interface for anything that can be drawn in the Ray tracing algorithm
 * If you want to make an object to be rendered, extend this class and implements the
 * methods here. NOTE: There are dirived classes of this that would provide a good start,
 * as this is just the minimum to draw an object.
 */
class Drawable
{
	public:
	/**
	 * @brief A virtual destructor to be over written later
	 */
	virtual ~Drawable(){ /* Do nothing */ }

	/**
	 * @brief Given a ray, returns true if the Drawable intersects the ray.
	 *
	 * @param viewRay The ray to test.
	 * @returns True if they intersect, false otherwise
	 */
	virtual bool intersects(const ray<3> viewRay) const = 0;

	/**
	 * @brief returns the time for the ray to reach the intersection.
	 * If you start at the viewRay's starting point, and travel in the 
	 * ray's direction at a speed of one lenght of the ray per second,
	 * the time returned is the time to reach the intersection.
	 * In other words viewRay(time) is the point of intersection.
	 *
	 * If there are multiple intersection, this will return the one closest
	 * to the camera (smalles time)
	 * This will not return negative values (ie. intersections behind the camera)
	 *
	 * @param viewRay The ray to test.
	 * @returns The time for the ray to reach the intersection point.
	 */
	virtual double intersection(const ray<3> viewRay) const = 0;

	/**
	 * @brief Gets the normal vectre for the given point on the Drawable's surface.
	 */
	virtual vectre<3> normal_vectre(const point<3> surface) const = 0;

	/**
	 * @brief Gets the Properties of the Drawable at the given point.
	 */
	virtual Properties getProperties(const point<3> location) const = 0;
};

#endif
