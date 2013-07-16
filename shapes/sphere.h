#ifndef SPHERE_H
#define SPHERE_H

#include <assert.h>
#include <limits>

#include "../common.h"
#include "../shape.h"
#include "../ray.h"

/**
 * Defines a sphere Shape.
 */
class Sphere: public Shape
{
    protected:
    Vector4d location;
    double radius;

    public:
    /**
     * Initializes a Sphere at the origin
     */
    Sphere();

    /**
     * Initializes a Sphere centered at the location with radius of rad.
     * @param loc the center of the sphere.
     * @param rad the radius of the sphere.
     */
    Sphere(const Vector4d& loc, double rad = 1.0);

    /**
     * Virtual default constructor.
     * Allocates a new Sphere() and returns the pointer.
     * @returns a new sphere.
     */
    virtual Shape* create() const;

    /**
     * Virtual copy constructor.
     * Allocates a new Sphere that is a copy of this one and returns the pointer.
     * @returns a pointer to the copied sphere.
     */
    virtual Shape* clone() const;

    virtual ~Sphere(){ /* do nothing */ }

    /**
     * Returns the intersection point for the ray.
     * If there is an intersection, this returns the scale factor
     * for the ray to reach the closest intersection.
     * Otherwise returns some negative number.
     */
    virtual double intersection(const ray& viewRay) const;

    /**
     * Returns the normal vector for a point on the surface.
     * If the point isn't on the surface, behavoir is undefined.
     * @param surface the point on the surface.
     * @returns a normal vector for the surface.
     */
    virtual Vector4d normal_vectre(const Vector4d& surface) const;

    virtual double getMinBound(int axis) const;
    virtual double getMaxBound(int axis) const;
    virtual bool intersectsBox(AABB box) const;
};
#endif
