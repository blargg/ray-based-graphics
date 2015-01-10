#ifndef SRC_SHAPES_TRIANGLE_H_
#define SRC_SHAPES_TRIANGLE_H_

#include <Eigen/Dense>
#include "shapes/shape.h"
#include "core/ray.h"
#include "core/common.h"

/**
 * Triangle shape.
 */
class Triangle: public Shape {
 protected:
    Vector4d p1;
    Vector4d p2;
    Vector4d p3;

    // the true geometric normal of the triangle
    Vector4d trueNormal;
    // normals for interpolation
    Vector4d n1;
    Vector4d n2;
    Vector4d n3;

 public:
    Triangle();

    /**
     * Constructs a triangle with verticies at the given points.
     */
    Triangle(Vector4d first, Vector4d second, Vector4d third);

    /**
     * Construct a triangle based on the three points and the normals at those
     * points
     */
    Triangle(Vector4d first, Vector4d second, Vector4d third,
        Vector4d norm1, Vector4d norm2, Vector4d norm3);

    /**
     * virtual default constructor.
     */
    virtual Shape* create() const;

    /**
     * virtual copy constructor.
     */
    virtual Shape* clone() const;

    /**
     * Cleans up and removes the triangle.
     */
    virtual ~Triangle() {/* do nothing */}

    /**
     * Calculates the intersection of the ray and triangle.
     */
    virtual double intersection(const ray& viewRay) const;

    /**
     * Returns the normal vector for the triangle.
     * @param surface a point on the surface
     * @returns a Vector4d for the normal vector.
     */
    virtual Vector4d normal_vector(const Vector4d& surface) const;

    virtual double getMinBound(int axis) const;
    virtual double getMaxBound(int axis) const;
    virtual bool intersectsBox(AABB box) const;

    virtual Vector4d randomSurfacePoint() const;

 private:
    std::tuple<double, double> uvCoords(const Vector4d &point) const;

    /**
     * calculate the normal of a triangle defined by the points x, y, z
     */
    Vector4d normal(Vector4d x, Vector4d y, Vector4d z) const;
};

#endif  // SRC_SHAPES_TRIANGLE_H_
