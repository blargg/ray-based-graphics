#include "core/common.h"
#include <cmath>

Vector4d cross(Vector4d a, Vector4d b) {
    Vector3d result = a.head<3>().cross(b.head<3>());
    return Vector4d(result(0), result(1), result(2), 0);
}

Vector4d reflectVector(Vector4d viewVector, Vector4d normal) {
    return viewVector - 2.0 * viewVector.dot(normal) * normal;
}

Vector4d perturb(Vector4d original, double max_angle) {
    Vector4d a = original;
    Vector4d up(0, 1, 0, 0);
    if (a == up || a == (-1 * up))
        up = Vector4d(1, 0, 0, 0);

    Vector4d right = cross(up, a);
    right.normalize();
    up = cross(right, a);

    double u = ((double)rand() / (double)(RAND_MAX));
    double v = ((double)rand() / (double)(RAND_MAX));
    double theta = u * 2.0 * M_PI;
    double minRand = cos(max_angle);
    v = (v * (1 - minRand)) + minRand;
    double phi = acos(v);
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);

    return (x * right + y * up + z * a);
}

Vector4d randomUnitVector() {
    // TODO better test
    return perturb(Vector4d(1,0,0,0), 2.0 * M_PI);
}

int randomRangeInt(int lower, int upper) {
    ASSERT(lower <= upper,
            "The lower bound should be less than or equal to the upper bound");
    int diff = upper - lower + 1;
    return (rand() % diff) + lower;
}

int skewedGeometricRandom() {
    double u = randomRange<double>(0.0, 1.0);

    if (u > 0.5) return 2;
    if (u > 0.25) return 1;
    return floor(log(u) / log(0.5)) + 1;
}
