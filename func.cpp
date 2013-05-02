
#include "func.h"

point<3> add(point<3> p, vectre<3> vec) {
    point<3> ret(p[0] + vec[0], p[1] + vec[1], p[2] + vec[2]);
    return ret;
}

point<3> sub(point<3> p, vectre<3> vec) {
    return point<3>(p[0] - vec[0], p[1] - vec[1], p[2] - vec[2]);
}

