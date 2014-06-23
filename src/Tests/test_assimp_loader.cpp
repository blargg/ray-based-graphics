#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "file_loader/assimp_loader.h"

using std::vector;

TEST(assimp_loader, conversion_test) {
    aiMatrix4x4 tmp(1.0,2.0,3.0, 4.0,
            5.0,6.0,7.0,8.0,
            9.0, 10.0, 11.0, 12.0,
            13.0, 14.0, 15.0, 16.0);
    Matrix4d m = convert_matrix(tmp);

    EXPECT_DOUBLE_EQ(m(0,0), 1.0);
    EXPECT_DOUBLE_EQ(m(0,1), 2.0);
    EXPECT_DOUBLE_EQ(m(0,2), 3.0);
    EXPECT_DOUBLE_EQ(m(0,3), 4.0);
    EXPECT_DOUBLE_EQ(m(1,0), 5.0);
    EXPECT_DOUBLE_EQ(m(1,1), 6.0);
}

TEST(assimp_loader, assimp_load) {

    vector<Drawable *> list;
    const aiScene *sc = getScene("obj_example.obj");
    assimp_append(sc, list);
    EXPECT_EQ(list.size(), 28);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
