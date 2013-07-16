#ifndef RT_COMMON
#define RT_COMMON

/**
 * A set of common functions and macros.
 * Included by almost every file.
 */

#include <Eigen/Dense>
using namespace Eigen;


#define EPSILON 0.000000001


/// does a cross product ignoring the forth value
Vector4d cross(Vector4d a, Vector4d b);

/**
 * returns a perturbed (randomly offset) version of the given vector
 *
 * @pre the original vector must be normalized
 * @param a the original vector to perturb
 * @param max_angle the maximum angle that the resulting vector can make with the
 *   original (in radians)
 * @returns a unit lenth vector perturb by at most max_angle
 */
Vector4d perturb(Vector4d a, double max_angle);

template <class T>
inline bool isUnitVector(T v) {
    float length = v.norm();
    return length > 1 - EPSILON && length < 1 + EPSILON;
}

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

/**
 * returns the minimum of the three given values
 */
template <class T>
T min3(T a, T b, T c) {
    if(a > b)
        return (b < c?b:c);
    else
        return (a < c?a:c);
}

#endif // RT_COMMON
