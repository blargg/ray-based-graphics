#include "triangle.h"
#include <assert.h>
#include <limits>
#include "core/common.h"
#include "shapes/triBoxOverlap.h"
#include "util/log.h"


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

Vector4d Triangle::normal_vector(const Vector4d &surface) const {
    double u, v;
    std::tie(u, v) = uvCoords(surface);
    Vector4d normal = (1 - u - v) * n1 + u * n2 + v * n3;
    normal.normalize();
    return normal;
}

Vector4d Triangle::normal(Vector4d x, Vector4d y, Vector4d z) const {
    Vector4d edge1 = y - x;
    Vector4d edge2 = z - x;
    Vector3d e1 = edge1.head(3);
    Vector3d e2 = edge2.head(3);
    Vector3d result = e1.cross(e2);
    return Vector4d(result(0), result(1), result(2), 0);
}

std::tuple<double, double> Triangle::uvCoords(const Vector4d &point) const {
    Vector4d edge1 = p2 - p1;
    Vector4d edge2 = p3 - p1;

    Vector4d h = cross(trueNormal, edge2);
    double a = edge1.dot(h);

    double f = 1/a;
    Vector4d s = point - p1;
    double u = f * s.dot(h);

    Vector4d q = cross(s, edge1);
    double v = f * q.dot(trueNormal);

    return std::make_tuple(u, v);
}

Triangle::Triangle():Shape(),p1(0,0,0,1),p2(1,0,0,1),p3(1,1,0,1) {
    trueNormal = n1 = n2 = n3 = normal(p1, p2, p3);
}

Triangle::Triangle(Vector4d first, Vector4d second, Vector4d third):Shape(),p1(first),p2(second),p3(third) {
    trueNormal = n1 = n2 = n3 = normal(p1, p2, p3);
}

Triangle::Triangle(Vector4d first, Vector4d second, Vector4d third,
        Vector4d norm1, Vector4d norm2, Vector4d norm3)
    :Shape(), p1(first), p2(second), p3(third), n1(norm1), n2(norm2), n3(norm3) {
    trueNormal = normal(p1, p2, p3);
}

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

Vector4d Triangle::randomSurfacePoint() const {
    double u = (double) rand() / (double) RAND_MAX;
    double v = (double) rand() / (double) RAND_MAX;

    if(u > 1.0 - v) {
        u = 1.0 - u;
        v = 1.0 - v;
    }

    Vector4d e1, e2;
    e1 = p2 - p1;
    e2 = p3 - p1;
    return e1 * u + e2 * v;
}

Shape* Triangle::create() const{
    return new Triangle();
}

Shape* Triangle::clone() const{
    return new Triangle(*this);
}
#undef EPSILON
