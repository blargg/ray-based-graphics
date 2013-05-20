#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../shape.h"
#include "../ray.h"
#include <Eigen/Dense>
#include "../common.h"

class Triangle: public Shape
{
    protected:
    Vector4d p1;
    Vector4d p2;
    Vector4d p3;

    public:
    Triangle();
    Triangle(Vector4d first, Vector4d second, Vector4d third);

    virtual Shape* create() const;
    virtual Shape* clone() const;

    virtual ~Triangle(){/* do nothing */}

    virtual double intersection(const ray& viewRay) const;
    virtual Vector4d normal_vectre(const Vector4d& surface) const;
};

#endif//TRIANGLE_H
