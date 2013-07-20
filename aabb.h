#ifndef RT_AABB_H
#define RT_AABB_H

#include <Eigen/Dense>
#include "common.h"
#include "ray.h"
using namespace Eigen;

/**
 * Axis Aligned Box.
 * Used for constructing acceleration structures
 *
 * @see KDTree
 */
struct AABB {
    Vector3d minCorner;
    Vector3d maxCorner;

    /**
     * Caclulates the surface area of the AABB.
     */
    float surfaceArea();

    /**
     * Checks if the point is within the bounds of
     * the AABB in the given dimention.
     */
    bool inbounds(Vector3d point, int dim);

    /**
     * Calculates the near and far intersections with the ray.
     * Sets the vaules to the intersection times, even if the
     * AABB lies behind the ray.
     * @post close and far are either the intersections or grabage
     *       if there was no intersection
     * @param ray the ray to test.
     * @param close the close intersection
     * @param far the far intersection
     */
    bool intersection(const ray &test_ray, double &time_min, double &time_max);

    /**
     * returns true if the ray intersects this AABB
     */
    bool intersects(const ray &test_ray);
};

#endif // RT_AABB_H
