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

    float surfaceArea() {
        Vector3d dim = maxCorner - minCorner;
        return 2.0 * (dim(0) * dim(1) + dim(0) * dim(2) + dim(1) * dim(2));
    };
};

#endif // RT_AABB_H
