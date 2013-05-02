#ifndef SPHERE_H
#define SPHERE_H

#include <assert.h>
#include <limits>

#include "../shape.h"
#include "../ray.h"
#include "../vectre.h"
#include "../point.h"

class Sphere: public Shape
{
	protected:
	double radius;

	public:
	Sphere();
	Sphere(const point<3>& loc, double rad = 1.0);

	virtual Shape* create() const;
	virtual Shape* clone() const;

	virtual ~Sphere(){ /* do nothing */ }

	virtual bool intersects(const ray<3>& viewRay) const;
	virtual double intersection(const ray<3>& viewRay) const;
	virtual vectre<3> normal_vectre(const point<3>& surface) const;
};
#endif
