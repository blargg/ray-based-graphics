#ifndef PLANE_H
#define PLANE_H

#include "../shape.h"
#include "../common.h"
#include <Eigen/Dense>
#include "assert.h"

/**
 * \class Shape
 * \brief A plane shape.
 * Defined by a normal vectre and a point on it's surface.
 *
 * \note A single plane in space is not a closed shape. Be careful
 * when using transparency, because transparency isn't designed to 
 * support open surfaces (although it is sometime useful).
 */
class Plane: public Shape
{
    protected:
    Vector4d location;
    /// The normal vectre to the plane (same for all points on it's surface)
    Vector4d normal;

    public:
    Plane():Shape(),location(0,0,0,1),normal(1,0,0,0){ /* do nothing */ }
    Plane(Vector4d loc, Vector4d n):Shape(),location(loc),normal(n){ /* do nothing */ }

    /// Make a new Plane on the heap and return it's pointer.
    virtual Shape* create()const{ return new Plane(); }

    /// Make a copy of this Plane on the heap and return it's pointer.
    virtual Shape* clone()const{ return new Plane(*this); }

    /// Returns the closest intersection with the ray. (See Shape).
    virtual double intersection(const ray& viewRay)const;

    /// Returns the Plane's normal vectre.
    virtual Vector4d normal_vectre(const Vector4d& surface)const{ return normal; }
};
#endif //PLANE_H
