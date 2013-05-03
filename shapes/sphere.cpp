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

    if(B*B - 4*C < 0.0)
        return -1.0;

    double temp0 = (-B - sqrt(B*B - 4*C) ) / 2.0;
    if(temp0 >= 0.0)
        return temp0;

    return (-B + sqrt(B*B - 4*C) ) / 2.0;
}

vectre<3> Sphere::normal_vectre(const point<3>& surface) const
{
    return vectre<3>( surface[0] - location[0],
               surface[1] - location[1],
               surface[2] - location[2]);
}

#undef EPSILON
