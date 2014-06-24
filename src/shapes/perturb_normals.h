#ifndef RT_PERTURB_NORMALS_H
#define RT_PERTURB_NORMALS_H

#include "shapes/shape.h"
#include "core/common.h"
#include "core/ray.h"

/**
 * Wrapper shape that randomly perturbs the normals of the original shape.
 * Every call to normal_vector() is randomly offset form the original shape.
 * This is for pathtracing algorithms and will look strange in raytracing algorithms.
 */
template<class T>
class PerturbNormals: public T
{
public:

    // TODO
    // clone and create cause a problem
    // requires more information about the template object
    using T::T;

    double angle = M_PI/8;

    /**
     * Takes the normal vector from the wrapped shape and randomly perturbs it.
     */
    virtual Vector4d normal_vector(const Vector4d& surface) const {
        return perturb(T::normal_vector(surface), angle);
    }

    virtual void setAngle(const double a) {
        angle = a;
    }
};

#endif // RT_PERTURB_NORMALS_H
