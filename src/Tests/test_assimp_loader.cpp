#include <gtest/gtest.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

#include "file_loader/assimp_loader.h"

using std::vector;

TEST(assimp_loader, conversion_test) {
    aiMatrix4x4 tmp(1.0, 2.0, 3.0, 4.0,
            5.0, 6.0, 7.0, 8.0,
            9.0, 10.0, 11.0, 12.0,
            13.0, 14.0, 15.0, 16.0);
    Matrix4d m = convert_matrix(tmp);

    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(m(0, 2), 3.0);
    EXPECT_DOUBLE_EQ(m(0, 3), 4.0);
    EXPECT_DOUBLE_EQ(m(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 6.0);
}

TEST(assimp_loader, assimp_load) {
    vector<Drawable *> list;
    vector<Drawable *> lights;
    const aiScene *sc = getScene("obj_example.obj");
    assimp_append(sc, list, lights);
    EXPECT_EQ(list.size(), 28);
    EXPECT_EQ(lights.size(), 0) << "no emmisive term in this obj file";
}

TEST(assimp_loader, lights) {
    vector<Drawable *> list;
    vector<Drawable *> lights;
    const aiScene *sc = getScene("room.dae");
    assimp_append(sc, list, lights);
    EXPECT_EQ(list.size(), 992);
    EXPECT_EQ(lights.size(), 2) << "There should be 2 objects with an emmissive term.";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
