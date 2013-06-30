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

// does a cross product ignoring the forth value
Vector4d cross(Vector4d a, Vector4d b);

/* returns a perturbed (randomly offset) version of the given vector
 * Note: the original vector must be normalized
 *
 * a the original vector to perturb
 * max_angle the maximum angle that the resulting vector can make with the
 *   original (in radians)
 */
Vector4d perturb(Vector4d a, double max_angle);

/**
 * returns the max of the three given values
 */
template <class T>
T max3(T a, T b, T c) {
    if(a > b)
        return (a>c?a:c);
    else
        return (b>c?b:c);
}

#endif // RT_COMMON
