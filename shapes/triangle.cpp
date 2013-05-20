#include "triangle.h"
#include <assert.h>
#include <limits>
#include "../common.h"


/**
 * operating with 3-vectors here because there are a lot of cross products
 */
double Triangle::intersection(const ray &viewRay)const{
    Vector4d edge1 = p2 - p1;
    Vector4d edge2 = p3 - p1;

    Vector4d h = cross(viewRay.dir, edge2);
    double a = edge1.dot(h);

    if(a > -EPSILON && a < EPSILON)
        return -1 * std::numeric_limits<double>::max();


    double f = 1/a;
    Vector4d s = viewRay.orig - p1;
    double u = f * s.dot(h);

    if(u < 0.0 || u > 1.0)
        return -1 * std::numeric_limits<double>::max();

    Vector4d q = cross(s, edge1);
    double v = f * q.dot(viewRay.dir);

    if(v < 0.0 || u + v > 1.0)
        return -1 * std::numeric_limits<double>::max();

    double t = f * edge2.dot(q);

    return t;
}

Vector4d Triangle::normal_vectre(const Vector4d &surface)const{
    Vector4d edge1 = p2 - p1;
    Vector4d edge2 = p3 - p1;
    Vector3d e1 = edge1.head(3);
    Vector3d e2 = edge2.head(3);
    Vector3d result = e1.cross(e2);
    return Vector4d(result(0), result(1), result(2), 0);
}

Triangle::Triangle():Shape(),p1(0,0,0,1),p2(1,0,0,1),p3(1,1,0,1)
{ /* do nothing */ }

Triangle::Triangle(Vector4d first, Vector4d second, Vector4d third):Shape(),p1(first),p2(second),p3(third)
{ /* do nothing */ }

Shape* Triangle::create() const{
    return new Triangle();
}

Shape* Triangle::clone() const{
    return new Triangle(*this);
}
#undef EPSILON
