#include <gtest/gtest.h>

#include "../shapes/sphere.h"

namespace{
class SphereTest : public ::testing::Test{
    protected:
    Sphere unit_sphere;
    Sphere s2;

    public:
    virtual void SetUp(){
        unit_sphere = Sphere(point<3>(0.0, 0.0, 0.0), 1);
        s2 = Sphere(point<3>(100.0, 10.5, 0.0), 100);
    }

    virtual void TearDown(){
        // do nothing
    }

};

TEST_F(SphereTest, intersects){
    ray<3> testRay;
    testRay.orig = point<3>(-10, 0.0, 0.0);
    testRay.dir = vectre<3>(1, 0, 0);
    EXPECT_TRUE(unit_sphere.intersects(testRay)) << "Intersects simple ray";

    testRay.orig = point<3>(-10, 0.5, 0.0);
    testRay.dir = vectre<3>(1, 0, 0);
    EXPECT_TRUE(unit_sphere.intersects(testRay)) << "Intersects at a skew angle";

    testRay.orig = point<3>(-10, 0.99, 0.0);
    testRay.dir = vectre<3>(1, 0, 0);
    EXPECT_TRUE(unit_sphere.intersects(testRay)) << "Intersects at edge";

    testRay.orig = point<3>(-10, 0.0, 0.0);
    testRay.dir = vectre<3>(0, 1, 0);
    EXPECT_FALSE(unit_sphere.intersects(testRay)) << "Line doesn't intersect the ray";

    testRay.orig = point<3>(-10, 0.0, 0.0);
    testRay.dir = vectre<3>(-1, 0, 0);
    EXPECT_FALSE(unit_sphere.intersects(testRay)) << "Sphere lies behind the ray origin";

    testRay.orig = point<3>(0.0, 0.0, 0.0);
    testRay.dir = vectre<3>(-1, 0, 0);
    EXPECT_TRUE(unit_sphere.intersects(testRay)) << "Ray originates inside the sphere";
}

TEST_F(SphereTest, intersection_point){
    ray<3> testRay;
    testRay.orig = point<3>(-10, 0.0, 0.0);
    testRay.dir = vectre<3>(1, 0.0, 0.0);
    EXPECT_DOUBLE_EQ(unit_sphere.intersection(testRay), 9.0) <<
        "Intersection should be about 9 units away";

    testRay.orig = point<3>(-1.0, 10.5, 0.0);
    testRay.dir = vectre<3>(1, 0, 0);
    EXPECT_DOUBLE_EQ(s2.intersection(testRay), 1.0) <<
        "Should work at spheres not at the origin";
}

TEST_F(SphereTest, normals){
    vectre<3> normal = unit_sphere.normal_vectre(point<3>(1.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(normal[0], 1.0);
    EXPECT_DOUBLE_EQ(normal[1], 0.0);
    EXPECT_DOUBLE_EQ(normal[2], 0.0);

    normal = unit_sphere.normal_vectre(point<3>(0.0, 1.0, 0.0));
    EXPECT_DOUBLE_EQ(normal[0], 0.0);
    EXPECT_DOUBLE_EQ(normal[1], 1.0);
    EXPECT_DOUBLE_EQ(normal[2], 0.0);
}

} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
