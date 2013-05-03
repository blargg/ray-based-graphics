#ifndef DRAWBLE_H
#define DRAWBLE_H

#include "ray.h"
#include "vectre.h"
#include "point.h"
#include "properties.h"

struct intersection {
    double time;
    vectre<3> normal;
    Properties p;
};


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
     * @brief returns the time for the ray to intersect the object, or negative
     * if there is no intersection.
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
