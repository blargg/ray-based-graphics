#include "triangle.h"
#include <assert.h>
#include <limits>

#define EPSILON 0.0000001

double Triangle::intersection(const ray<3> &viewRay)const{
    vectre<3> edge1(p1, p2);
    vectre<3> edge2(p1, p3);

    vectre<3> h = viewRay.dir.cross_prod(edge2);
    double a = edge1.dot_prod(h);

    if(a > -EPSILON && a < EPSILON)
        return -1 * std::numeric_limits<double>::max();


    double f = 1/a;
    vectre<3>s(p1, viewRay.orig);
    double u = f * s.dot_prod(h);

    if(u < 0.0 || u > 1.0)
        return -1 * std::numeric_limits<double>::max();

    vectre<3> q = s.cross_prod(edge1);
    double v = f * q.dot_prod(viewRay.dir);

    if(v < 0.0 || u + v > 1.0)
        return -1 * std::numeric_limits<double>::max();

    double t = f * edge2.dot_prod(q);

    return t;
}

vectre<3> Triangle::normal_vectre(const point<3> &surface)const{
    vectre<3> edge1(p1, p2);
    vectre<3> edge2(p1, p3);
    return edge1.cross_prod(edge2);
}

Triangle::Triangle():Shape(point<3>(0,0,0)),p1(0,0,0),p2(1,0,0),p3(1,1,0)
{ /* do nothing */ }

Triangle::Triangle(point<3> first, point<3> second, point<3> third):Shape(first),p1(first),p2(second),p3(third)
{ /* do nothing */ }

Shape* Triangle::create() const{
    return new Triangle();
}

Shape* Triangle::clone() const{
    return new Triangle(*this);
}
#undef EPSILON
