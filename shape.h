#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include <Eigen/Dense>
#include "common.h"

using namespace Eigen;

/**
 * @class Shape
 * @ingroupe Shapes
 *
 * @brief Virtual class for a 3d shape.
 * Allows objects to be moved and rotated. Defines the intersections with
 * rays.
 */
class Shape
{
    protected:

    public:
    /// Default constructor. Does nothing.
    Shape();

    /// Virtual deconstructor.
    virtual ~Shape(){ /* Do nothing */ }

    /// Virtual default constructor for Shapes
    virtual Shape* create() const = 0;
    /// Virtual copy constructor for Shapes.
    virtual Shape* clone() const = 0;


    /// Gives the closest intersection in front of the ray.
    virtual double intersection(const ray& viewRay) const = 0;
    /// Gives the normal vectre for the given point.
    virtual Vector4d normal_vectre(const Vector4d& surface) const = 0;

};
#endif
