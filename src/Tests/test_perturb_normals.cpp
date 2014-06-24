#include <gtest/gtest.h>

#include <cmath>

#include "shapes/perturb_normals.h"
#include "shapes/sphere.h"
#include "core/ray.h"

TEST(PerturbNormals, Intersection) {
    Sphere s(Vector4d(1,1,1,1), 10.0);
    PerturbNormals<Sphere> wrapped(Vector4d(1,1,1,1), 10.0);
    wrapped.setAngle(M_PI/8);

    // TODO better tests
    ray viewRay;
    viewRay.orig = Vector4d(1,1,-10,1);
    viewRay.dir = Vector4d(0,0,1,0);
    EXPECT_NEAR(wrapped.intersection(viewRay), 1.0, 0.001);

    viewRay.orig = Vector4d(100,1,-10,1);
    viewRay.dir = Vector4d(0,0,1,0);
    EXPECT_LT(wrapped.intersection(viewRay), 0.0);

    for (int i = 0; i < 1000; i++) {
        viewRay.orig = Vector4d(1.1,0.9,1.3,1);
        viewRay.dir = Vector4d::Random();
        viewRay.dir(3) = 0.0;
        viewRay.dir.normalize();

        double actualdist = s.intersection(viewRay);
        double measureddist = wrapped.intersection(viewRay);

        EXPECT_DOUBLE_EQ(actualdist, measureddist) << "The sphere and the perturbed sphere should have the same intersections";
    }
}

TEST(PerturbNormals, NormalVectors) {
    Sphere s(Vector4d(0,0,0,1), 1.0);
    PerturbNormals<Sphere> wrapped(Vector4d(0,0,0,1), 1.0);
    wrapped.setAngle(M_PI/8);

    Vector4d testPoint = Vector4d(1,0,0,1);
    Vector4d normal = s.normal_vector(testPoint);
    Vector4d perturbed_normal = wrapped.normal_vector(testPoint);

    EXPECT_TRUE(isUnitVector(normal)) << "result is not normalized";
    EXPECT_TRUE(isUnitVector(perturbed_normal)) << "result is not normalized";
    EXPECT_GT(normal.dot(perturbed_normal), cos(M_PI/8)) << "vectors must form a close angle with eachother";

    wrapped.setAngle(0);

    testPoint = Vector4d(1,0,0,1);
    normal = s.normal_vector(testPoint);
    perturbed_normal = wrapped.normal_vector(testPoint);

    EXPECT_TRUE(isUnitVector(normal)) << "result is not normalized";
    EXPECT_TRUE(isUnitVector(perturbed_normal)) << "result is not normalized";
    EXPECT_GT(normal.dot(perturbed_normal), cos(M_PI/8));

    for(int i = 0; i < 1000; i++) {
        wrapped.setAngle(M_PI/8);
        testPoint = Vector4d::Random();
        testPoint(3) = 0; testPoint.normalize(); testPoint(3) = 1.0;
        normal = s.normal_vector(testPoint);
        perturbed_normal = wrapped.normal_vector(testPoint);

        EXPECT_TRUE(isUnitVector(normal)) << "result is not normalized";
        EXPECT_TRUE(isUnitVector(perturbed_normal)) << "result is not normalized";
        EXPECT_GT(normal.dot(perturbed_normal), cos(M_PI/8));
    }
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
