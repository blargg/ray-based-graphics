#include "common.h"

Vector4d cross(Vector4d a, Vector4d b) {
    Vector3d first = a.head(3);
    Vector3d second = b.head(3);
    Vector3d result = first.cross(second);
    return Vector4d(result(0), result(1), result(2), 0);
}
