#include <gtest/gtest.h>

#include "../shapes/plane.h"

namespace{
class PlaneTest : public ::testing::Test{
    protected:
        Plane plane1;

        virtual void SetUp(){
            plane1 = Plane(
                    point<3>(0, 0, 0),
                    vectre<3>(0, 1.0, 0));
        }

        virtual void TearDown(){
            // do nothing
        }
};//PlaneTest

TEST_F(PlaneTest, intersects){
    ray<3> testRay;
    testRay.orig = point<3>(0, 10, 0);
    testRay.dir = vectre<3>(0, -1,0);
    EXPECT_TRUE(plane1.intersects(testRay)) << "Direct intersection";

    testRay.orig = point<3>(10, 10, -340.5);
    testRay.dir = vectre<3>(0, 1, 0);
    EXPECT_FALSE(plane1.intersects(testRay)) << "Ray faces oposite direction";

    testRay.orig = point<3>(10, 10, -340.5);
    testRay.dir = vectre<3>(1, 0, 0);
    EXPECT_FALSE(plane1.intersects(testRay)) << "Ray is parallel";
}

TEST_F(PlaneTest, intersection_point){
    ray<3> testRay;
    testRay.orig = point<3>(0, 10, 0);
    testRay.dir = vectre<3>(0, -1,0);
    EXPECT_DOUBLE_EQ(plane1.intersection(testRay), 10.0) << "Direct intersection";
}

TEST_F(PlaneTest, normals){
    vectre<3> normal = plane1.normal_vectre(point<3>(0.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(normal[0], 0);
    EXPECT_DOUBLE_EQ(normal[1], 1.0);
    EXPECT_DOUBLE_EQ(normal[2], 0);
}

}//namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
