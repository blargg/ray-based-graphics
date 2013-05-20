#ifndef RT_COMMON
#define RT_COMMON

/**
 * A set of common functions and macros.
 * Included by almost every file.
 */

#include <Eigen/Dense>
using namespace Eigen;


#define NDEBUG
#define EPSILON 0.000000001

Vector4d cross(Vector4d a, Vector4d b);

#endif // RT_COMMON
