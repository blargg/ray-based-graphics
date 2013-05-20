#include <gtest/gtest.h>

#include "../shapes/plane.h"
#include <Eigen/Dense>

namespace{
class PlaneTest : public ::testing::Test{
    protected:
        Plane plane1;

        virtual void SetUp(){
            plane1 = Plane(
                    Vector4d(0, 0, 0, 1),
                    Vector4d(0, 1.0, 0, 0));
        }

        virtual void TearDown(){
            // do nothing
        }
};//PlaneTest

TEST_F(PlaneTest, intersects){
    ray testRay;
    testRay.orig = Vector4d(0, 10, 0, 1);
    testRay.dir = Vector4d(0, -1,0, 0);
    EXPECT_GT(plane1.intersection(testRay), 0.0) << "Direct intersection";

    testRay.orig = Vector4d(10, 10, -340.5, 1);
    testRay.dir = Vector4d(0, 1, 0, 0);
    EXPECT_LT(plane1.intersection(testRay), 0.0) << "Ray faces oposite direction";

    testRay.orig = Vector4d(10, 10, -340.5, 1);
    testRay.dir = Vector4d(1, 0, 0, 0);
    EXPECT_LT(plane1.intersection(testRay), 0.0) << "Ray is parallel";
}

TEST_F(PlaneTest, intersection_point){
    ray testRay;
    testRay.orig = Vector4d(0, 10, 0, 1);
    testRay.dir = Vector4d(0, -1,0, 0);
    EXPECT_DOUBLE_EQ(plane1.intersection(testRay), 10.0) << "Direct intersection";
}

TEST_F(PlaneTest, normals){
    Vector4d normal = plane1.normal_vectre(Vector4d(0.0, 0.0, 0.0, 1));
    EXPECT_DOUBLE_EQ(normal[0], 0);
    EXPECT_DOUBLE_EQ(normal[1], 1.0);
    EXPECT_DOUBLE_EQ(normal[2], 0);
}

}//namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
