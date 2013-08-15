#include "triangle.h"
#include <assert.h>
#include <limits>
#include "../common.h"
#include "../triBoxOverlap.h"


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

Vector4d Triangle::normal_vector(const Vector4d &surface)const{
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

double Triangle::getMinBound(int axis) const {
    return min3<double>(p1(axis), p2(axis), p3(axis));
}

double Triangle::getMaxBound(int axis) const {
    return max3<double>(p1(axis), p2(axis), p3(axis));
}

bool Triangle::intersectsBox(AABB box) const {
    Vector3d bcenter = (box.minCorner + box.maxCorner) / 2.0;
    float boxcenter[3];
    boxcenter[0] = bcenter[0];
    boxcenter[1] = bcenter[1];
    boxcenter[2] = bcenter[2];
    Vector3d bhalfsize = (box.maxCorner - box.minCorner) / 2.0;
    bhalfsize += Vector3d::Constant(EPSILON * 1000);
    float boxhalfsize[3];
    boxhalfsize[0] = bhalfsize[0];
    boxhalfsize[1] = bhalfsize[1];
    boxhalfsize[2] = bhalfsize[2];
    float triverts[3][3];

    triverts[0][0] = p1[0];
    triverts[0][1] = p1[1];
    triverts[0][2] = p1[2];

    triverts[1][0] = p2[0];
    triverts[1][1] = p2[1];
    triverts[1][2] = p2[2];

    triverts[2][0] = p3[0];
    triverts[2][1] = p3[1];
    triverts[2][2] = p3[2];

    return triBoxOverlap(boxcenter, boxhalfsize, triverts);
}

Shape* Triangle::create() const{
    return new Triangle();
}

Shape* Triangle::clone() const{
    return new Triangle(*this);
}
#undef EPSILON
