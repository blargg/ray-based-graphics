#include <gtest/gtest.h>

#include "shapes/triangle.h"
#include "core/common.h"
#include "kdtree/aabb.h"
#include <Eigen/Dense>

namespace{
class TriangleTest : public ::testing::Test{
    protected:
    Triangle t1;
    Triangle t2;

    public:
    virtual void SetUp(){
        t1 = Triangle(Vector4d(0.0, 0.0, 0.0, 0.0),
                Vector4d(1.0, 0.0, 0.0, 0.0),
                Vector4d(1.0, 1.0, 0.0, 0.0));
        t2 = Triangle();
    }

    virtual void TearDown(){
        // do nothing
    }

};

TEST_F(TriangleTest, intersects){
    ray testRay;
    testRay.orig = Vector4d(0.5, 0.25, 1.0, 1);
    testRay.dir = Vector4d(0, 0, -1, 0);
    EXPECT_GT(t1.intersection(testRay), 0.0) << "Intersects simple ray";

    testRay.orig = Vector4d(0.9, 0.5, 10.0, 1);
    testRay.dir = Vector4d(0, 0, -1, 0);
    EXPECT_GT(t1.intersection(testRay), 0.0) << "Should definatly intersect tringle";

    testRay.orig = Vector4d(0.9, 0.5, 10.0, 1);
    testRay.dir = Vector4d(0, 0, 1, 0);
    EXPECT_LT(t1.intersection(testRay), 0.0) << "Ray faces opposite direction";

    testRay.orig = Vector4d(0.0, 1.5, 10.0, 1);
    testRay.dir = Vector4d(0, 0, -1, 0);
    EXPECT_LT(t1.intersection(testRay), 0.0) << "Intersects the plane, but not triangle";

    testRay.orig = Vector4d(0.0, 1.5, 10.0, 1);
    testRay.dir = Vector4d(1, 0, 0, 0);
    EXPECT_LT(t1.intersection(testRay), 0.0) << "Parallel to the triangle";

    testRay.orig = Vector4d(0,0,1, 1);
    testRay.dir = Vector4d(0.11, 0.1, -1, 0).normalized();
    EXPECT_GT(t1.intersection(testRay), 0.0) << "Angled intersection";
}

TEST_F(TriangleTest, intersection_point){
    ray testRay;
    testRay.orig = Vector4d(0.5, 0.25, 1.0, 1);
    testRay.dir = Vector4d(0, 0, -1, 0);
    EXPECT_DOUBLE_EQ(t1.intersection(testRay), 1.0) <<
        "Straight at the Triangle";

    testRay.orig = Vector4d(0.9, 0.5, 10.0, 1);
    testRay.dir = Vector4d(0, 0, -1, 0);
    EXPECT_DOUBLE_EQ(t2.intersection(testRay), 10.0) <<
        "Straight at the Triangle";

    testRay.orig = Vector4d(0.5, 0.25, -1.0, 1);
    testRay.dir = Vector4d(0, 0, 1, 0);
    EXPECT_DOUBLE_EQ(t1.intersection(testRay), 1.0) <<
        "Straight at the Triangle";

}

TEST_F(TriangleTest, normals){
    Vector4d normal = t1.normal_vector(Vector4d(1.0, 0.0, 0.0, 1));
    EXPECT_DOUBLE_EQ(normal[0], 0.0);
    EXPECT_DOUBLE_EQ(normal[1], 0.0);
    EXPECT_DOUBLE_EQ(normal[2], 1.0);

    normal = t1.normal_vector(Vector4d(0.0, 1.0, 0.0, 0));
    EXPECT_DOUBLE_EQ(normal[0], 0.0);
    EXPECT_DOUBLE_EQ(normal[1], 0.0);
    EXPECT_DOUBLE_EQ(normal[2], 1.0);
}

TEST_F(TriangleTest, intersectsAABB) {
    AABB box;

    box.minCorner = Vector3d(-1,-1,-1);
    box.maxCorner = Vector3d(2,2,2);
    EXPECT_TRUE(t1.intersectsBox(box)) << "Box entirely contains triangle";

    box.minCorner = Vector3d(0,0,-1);
    box.maxCorner = Vector3d(2,2,2);
    EXPECT_TRUE(t1.intersectsBox(box)) << "Partial intersection";

    box.minCorner = Vector3d(0,.6,-1);
    box.maxCorner = Vector3d(.5,1,2);
    EXPECT_FALSE(t1.intersectsBox(box)) << "Close miss";

    box.minCorner = Vector3d(3,3,3);
    box.maxCorner = Vector3d(4,4,4);
    EXPECT_FALSE(t1.intersectsBox(box)) << "No intersection";

    box.minCorner = Vector3d(0,0,0);
    box.maxCorner = Vector3d(1,1,1);
    EXPECT_TRUE(t1.intersectsBox(box)) << "Triangle on face of box intersects";

    box.minCorner = Vector3d(0,0,0.000001);
    box.maxCorner = Vector3d(1,1,1);
    EXPECT_FALSE(t1.intersectsBox(box)) << "Triangle just misses the face of the box";

    box.minCorner = Vector3d(0,0,-1);
    box.maxCorner = Vector3d(1,1,-0.0000001);
    EXPECT_FALSE(t1.intersectsBox(box)) << "Triangle just misses the face of the box";
}

} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
