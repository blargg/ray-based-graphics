#include "sphere.h"

#define EPSILON 0.0000001

Sphere::Sphere():Shape( point<3>(0.0,0.0,0.0) ),radius(1.0)
{ /* do nothing */ }

Sphere::Sphere(const point<3>& loc, double rad):Shape(loc),radius(rad)
{ /* do nothing */ }

Shape* Sphere::create() const
{
	return new Sphere();
}

Shape* Sphere::clone() const
{
	return new Sphere(*this);
}

bool Sphere::intersects(const ray<3>& viewRay) const
{
	// TODO optimise this, cause it gets called a lot
	
	//only takes rays with unit vectors
	assert(viewRay.dir.length_sq() <= 1.0 + EPSILON);
	assert(viewRay.dir.length_sq() >= 1.0 - EPSILON);

	double B = 2 * (viewRay.dir[0] * (viewRay.orig[0] - location[0]) + 
					viewRay.dir[1] * (viewRay.orig[1] - location[1]) + 
					viewRay.dir[2] * (viewRay.orig[2] - location[2]) );
	double C = (viewRay.orig[0] - location[0] ) * (viewRay.orig[0] - location[0] ) +
			   (viewRay.orig[1] - location[1] ) * (viewRay.orig[1] - location[1] ) +
			   (viewRay.orig[2] - location[2] ) * (viewRay.orig[2] - location[2] ) -
			   radius * radius;
	// The sqrt is irrational an there is no real intersection
	if(B*B - 4*C < 0.0)
		return false;

	double far_intersection = (-B + sqrt(B*B - 4*C) ) / 2.0;

	// calculate the time (distance) to the further intersection
	// if that time is greater than 0, then at least one intersection
	// is in front of the ray
	if(far_intersection < -1.0 * EPSILON)
		return false;

	/*
	 * In cases where the origin of the ray is very close to the surface
	 * of the sphere, there can be strange (random) results.
	 * So instead, check if the ray points into the surface
	 * or away from it (using the normal at the intersection and dot it
	 * with the viewRay)
	 */


	// If far_intersection is pretty much on the surface of the sphere, 
	if(far_intersection < EPSILON)
	{
		// true if the viewRay points into the object,
		// false if viewRay points away
		return normal_vectre(viewRay(far_intersection),point<3>()).dot_prod(viewRay.dir) < 0.0;
	}

	double close_intersection = (-B - sqrt(B*B - 4*C) ) / 2.0; 
	// ray is close to the close intersection
	if(close_intersection < EPSILON && close_intersection > -1.0 * EPSILON)
		return normal_vectre(viewRay(close_intersection),point<3>()).dot_prod(viewRay.dir) < 0.0;


	// the ray isn't on the surface and there is at least a far intersection
	return true;
}

double Sphere::intersection(const ray<3>& viewRay) const
{
	//only takes rays with unit vectors
	assert(viewRay.dir.length_sq() <= 1.0 + EPSILON);
	assert(viewRay.dir.length_sq() >= 1.0 - EPSILON);

	double B = 2 * (viewRay.dir[0] * (viewRay.orig[0] - location[0]) + 
					viewRay.dir[1] * (viewRay.orig[1] - location[1]) + 
					viewRay.dir[2] * (viewRay.orig[2] - location[2]) );
	double C = (viewRay.orig[0] - location[0] ) * (viewRay.orig[0] - location[0] ) +
			   (viewRay.orig[1] - location[1] ) * (viewRay.orig[1] - location[1] ) +
			   (viewRay.orig[2] - location[2] ) * (viewRay.orig[2] - location[2] ) -
			   radius * radius;

	double temp0 = (-B - sqrt(B*B - 4*C) ) / 2.0;
	if(temp0 >= 0.0)
		return temp0;

	// assume that the intersection() will be called only when there is an intersection
	assert((-B + sqrt(B*B - 4*C) ) / 2.0 >= 0.0);
	return (-B + sqrt(B*B - 4*C) ) / 2.0;
}

vectre<3> Sphere::normal_vectre(const point<3>& surface, const point<3>& ray_start) const
{
	/*
	double vals [3];
	vals[0] = surface[0] - location[0];
	vals[1] = surface[1] - location[1];
	vals[2] = surface[2] - location[2];
	return vectre<3>(vals);
	*/
	return vectre<3>( surface[0] - location[0],
			   surface[1] - location[1],
			   surface[2] - location[2]);
}

#undef EPSILON // <----- does this help or hurt?
