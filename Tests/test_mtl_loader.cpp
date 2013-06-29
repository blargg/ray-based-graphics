#include <gtest/gtest.h>

#include "../mtl_loader.h"
#include <map>
#include <string>

using std::map;
using std::string;

TEST(MtlLoader, load_to_map) {
    map<string, SolidColor> materials;
    MtlLoader mloader;
    mloader.add_to_map(materials, "Tests/stove.mtl");

    EXPECT_EQ(materials.size(), 2) << "Wrong number of materials loaded.";
    EXPECT_GT(materials.count("TestMat"), 0) << "Could not find TestMat.";
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
