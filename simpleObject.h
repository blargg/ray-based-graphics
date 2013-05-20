#ifndef SIMPLEOBJECT_H
#define SIMPLEOBJECT_H

#include "ray.h"
#include "common.h"

#include "drawable.h"
#include "shape.h"
#include "material.h"

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

    virtual double intersection(const ray viewRay) const;
    virtual Vector4d normal_vectre(const Vector4d surface) const;
    virtual Properties getProperties(const Vector4d location) const;

    void setShape(const Shape& s);
    void setMaterial(const Material& mat);
};

#endif
