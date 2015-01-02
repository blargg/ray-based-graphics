#include <gtest/gtest.h>

#include "file_loader/cam_loader.h"

TEST(cam_loader, loadCameraFromFile) {
    Camera c = loadCameraFromFile("cam_file.cam");

    // location
    EXPECT_FLOAT_EQ(c.position.orig[0], 4.0);
    EXPECT_FLOAT_EQ(c.position.orig[1], 5.0);
    EXPECT_FLOAT_EQ(c.position.orig[2], 6.0);

    // direction
    EXPECT_FLOAT_EQ(c.position.dir[0], 1.0);
    EXPECT_FLOAT_EQ(c.position.dir[1], 0.5);
    EXPECT_FLOAT_EQ(c.position.dir[2], 1.1);

    // up direction
    EXPECT_FLOAT_EQ(c.up[0], 1.0);
    EXPECT_FLOAT_EQ(c.up[1], 2.0);
    EXPECT_FLOAT_EQ(c.up[2], 3.0);

    // dim
    EXPECT_DOUBLE_EQ(c.worldWidth, 10);
    EXPECT_DOUBLE_EQ(c.worldHeight, 20);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
