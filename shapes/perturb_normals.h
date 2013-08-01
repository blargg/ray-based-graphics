#ifndef RT_PERTURB_NORMALS_H
#define RT_PERTURB_NORMALS_H

#include "../shape.h"
#include "../common.h"
#include "../ray.h"

/**
 * Wrapper shape that randomly perturbs the normals of the original shape.
 * Every call to normal_vector() is randomly offset form the original shape.
 * This is for pathtracing algorithms and will look strange in raytracing algorithms.
 */
class PerturbNormals: public Shape
{
public:
    /**
     * Constructs the new shape.
     * @param s the original shape to wrap.
     * @param angle the maximum angle (radians) that a vector will get offset by.
     */
    PerturbNormals (const Shape& s, double angle);
    virtual ~PerturbNormals ();

    virtual Shape* create() const;
    virtual Shape* clone() const;

    /**
     * Passes the information to the shape it wraps for the intersection
     */
    virtual double intersection(const ray& viewRay) const;

    /**
     * Takes the normal vector from the wrapped shape and randomly perturbs it.
     */
    virtual Vector4d normal_vector(const Vector4d& surface) const;

    virtual double getMinBound(int axis) const;
    virtual double getMaxBound(int axis) const;
    virtual bool intersectsBox(AABB box) const;

private:
    Shape* shape;
    double angle;
};

#endif // RT_PERTURB_NORMALS_H
