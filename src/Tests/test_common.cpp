#include <gtest/gtest.h>

#include "core/common.h"
#include <Eigen/Dense>

TEST(CommonTest, cross) {
    Vector4d res = cross(Vector4d(1,0,0,0), Vector4d(0,1,0,0));
    EXPECT_TRUE(res == Vector4d(0,0,1,0));

    res = cross(Vector4d(1,2,3,0), Vector4d(4,5,6,0));
    EXPECT_TRUE(res == Vector4d(-3,6,-3,0));

    res = cross(Vector4d(-1,0,2,0), Vector4d(1,5,-1,0));
    EXPECT_TRUE(res == Vector4d(-10,1,-5,0));
}

// TODO test reflectVector

TEST(CommonTest, isUnitVector) {
    Vector4d v(1,0,0,0);
    EXPECT_TRUE(isUnitVector<Vector4d>(v)) << "v is a unit vector";
    EXPECT_FALSE(isUnitVector<Vector3d>(Vector3d(1,1,1))) << "Vector3d:: that is not a unit vector";
    EXPECT_FALSE(isUnitVector<Vector3d>(Vector3d(1,0,-1))) << "Vector3d:: that is not a unit vector";
    EXPECT_FALSE(isUnitVector<Vector3d>(Vector3d(1,0,.001))) << "Vector3d:: that is not a unit vector";

    for(int i = 0; i < 100; i++) {
        Vector3d a = Vector3d::Random();
        a.normalize();
        EXPECT_TRUE(isUnitVector<Vector3d>(a)) << "random normalized vector";
    }
}

TEST(CommonTest, perturb) {
    Vector4d res = perturb(Vector4d(1,0,0,0), M_PI/2.0);
    EXPECT_GE(res(0), 0.0);
    EXPECT_TRUE(isUnitVector<Vector4d>(res)) << "result is not normalized";

    int i = 0;
    Vector4d original;
    for(i = 0; i < 100; i++) {
        original = Vector4d::Random();
        original[3] = 0.0;
        original.normalize();
        res = perturb(original, M_PI/2.0);
        EXPECT_LE(res.dot(original), 1.0) << "res or original to large?";
        EXPECT_GE(res.dot(original), 0.0) << "angle over the limit.";
        EXPECT_TRUE(isUnitVector<Vector4d>(res)) << "result is not normalized";
    }
}

TEST(CommonTest, max3) {
    EXPECT_EQ(max3<int>(10,1,2), 10) << "fail on ints";
    EXPECT_DOUBLE_EQ(max3<double>(1.001, 2.02, 2.1), 2.1) << "fail on double";
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
