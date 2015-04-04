#ifndef SRC_CORE_COMMON_H_
#define SRC_CORE_COMMON_H_

/**
 * A set of common functions and macros.
 * Included by almost every file.
 */

#include <Eigen/Dense>
#include <cmath>
#include "util/debug.h"
using namespace Eigen;


#define EPSILON 0.000000001


/// does a cross product ignoring the forth value
Vector4d cross(Vector4d a, Vector4d b);

/**
 * Calculates the reflected vector.
 * The viewVector is heading into the surface,
 * the returned vector will be heading out of the surface
 *
 * @pre the given vectors are normalized
 * @post the vector returned is normalized
 * @param viewVector the vector viewing the Surface, heading into the surface
 * @param normal the normal of the surface to reflect over
 * @returns the reflected vector
 */
Vector4d reflectVector(Vector4d viewVector, Vector4d normal);

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
    if (a > b)
        return (a > c?a:c);
    else
        return (b > c?b:c);
}

template <typename T>
T vmax(T x) {
    return x;
}

template <typename T, typename...Args>
T vmax(T x, Args... xs) {
    if (x > vmax(xs...))
        return x;
    return vmax(xs...);
}

/**
 * returns the minimum of the three given values
 */
template <class T>
T min3(T a, T b, T c) {
    if (a > b)
        return (b < c?b:c);
    else
        return (a < c?a:c);
}

template <class T>
T randomRange(T lower, T upper) {
    ASSERT(lower < upper,
            "The lower bound should be lower than the upper bound");
    auto diff = upper - lower;
    return diff * (((T) rand()) / ((T)(RAND_MAX))) + lower;
}

int randomRangeInt(int lower, int upper);

// generates a random integer
// 1 with prob 0.25
// 2 with prob 0.5
// u > 3, with prob 2^(-u)
int skewedGeometricRandom();

#endif  // SRC_CORE_COMMON_H_
