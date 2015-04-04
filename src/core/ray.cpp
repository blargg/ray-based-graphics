#include "core/ray.h"
#include "util/debug.h"
#include "core/common.h"

ray::ray(const Vector4d origin, const Vector4d direction):
    orig(origin), dir(direction)
{ /* do nothing */ }

Vector4d ray::operator()(const double time) const {
    return dir * time + orig;
}

Vector4d ray::projectOnToPlane(const Vector4d point) const {
    ASSERT(fabs(point[3] - 1.0) < 0.00001, "points should have extra coordinate set to 1.0");
    ASSERT(isUnitVector(dir), "rays should have unit direction");
    Vector4d diff = point - orig;
    double dist = diff.dot(dir);
    return point - dist * dir;
}
