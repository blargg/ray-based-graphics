#include "plane.h"
#include <limits>

double Plane::intersection(const ray<3>& viewRay)const
{
    double denominator = viewRay.dir.dot_prod(normal);
    if( fabs(denominator) < EPSILON )
        return -1 * std::numeric_limits<double>::max();
    return normal.dot_prod(vectre<3>(viewRay.orig, location)) / denominator;
}
