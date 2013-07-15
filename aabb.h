#ifndef RT_AABB_H
#define RT_AABB_H

/**
 * Axis Aligned Box.
 * Used for constructing acceleration structures
 *
 * @see KDTree
 */
struct AABB {
    Vector3d minCorner;
    Vector3d maxCorner;
};

#endif // RT_AABB_H
