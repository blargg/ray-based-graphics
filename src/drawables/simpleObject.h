#ifndef SIMPLEOBJECT_H
#define SIMPLEOBJECT_H

#include "core/ray.h"
#include "core/common.h"

#include "drawables/drawable.h"
#include "shapes/shape.h"
#include "materials/material.h"

#include "shapes/sphere.h"
#include "materials/solidColor.h"

#include "assert.h"

/**
 * Drawable that pairs a shape with a material.
 * Simply calls the appropiate functions of the objects you pass to it.
 * This allows mixing of any shape with any material.
 */
class SimpleObject: public Drawable
{
    private:
    /// The shape of the SimpleObject
    Shape* shape;
    /// The texture of the Object
    Material* texture;

    void copy(const SimpleObject& other);

    public:
    /**
     * Initialize a SimpleObject with a sphere and a solid color
     */
    SimpleObject();

    /**
     * Initialize the SimpleObject with the given shape and material.
     * Copies the given shape and material to the internal object.
     * @param s the shape for the simple object
     * @param mat the material to use
     */
    SimpleObject(const Shape& s, const Material& mat);

    /**
     * Makes a new SimpleObject as a copy of antoher.
     */
    SimpleObject(const SimpleObject& other);

    /**
     * Defines the assignment operator.
     */
    SimpleObject& operator=(SimpleObject const & rhs);

    /**
     * Cleans up the simple object.
     * Deletes the internal shape and material.
     */
    virtual ~SimpleObject();

    virtual double intersection(const ray viewRay) const;
    virtual Vector4d normal_vector(const Vector4d surface) const;
    virtual Properties getProperties(const Vector4d location) const;
    virtual double getMinBound(int axis) const;
    virtual double getMaxBound(int axis) const;
    virtual bool intersectsBox(AABB box) const;
    virtual Vector4d randomSurfacePoint() const;

    void setShape(const Shape& s);
    void setMaterial(const Material& mat);
};

#endif
