#ifndef DRAWBLE_H
#define DRAWBLE_H

#include "core/ray.h"
#include "core/properties.h"
#include "kdtree/aabb.h"

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
    virtual double intersection(const ray viewRay) const = 0;

    /**
     * @brief Gets the normal vectre for the given point on the Drawable's surface.
     */
    virtual Vector4d normal_vector(const Vector4d surface) const = 0;

    /**
     * @brief Gets the Properties of the Drawable at the given point.
     */
    virtual Properties getProperties(const Vector4d location) const = 0;

    /**
     * Returns the minimum bound for the drawable.
     */
    virtual double getMinBound(int axis) const = 0;

    /**
     * Returns the maximum bount for the drawable
     */
    virtual double getMaxBound(int axis) const = 0;

    /**
     * Checks if the Drawable intersects the aabox
     * @returns true if the drawable intersects the box
     */
    virtual bool intersectsBox(AABB box) const = 0;
};

#endif
