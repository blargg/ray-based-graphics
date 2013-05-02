#include <gtest/gtest.h>

#include "../shapes/sphere.h"
#include "../bounding_shape.h"
#include "../materials/solidColor.h"
#include "../simpleObject.h"

namespace{
class BoundingTest : public ::testing::Test{
    protected:
    Sphere unit_sphere;
    Drawable *unit_pointer;
    Sphere s2;
    Drawable *s2_pointer;
    Sphere bounds;
    BoundingShape *container;

    public:
    virtual void SetUp(){
        SolidColor mat;
        unit_sphere = Sphere(point<3>(0.0, 0.0, 0.0), 1);
        s2 = Sphere(point<3>(10.0, 10.5, 0.0), 10);
        bounds = Sphere(point<3>(0,0,0), 100);
        container = new BoundingShape(bounds);
        unit_pointer = new SimpleObject(unit_sphere, mat);
        container->addDrawable(unit_pointer);
        container->addDrawable(new SimpleObject(s2, mat));

        container = new BoundingShape(Sphere(point<3>(0,0,0),100));
        Sphere s(point<3>(0,0,100), 10);
        Properties prop;
        prop.color = Color(0,1,0);
        prop.reflect = 0.3;
        prop.specular = Color(0.0,0.0,0.0);
        prop.spec_power = 0.0;
        container->addDrawable(new SimpleObject(s, SolidColor(prop)));
        Sphere s2(point<3>(20,0,100), 10);
        prop.color = Color(.2, .2, .8);
        container->addDrawable(new SimpleObject(s2, SolidColor(prop)));
        Sphere s3(point<3>(-20,0,100), 10);
        prop.color = Color(.5, .8, .4);
        container->addDrawable(new SimpleObject(s3, SolidColor(prop)));
    }

    virtual void TearDown(){
        delete container;
    }

};

TEST_F(BoundingTest, intersectsObj){
    ray<3> viewRay;
    viewRay.dir = vectre<3>(0,0,1);
    viewRay.orig = point<3>(0,0,-200);
    EXPECT_EQ(container->intersectionObj(viewRay), unit_pointer);

    viewRay.dir = vectre<3>(0,0,1);
    viewRay.orig = point<3>(0,100,0);
    EXPECT_EQ(container->intersectionObj(viewRay), (Drawable *)NULL);
}


} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
