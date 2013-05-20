#include <gtest/gtest.h>

#include <vector>
#include "../AreaLight.h"
#include "../light.h"

using std::vector;

TEST(AreaLight, generateslights){
    ray orientation;
    orientation.dir = Vector4d(0,0,-1,0);
    orientation.orig = Vector4d(100,50,500,1);
    AreaLight arLight(orientation, Vector4d(0,1,0, 0), 5000, 9, 1.0);
    vector<Light> lightList = arLight.generateLights();

    EXPECT_EQ(lightList.size(), 81) << "Number of lights sould be numLights^2";

    for(unsigned int i = 0; i < lightList.size(); i++){
        Vector4d p = lightList[i].location;
        EXPECT_TRUE(p[0] <= 5101 && p[0] >= 100)
            << "Light(x) is not in the bounds:" << p[0];
        EXPECT_TRUE(p[1] <= 5051 && p[1] >= 50)
            << "Light(y) is not in the bounds:" << p[1];
        EXPECT_TRUE(p[2] <= 501 && p[2] >= 499)
            << "Light(z) is not in the bounds;" << p[2];
    }

}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
