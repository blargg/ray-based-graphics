#ifndef PLANE_H
#define PLANE_H

#include "../shape.h"
#include "assert.h"

#define EPSILON 0.00000000001

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
	/// The normal vectre to the plane (same for all points on it's surface)
	vectre<3> normal;

	public:
	Plane():Shape(),normal(1,0,0){ /* do nothing */ }
	Plane(point<3> loc, vectre<3> n):Shape(loc),normal(n){ /* do nothing */ }

	/// Make a new Plane on the heap and return it's pointer.
	virtual Shape* create()const{ return new Plane(); }

	/// Make a copy of this Plane on the heap and return it's pointer.
	virtual Shape* clone()const{ return new Plane(*this); }

	/// Returns true if the ray intersects the Plane.
	virtual bool intersects(const ray<3>& viewRay)const;

	/// Returns the closest intersection with the ray. (See Shape).
	virtual double intersection(const ray<3>& viewRay)const;

	/// Returns the Plane's normal vectre.
	virtual vectre<3> normal_vectre(const point<3>& surface)const{ return normal; }
};
#endif //PLANE_H
