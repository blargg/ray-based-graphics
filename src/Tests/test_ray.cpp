#include <gtest/gtest.h>

#include "core/ray.h"
#include <Eigen/Dense>

using namespace Eigen;

TEST(RayTest, value_at_distance) {
    ray a;
    a.dir = Vector4d(1.0,1.0,1.0,0.0);
    a.orig = Vector4d(0,0,0,1);

    Vector4d test = a(1.0);
    Vector4d expect(1,1,1,1);
    EXPECT_DOUBLE_EQ(test(0),expect(0));
    EXPECT_DOUBLE_EQ(test(1),expect(1));
    EXPECT_DOUBLE_EQ(test(2),expect(2));
    EXPECT_DOUBLE_EQ(test(3),expect(3));

    a.orig = Vector4d(100,1.5,0,1);
    a.dir  = Vector4d(20, 4, 0, 0);
    test = a(2.0);
    expect = Vector4d(140,9.5,0,1);
    EXPECT_DOUBLE_EQ(test(0),expect(0));
    EXPECT_DOUBLE_EQ(test(1),expect(1));
    EXPECT_DOUBLE_EQ(test(2),expect(2));
    EXPECT_DOUBLE_EQ(test(3),expect(3));
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
