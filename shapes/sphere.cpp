#include "sphere.h"
#include <limits>

Sphere::Sphere():location(0,0,0,1),radius(1.0)
{ /* do nothing */ }

Sphere::Sphere(const Vector4d& loc, double rad):location(loc),radius(rad)
{ /* do nothing */ }

Shape* Sphere::create() const
{
    return new Sphere();
}

Shape* Sphere::clone() const
{
    return new Sphere(*this);
}

double Sphere::intersection(const ray& viewRay) const
{
    double B = 2 * (viewRay.dir(0) * (viewRay.orig(0) - location(0)) +
                    viewRay.dir(1) * (viewRay.orig(1) - location(1)) +
                    viewRay.dir(2) * (viewRay.orig(2) - location(2)) );
    double C = (viewRay.orig(0) - location(0) ) * (viewRay.orig(0) - location(0) ) +
               (viewRay.orig(1) - location(1) ) * (viewRay.orig(1) - location(1) ) +
               (viewRay.orig(2) - location(2) ) * (viewRay.orig(2) - location(2) ) -
               radius * radius;

    if(B*B - 4*C < 0.0)
        return -1 * std::numeric_limits<double>::max();

    double temp0 = (-B - sqrt(B*B - 4*C) ) / 2.0;
    if(temp0 >= 0.0)
        return temp0;

    return (-B + sqrt(B*B - 4*C) ) / 2.0;
}

Vector4d Sphere::normal_vectre(const Vector4d& surface) const
{
    return surface - location;
}

double Sphere::getMinBound(int axis) const {
    return location(axis) - radius;
}

double Sphere::getMaxBound(int axis) const {
    return location(axis) - radius;
}

bool Sphere::intersectsBox(AABB box) const {
    return true;
}

#undef EPSILON
