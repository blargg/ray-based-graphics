#include "bounding_shape.h"
#include <stdio.h>

using std::cout;

BoundingShape::BoundingShape(const Shape& s) {
    boundry = s.clone();
}

BoundingShape::~BoundingShape() {
    //delete boundry;
}

void BoundingShape::addDrawable(Drawable *obj) {
    objList.push_back(obj);
}

Drawable  *BoundingShape::intersectionObj(const ray<3> viewRay) const {
    if(boundry->intersection(viewRay) < 0.0)
        return NULL;

    double closestTime = -1;
    Drawable *closestObject = NULL;
    for(unsigned int i = 0; i < objList.size(); i++) {
        if(objList[i]->intersection(viewRay) > 0.0) {
            double intersection = objList[i]->intersection(viewRay);
            if(closestTime < 0 || intersection < closestTime) {
                closestTime = intersection;
                closestObject = objList[i];
            }
        }
    }

    return closestObject;
}
