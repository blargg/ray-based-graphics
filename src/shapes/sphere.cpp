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

Vector4d Sphere::normal_vector(const Vector4d& surface) const
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
    double dmin = 0.0;

    for(int i = 0; i < 3; i++) {
        if(location(i) < box.minCorner(i)){
            dmin += (location(i) - box.minCorner(i)) * (location(i) - box.minCorner(i));
        }
        else if(location(i) > box.maxCorner(i)) {
            dmin += (location(i) - box.maxCorner(i)) * (location(i) - box.maxCorner(i));
        }
    }

    return dmin < (radius * radius);
}

Vector4d Sphere::randomSurfacePoint() const {
    double u = ((double)rand() / (double)(RAND_MAX));
    double v = ((double)rand() / (double)(RAND_MAX));
    double theta = u * 2.0 * M_PI;
    double phi = acos(2 * v - 1);
    return location + radius * Vector4d(sin(phi) * cos(theta),
                                        sin(phi) * sin(theta),
                                        cos(phi),
                                        0);
}
