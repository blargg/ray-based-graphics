#include "plane.h"

bool Plane::intersects(const ray<3>& viewRay)const
{
	double denominator = viewRay.dir.dot_prod(normal);
	// if the viewRay vector is perpendicular to the normal vector, the ray doesn't intersect the Plane.
	if( fabs(denominator) < EPSILON )
		return false;
	vectre<3> diff( location[0] - viewRay.orig[0],
					location[1] - viewRay.orig[1],
					location[2] - viewRay.orig[2]);

	double t = diff.dot_prod(normal) / denominator;
	// if t < Epsilon : return false
	// if t >= Epsilon : return true
	return t >= EPSILON;
}

double Plane::intersection(const ray<3>& viewRay)const
{
	double denominator = viewRay.dir.dot_prod(normal);
	assert(denominator != 0);
	return normal.dot_prod(vectre<3>(viewRay.orig, location)) / denominator;
}
