#include <gtest/gtest.h>

#include "../loader.h"
#include "../raytracer.h"
#include "../ray.h"

TEST(loader, loadsShapes){
    Raytracer render;
    render = loadScene("scene.data");
    Light l = render.lightList[0];
    EXPECT_DOUBLE_EQ(l.location[0], 100) << "Location should match (x)";
    EXPECT_DOUBLE_EQ(l.location[1], 50.25) << "Location should match (y)";
    EXPECT_DOUBLE_EQ(l.location[2], 10) << "Location should match (z)";

    Drawable *fst = render.objList[0];
    ray<3> viewRay;
    viewRay.dir = vectre<3>(1,0,0);
    viewRay.orig = point<3>(-2,0,0);
    EXPECT_GT(fst->intersection(viewRay), 0.0);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
