#ifndef RT_PERTURB_NORMALS_H
#define RT_PERTURB_NORMALS_H

#include "../shape.h"
#include "../common.h"
#include "../ray.h"

class PerturbNormals: public Shape
{
public:
    PerturbNormals (const Shape& s, double angle);
    virtual ~PerturbNormals ();

    virtual Shape* create() const;
    virtual Shape* clone() const;

    virtual double intersection(const ray& viewRay) const;
    virtual Vector4d normal_vectre(const Vector4d& surface) const;

private:
    Shape* shape;
    double angle;
};

#endif // RT_PERTURB_NORMALS_H
