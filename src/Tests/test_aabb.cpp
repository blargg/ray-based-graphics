#include <gtest/gtest.h>

#include <Eigen/Dense>
#include <math.h>
#include "kdtree/aabb.h"
#include "core/ray.h"

TEST(AABBTest, surfaceArea) {
    AABB box;
    box.minCorner = Vector3d(0,0,0);
    box.maxCorner = Vector3d(1,1,1);
    EXPECT_DOUBLE_EQ(6.0, box.surfaceArea()) << "1x1x1 box";

    box.minCorner = Vector3d(0,-1,-2);
    box.maxCorner = Vector3d(1,1,1);
    EXPECT_DOUBLE_EQ(22.0, box.surfaceArea()) << "shaped box";
}

TEST(AABBTest, intersection) {
    double tMin, tMax;
    AABB box;
    box.minCorner = Vector3d(0,0,0);
    box.maxCorner = Vector3d(1,1,1);

    ray testRay;
    testRay.orig = Vector4d(0.5,0.5,0.5,1);
    testRay.dir = Vector4d(1,0,0,0);
    EXPECT_TRUE(box.intersection(testRay, tMin, tMax)) << "Ray inside the box";
    EXPECT_DOUBLE_EQ(-0.5, tMin) << "first intersection behind the ray";
    EXPECT_DOUBLE_EQ(0.5, tMax) << "Second intersection infront of ray";

    testRay.orig = Vector4d(-1,0.5,0.5,1);
    testRay.dir = Vector4d(1,0,0,0);
    EXPECT_TRUE(box.intersection(testRay, tMin, tMax)) << "Ray inside the box";
    EXPECT_DOUBLE_EQ(1, tMin) << "first intersection behind the ray";
    EXPECT_DOUBLE_EQ(2, tMax) << "Second intersection infront of ray";

    testRay.orig = Vector4d(0.5,0.5,0.5,1);
    testRay.dir = Vector4d(0,1,0,0);
    EXPECT_TRUE(box.intersection(testRay, tMin, tMax)) << "Ray inside the box";
    EXPECT_DOUBLE_EQ(-0.5, tMin);
    EXPECT_DOUBLE_EQ(0.5, tMax);

    testRay.orig = Vector4d(0.5,0.5,0.5,1);
    testRay.dir = Vector4d(0,0,1,0);
    EXPECT_TRUE(box.intersection(testRay, tMin, tMax)) << "Ray inside the box";
    EXPECT_DOUBLE_EQ(-0.5, tMin);
    EXPECT_DOUBLE_EQ(0.5, tMax);

    testRay.orig = Vector4d(0,0,0.5,1);
    testRay.dir = Vector4d(1,1,0,0).normalized();
    EXPECT_TRUE(box.intersection(testRay, tMin, tMax)) << "Ray inside the box";
    EXPECT_DOUBLE_EQ(0, tMin);
    EXPECT_DOUBLE_EQ(sqrt(2.0), tMax);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
