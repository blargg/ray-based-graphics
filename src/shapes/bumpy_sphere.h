#ifndef RT_BUMPY_SPHERE_H
#define RT_BUMPY_SPHERE_H

#include "core/easypng.h"

#include "core/ray.h"
#include "drawable.h"
#include "properties.h"
#include "shapes/sphere.h"
#include "materials/solidColor.h"

/**
 * A simple bump mapped sphere.
 * The texture file pointed to must be a generated normal map
 */
class BumpySphere : public Drawable {
    public:
    /// Defaut has no bumps
    BumpySphere():sphere(point<3>(0,0,0),10),mat(Color(0,0,1)),bumpMap()
    { /* Do nothing */ }
    BumpySphere(Sphere s, Color c, string const filename):sphere(s),mat(c),bumpMap(filename)
    { /* Do nothing */ }

    virtual double intersection(const ray<3> viewRay) const;
    virtual vectre<3> normal_vector(const point<3> surface) const;
    virtual Properties getProperties(const point<3> location) const;

    private:
    Sphere sphere;
    SolidColor mat;
    PNG bumpMap;
};

#endif // RT_BUMPY_SPHERE_H
