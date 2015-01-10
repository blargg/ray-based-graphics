#include <gtest/gtest.h>

#include "file_loader/cam_loader.h"

TEST(cam_loader, loadCameraFromFile) {
    Camera c = loadCameraFromFile("cam_file.cam");

    // location
    EXPECT_FLOAT_EQ(c.position.orig[0], 4.0);
    EXPECT_FLOAT_EQ(c.position.orig[1], 5.0);
    EXPECT_FLOAT_EQ(c.position.orig[2], 6.0);

    // direction
    Vector4d expected = Vector4d(1.0, 0.5, 1.1, 0.0);
    expected.normalize();
    EXPECT_LT((c.position.dir - expected).norm(), 0.00001) << "direction vector";

    // up direction
    Vector4d expected_up = Vector4d(1, 2, 3, 0).normalized();
    EXPECT_FLOAT_EQ((c.up - expected_up).norm(), 0.0) << "up vector";

    // dimensions
    EXPECT_DOUBLE_EQ(c.worldWidth, 10) << "world width";
    EXPECT_DOUBLE_EQ(c.worldHeight, 20) << "world height";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
