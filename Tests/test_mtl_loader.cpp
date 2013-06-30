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

    SolidColor result = materials["OtherMat"];
    Properties p = result.getProperties(Vector4d(0,0,0,1));
    EXPECT_DOUBLE_EQ(p.emittance.red, 1.0) << "Set the red emittance wrong";
    EXPECT_DOUBLE_EQ(p.emittance.green, 1.0) << "Set the green emittance wrong";
    EXPECT_DOUBLE_EQ(p.emittance.blue, 1.0) << "Set the blue emittance wrong";

    EXPECT_DOUBLE_EQ(p.color.red, 0.5) << "Set the red color wrong";
    EXPECT_DOUBLE_EQ(p.color.green, 0.5) << "Set the green color wrong";
    EXPECT_DOUBLE_EQ(p.color.blue, 0.0) << "Set the blue color wrong";
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
