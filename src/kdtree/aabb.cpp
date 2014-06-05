#include "kdtree/aabb.h"
#include <algorithm>

float AABB::surfaceArea() {
    Vector3d dim = maxCorner - minCorner;
    return 2.0 * (dim(0) * dim(1) + dim(0) * dim(2) + dim(1) * dim(2));
}

bool AABB::inbounds(Vector3d point, int dim) {
    return point(dim) <= maxCorner(dim) && point(dim) >= minCorner(dim);
}

bool AABB::intersection(const ray &test_ray, double &time_min, double &time_max) {
    time_min = -1 *std::numeric_limits<double>::max();
    time_max = std::numeric_limits<double>::max();

    for(int dim = 0; dim < 3; dim++) {
        int odim1 = (dim + 1) % 3;
        int odim2 = (dim + 2) % 3;

        if(fabs(test_ray.dir(dim)) < EPSILON &&
                !inbounds(test_ray.orig.head<3>(), odim1) &&
                !inbounds(test_ray.orig.head<3>(), odim2))
                return false;
        double t1 = (maxCorner(dim) - test_ray.orig(dim)) / test_ray.dir(dim);
        double t2 = (minCorner(dim) - test_ray.orig(dim)) / test_ray.dir(dim);
        if(t1 > t2)
            std::swap(t1,t2);

        time_min = std::max(time_min, t1);
        time_max = std::min(time_max, t2);
    }

    return time_min < time_max && time_max > EPSILON;
}

bool AABB::intersects(const ray &test_ray) {
    double tmin, tmax;
    return intersection(test_ray, tmin, tmax);
}
