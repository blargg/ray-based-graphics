#ifndef SPHERE_H
#define SPHERE_H

#include <assert.h>
#include <limits>

#include "../common.h"
#include "../shape.h"
#include "../ray.h"

class Sphere: public Shape
{
    protected:
    Vector4d location;
    double radius;

    public:
    Sphere();
    Sphere(const Vector4d& loc, double rad = 1.0);

    virtual Shape* create() const;
    virtual Shape* clone() const;

    virtual ~Sphere(){ /* do nothing */ }

    virtual double intersection(const ray& viewRay) const;
    virtual Vector4d normal_vectre(const Vector4d& surface) const;
};
#endif
