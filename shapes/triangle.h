#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../shape.h"
#include "../ray.h"
#include "../vectre.h"
#include "../point.h"

class Triangle: public Shape
{
    protected:
    point<3> p1;
    point<3> p2;
    point<3> p3;

    public:
    Triangle();
    Triangle(point<3> first, point<3> second, point<3> third);

    virtual Shape* create() const;
    virtual Shape* clone() const;

    virtual ~Triangle(){/* do nothing */}

    virtual bool intersects(const ray<3>& viewRay) const;
    virtual double intersection(const ray<3>& viewRay) const;
    virtual vectre<3> normal_vectre(const point<3>& surface) const;
};

#endif//TRIANGLE_H
