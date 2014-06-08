#include <gtest/gtest.h>

#include "shapes/sphere.h"
#include "aabb.h"

namespace{
class SphereTest : public ::testing::Test{
    protected:
    Sphere unit_sphere;
    Sphere s2;

    public:
    virtual void SetUp(){
        unit_sphere = Sphere(Vector4d(0.0, 0.0, 0.0, 1), 1);
        s2 = Sphere(Vector4d(100.0, 10.5, 0.0, 1), 100);
    }

    virtual void TearDown(){
        // do nothing
    }

};

TEST_F(SphereTest, intersects){
    ray testRay;
    testRay.orig = Vector4d(-10, 0.0, 0.0, 1);
    testRay.dir = Vector4d(1, 0, 0, 0);
    EXPECT_GT(unit_sphere.intersection(testRay), 0.0) << "Intersects simple ray";

    testRay.orig = Vector4d(-10, 0.5, 0.0, 1);
    testRay.dir = Vector4d(1, 0, 0, 0);
    EXPECT_GT(unit_sphere.intersection(testRay), 0.0) << "Intersects at a skew angle";

    testRay.orig = Vector4d(-10, 0.99, 0.0, 1);
    testRay.dir = Vector4d(1, 0, 0, 0);
    EXPECT_GT(unit_sphere.intersection(testRay), 0.0) << "Intersects at edge";

    testRay.orig = Vector4d(-10, 0.0, 0.0, 1);
    testRay.dir = Vector4d(0, 1, 0, 0);
    EXPECT_LT(unit_sphere.intersection(testRay), 0.0) << "Line doesn't intersect the ray";

    testRay.orig = Vector4d(-10, 0.0, 0.0, 1);
    testRay.dir = Vector4d(-1, 0, 0, 0);
    EXPECT_LT(unit_sphere.intersection(testRay), 0.0) << "Sphere lies behind the ray origin";

    testRay.orig = Vector4d(0.0, 0.0, 0.0, 1);
    testRay.dir = Vector4d(-1, 0, 0, 0);
    EXPECT_GT(unit_sphere.intersection(testRay), 0.0) << "Ray originates inside the sphere";
}

TEST_F(SphereTest, intersection_point){
    ray testRay;
    testRay.orig = Vector4d(-10, 0.0, 0.0, 1);
    testRay.dir = Vector4d(1, 0.0, 0.0, 0);
    EXPECT_DOUBLE_EQ(unit_sphere.intersection(testRay), 9.0) <<
        "Intersection should be about 9 units away";

    testRay.orig = Vector4d(-1.0, 10.5, 0.0, 1);
    testRay.dir = Vector4d(1, 0, 0, 0);
    EXPECT_DOUBLE_EQ(s2.intersection(testRay), 1.0) <<
        "Should work at spheres not at the origin";
}

TEST_F(SphereTest, normals){
    Vector4d normal = unit_sphere.normal_vector(Vector4d(1.0, 0.0, 0.0, 1));
    EXPECT_DOUBLE_EQ(normal(0), 1.0);
    EXPECT_DOUBLE_EQ(normal(1), 0.0);
    EXPECT_DOUBLE_EQ(normal(2), 0.0);

    normal = unit_sphere.normal_vector(Vector4d(0.0, 1.0, 0.0, 1));
    EXPECT_DOUBLE_EQ(normal(0), 0.0);
    EXPECT_DOUBLE_EQ(normal(1), 1.0);
    EXPECT_DOUBLE_EQ(normal(2), 0.0);
}

TEST_F(SphereTest, intersectsAABB) {
    AABB box;
    box.minCorner = Vector3d(-2,-2,-2);
    box.maxCorner = Vector3d(2,2,2);
    EXPECT_TRUE(unit_sphere.intersectsBox(box)) << "box completely holds shape";

    box.minCorner = Vector3d(0,0,0);
    box.maxCorner = Vector3d(2,2,2);
    EXPECT_TRUE(unit_sphere.intersectsBox(box)) << "box partial overlap";

    box.minCorner = Vector3d(0,0,0);
    box.maxCorner = Vector3d(.2,.2,.2);
    EXPECT_TRUE(unit_sphere.intersectsBox(box)) << "box inside shape";

    box.minCorner = Vector3d(.9,.9,.9);
    box.maxCorner = Vector3d(3,3,3);
    EXPECT_FALSE(unit_sphere.intersectsBox(box)) << "intersects sphere's bounding box, but not sphere";

    box.minCorner = Vector3d(2,2,2);
    box.maxCorner = Vector3d(3,3,3);
    EXPECT_FALSE(unit_sphere.intersectsBox(box)) << "clearly no intersection";
}

} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
