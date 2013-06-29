#include <gtest/gtest.h>

#include "../obj_loader.h"

TEST(obj_loader, obj_load) {
    vector<Drawable*> list;
    ObjLoader loader;
    loader.load_to_list(list, "Tests/obj_example.obj");

}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
