#ifndef RT_BOUNDINGSHAPE_H
#define RT_BOUNDINGSHAPE_H

#include <vector>

#include "drawable.h"
#include "material.h"
#include "shape.h"
#include "ray.h"

class BoundingShape {
    public:

    BoundingShape(const Shape& s);
    virtual ~BoundingShape();

    void addDrawable(Drawable *obj);

    /**
     * finds the closes intersecting object for a ray
     * returns null if there is not intersection
     */
    Drawable *intersectionObj(const ray<3> viewRay) const;

    private:
    Shape *boundry;
    std::vector<Drawable *> objList;
};

#endif // RT_BOUNDINGSHAPE_H
