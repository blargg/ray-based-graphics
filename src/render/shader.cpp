
#include "render/shader.h"
#include "core/common.h"
#include "util/debug.h"

double geometric(Vector4d s1, Vector4d n1, Vector4d s2, Vector4d n2) {
    ASSERT(isUnitVector(n1), "assumes the normals are normalized");
    ASSERT(isUnitVector(n2), "assumes the normals are normalized");
    Vector4d segment = s1 - s2;
    Vector4d seg_dir = segment.normalized();
    double cos1 = n1.dot(seg_dir);
    double cos2 = n2.dot(-1 * seg_dir);

    return std::abs(cos1 * cos2) / segment.squaredNorm();
}
