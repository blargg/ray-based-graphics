#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Eigen/Dense>
#include "shapes/shape.h"
#include "ray.h"
#include "core/common.h"

/**
 * Triangle shape.
 */
class Triangle: public Shape
{
    protected:
    Vector4d p1;
    Vector4d p2;
    Vector4d p3;

    public:
    Triangle();

    /**
     * Constructs a triangle with verticies at the given points.
     */
    Triangle(Vector4d first, Vector4d second, Vector4d third);

    /**
     * virtual default constructor.
     */
    virtual Shape* create() const;

    /**
     * virtual copy constructor.
     */
    virtual Shape* clone() const;

    /**
     * Cleans up and removes the triangle.
     */
    virtual ~Triangle(){/* do nothing */}

    /**
     * Calculates the intersection of the ray and triangle.
     */
    virtual double intersection(const ray& viewRay) const;

    /**
     * Returns the normal vector for the triangle.
     * @param surface a point on the surface
     * @returns a Vector4d for the normal vector.
     */
    virtual Vector4d normal_vector(const Vector4d& surface) const;

    virtual double getMinBound(int axis) const;
    virtual double getMaxBound(int axis) const;
    virtual bool intersectsBox(AABB box) const;
};

#endif//TRIANGLE_H
