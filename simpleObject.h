#ifndef SIMPLEOBJECT_H
#define SIMPLEOBJECT_H

// math dependencies
#include "ray.h"
#include "vectre.h"
#include "point.h"

// reqired raytracing dependencies
#include "drawable.h"
#include "shape.h"
#include "material.h"

// not really required, but allows the default constructor to
// not brake the system.
#include "shapes/sphere.h"
#include "materials/solidColor.h"

#include "assert.h"

class SimpleObject: public Drawable
{
    private:
    /// The shape of the SimpleObject
    Shape* shape;
    /// The texture of the Object
    Material* texture;

    void copy(const SimpleObject& other);

    public:
    SimpleObject();
    SimpleObject(const Shape& s, const Material& mat);
    SimpleObject(const SimpleObject& other);

    SimpleObject& operator=(SimpleObject const & rhs);

    virtual ~SimpleObject();

    virtual double intersection(const ray<3> viewRay) const;
    virtual vectre<3> normal_vectre(const point<3> surface) const;
    virtual Properties getProperties(const point<3> location) const;

    void setShape(const Shape& s);
    void setMaterial(const Material& mat);
};

#endif
