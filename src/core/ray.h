/**
 * @brief A ray object (point and vectre)
 *
 * contains a point and a vectre
 * has several methods to help manage them
 */
#ifndef SRC_CORE_RAY_H_
#define SRC_CORE_RAY_H_

#include <Eigen/Dense>

using namespace Eigen;

/**
 * Defines a ray (point and vector).
 */
class ray {
    public:
    /**
     * @brief Where the origin of the ray is.
     */
    Vector4d orig;
    /**
     * @brief The direction in which the ray is traveling.
     */
    Vector4d dir;

    /**
     * @brief Makes a ray with the default point and vectre objects
     *
     * Simply allows the default constructors for the point orig and the vectre
     * dir to be called
     */
    ray() { /* do nothing */ }

    /**
     * @brief Makes a new ray with the given origin and direction.
     *
     * @param origin Point where the ray starts.
     * @param direction Veter that represents where the ray is going.
     */
    ray(const Vector4d origin, const Vector4d direction);

    /**
     * @brief Gives the point at which the ray will be at the given time
     *
     * Starting at the Point orig and moving in the direction dir, where the
     * speed is one lenght of vecter dir per second, returns the point location of
     * the ray at that time
     *
     * @param time The time when you want to know the ray's location.
     * @returns the point where the ray is.
     */
    Vector4d operator()(const double time) const;

    /**
     * Takes a point in space, and projects it onto the plane determined by this
     * ray
     *
     * The plane contains the point orig, and its normal is the ray's vector
     * dir.
     */
    Vector4d projectOnToPlane(const Vector4d point) const;
};


#endif  // SRC_CORE_RAY_H_
