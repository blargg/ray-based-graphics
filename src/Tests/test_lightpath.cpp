#include <gtest/gtest.h>

#include "render/lightpath.h"

LightPath mkPath(int numPoints) {
    Vector4d light(0, 0, 0, 1);
    Vector4d cam((numPoints - 1) * 1.0, 0, 0, 1);
    Color emitted(1,1,1);

    vector<PathPoint> bounces;
    for (int i = 1; i < numPoints - 1; i++) {
        PathPoint bounce;
        bounce.location = Vector4d(i * 1.0, 0, 0, 1);
        bounce.normal = Vector4d(0, 1, 0, 0);
        bounce.shader = Diffuse;
        bounces.push_back(bounce);
    }
    return LightPath(light, emitted, bounces, cam);
}

TEST(LightPathTest, deleteSubpath) {
    LightPath p1 = mkPath(4);
    EXPECT_EQ(p1.size(), 4 - 2);

    LightPartialPath lightPath;
    CamPartialPath camPath;
    std::tie(lightPath, camPath) = p1.deleteSubpath(-2, 4);
    EXPECT_FALSE(camPath.exists) << "the camera path should be empty";
    EXPECT_FALSE(lightPath.exists) << "the light path should be empty";

    std::tie(lightPath, camPath) = p1.deleteSubpath(-1, 4);
    EXPECT_FALSE(camPath.exists) << "the camera path should exist";
    EXPECT_TRUE(lightPath.exists) << "the light path should be empty";
    EXPECT_EQ(lightPath.bounces.size(), 0) << "light path should have no bounces";

    std::tie(lightPath, camPath) = p1.deleteSubpath(-1, 3);
    EXPECT_TRUE(camPath.exists) << "the camera path should exist";
    EXPECT_TRUE((camPath.cameraLocation.dir - Vector4d(-1, 0, 0, 0)).squaredNorm() < 0.0001)
            << "camPath should point in the direction of the next bounce";
    EXPECT_EQ(camPath.bounces.size(), 0) << "light path should have no bounces";
    EXPECT_TRUE(lightPath.exists) << "the light path should be empty";
    EXPECT_EQ(lightPath.bounces.size(), 0) << "light path should have no bounces";

    // should not delete any points, just the connection between the light and
    // the remainder of the path
    std::tie(lightPath, camPath) = p1.deleteSubpath(-1, 0);
    EXPECT_EQ(lightPath.bounces.size(), 0) << "light path should have no bounces";
    EXPECT_EQ(camPath.bounces.size(), 2) << "cam path should have 2 bounces";
    // the camera path should be in reverse order of the light path
    for (int i = 0; i < camPath.bounces.size(); i++) {
        EXPECT_EQ(camPath.bounces[i].location[0], 2 - i);
    }

    std::tie(lightPath, camPath) = p1.deleteSubpath(0, 1);
    EXPECT_EQ(lightPath.bounces.size(), 1) << "light path should have 1 bounce";
    EXPECT_EQ(camPath.bounces.size(), 1) << "cam path should have 1 bounce";
}

TEST(LightPath, constructor) {
    LightPartialPath lightPath;
    CamPartialPath camPath;

    lightPath.lightLocation = Vector4d(0, 0, 0, 1);
    lightPath.emitted = Color(1, 0.5, 0.333);

    camPath.cameraLocation.orig = Vector4d(1, 0, 0, 1);
    camPath.bounces.clear();
    for (int i = 0; i < 4; i++) {
        PathPoint tmpPoint;
        tmpPoint.location = Vector4d((double) i / 4, 0, 0, 1);
        camPath.bounces.push_back(tmpPoint);
    }

    LightPath fullPath(lightPath, camPath);
    for (int i = 0; i < 4; i++) {
        PathPoint p = fullPath.getPoint(i);
        EXPECT_DOUBLE_EQ((3.0 - i) / 4, p.location(0)) << "The list should be in reverse order";
    }
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
