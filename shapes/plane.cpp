#include "plane.h"
#include <limits>

double Plane::intersection(const ray& viewRay)const
{
    double denominator = viewRay.dir.dot(normal);
    if( fabs(denominator) < EPSILON )
        return -1 * std::numeric_limits<double>::max();
    return normal.dot(location - viewRay.orig) / denominator;
}
