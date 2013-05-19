#include <gtest/gtest.h>

#include "../shapes/triangle.h"

namespace{
class TriangleTest : public ::testing::Test{
    protected:
    Triangle t1;
    Triangle t2;

    public:
    virtual void SetUp(){
        t1 = Triangle(point<3>(0.0, 0.0, 0.0),
                point<3>(1.0, 0.0, 0.0),
                point<3>(1.0, 1.0, 0.0));
        t2 = Triangle();
    }

    virtual void TearDown(){
        // do nothing
    }

};

TEST_F(TriangleTest, intersects){
    ray<3> testRay;
    testRay.orig = point<3>(0.5, 0.25, 1.0);
    testRay.dir = vectre<3>(0, 0, -1);
    EXPECT_GT(t1.intersection(testRay), 0.0) << "Intersects simple ray";

    testRay.orig = point<3>(0.9, 0.5, 10.0);
    testRay.dir = vectre<3>(0, 0, -1);
    EXPECT_GT(t1.intersection(testRay), 0.0) << "Should definatly intersect tringle";

    testRay.orig = point<3>(0.9, 0.5, 10.0);
    testRay.dir = vectre<3>(0, 0, 1);
    EXPECT_LT(t1.intersection(testRay), 0.0) << "Ray faces opposite direction";

    testRay.orig = point<3>(0.0, 1.5, 10.0);
    testRay.dir = vectre<3>(0, 0, -1);
    EXPECT_LT(t1.intersection(testRay), 0.0) << "Intersects the plane, but not triangle";

    testRay.orig = point<3>(0.0, 1.5, 10.0);
    testRay.dir = vectre<3>(1, 0, 0);
    EXPECT_LT(t1.intersection(testRay), 0.0) << "Parallel to the triangle";

    testRay.orig = point<3>(0,0,1);
    testRay.dir = vectre<3>(0.11, 0.1, -1).unit_vectre();
    EXPECT_GT(t1.intersection(testRay), 0.0) << "Angled intersection";
}

TEST_F(TriangleTest, intersection_point){
    ray<3> testRay;
    testRay.orig = point<3>(0.5, 0.25, 1.0);
    testRay.dir = vectre<3>(0, 0, -1);
    EXPECT_DOUBLE_EQ(t1.intersection(testRay), 1.0) <<
        "Straight at the Triangle";

    testRay.orig = point<3>(0.9, 0.5, 10.0);
    testRay.dir = vectre<3>(0, 0, -1);
    EXPECT_DOUBLE_EQ(t2.intersection(testRay), 10.0) <<
        "Straight at the Triangle";

    testRay.orig = point<3>(0.5, 0.25, -1.0);
    testRay.dir = vectre<3>(0, 0, 1);
    EXPECT_DOUBLE_EQ(t1.intersection(testRay), 1.0) <<
        "Straight at the Triangle";

}

TEST_F(TriangleTest, normals){
    vectre<3> normal = t1.normal_vectre(point<3>(1.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(normal[0], 0.0);
    EXPECT_DOUBLE_EQ(normal[1], 0.0);
    EXPECT_DOUBLE_EQ(normal[2], 1.0);

    normal = t1.normal_vectre(point<3>(0.0, 1.0, 0.0));
    EXPECT_DOUBLE_EQ(normal[0], 0.0);
    EXPECT_DOUBLE_EQ(normal[1], 0.0);
    EXPECT_DOUBLE_EQ(normal[2], 1.0);
}

} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
