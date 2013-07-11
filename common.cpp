#include "common.h"

Vector4d cross(Vector4d a, Vector4d b) {
    Vector3d result = a.head<3>().cross(b.head<3>());
    return Vector4d(result(0), result(1), result(2), 0);
}

Vector4d perturb(Vector4d original, double max_angle){
    Vector4d a = original;
    Vector4d up(0,1,0,0);
    if(a == up || a == (-1 * up))
        up = Vector4d(1,0,0,0);

    Vector4d right = cross(up, a);
    right.normalize();
    up = cross(right, a);

    double theta = ((double)rand() / (double)RAND_MAX) * max_angle;
    double thi = ((double)rand() / (double)RAND_MAX) * (2.0 * M_PI);
    double x = sin(theta) * cos(thi);
    double y = sin(theta) * sin(thi);
    double z = cos(theta);

    return (x * right + y * up + z * a);
}
