#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "kdtree.h"
#include "file_loader/obj_loader.h"

namespace {
class KDTreeTest : public ::testing::Test {
protected:
    vector<Drawable *> objList;
    vector<Drawable *> complexScene;

public:
    virtual void SetUp() {
        ObjLoader loader;
        loader.load_to_list(objList, "Tests/obj_example.obj");
        loader.load_to_list(complexScene, "Tests/complex_scene.obj");
    }

    virtual void TearDown() {
        for(int i = 0; i < objList.size(); i++) {
            delete objList[i];
        }

        for(int i = 0; i < complexScene.size(); i++) {
            delete complexScene[i];
        }
    }

};

void simpleIntersection(vector<Drawable*> objList, ray viewRay, double &time,
        Drawable **obj) {
    time = std::numeric_limits<double>::max();
    *obj = NULL;
    for(int i = 0; i < objList.size(); i++) {
        Drawable *curObj = objList[i];
        double curTime = curObj->intersection(viewRay);
        if(curTime > 0.0 && (curTime < time || *obj == NULL)) {
            *obj = curObj;
            time = curTime;
        }
    }
}

TEST_F(KDTreeTest, buildTree) {
    KDTree testTree(objList);
    KDTree anotherTree(complexScene);
}

TEST_F(KDTreeTest, intersection) {
    KDTree tree(complexScene);

    ray testRay;
    testRay.dir = Vector4d(0,-1,-1,0).normalized();
    testRay.orig = Vector4d(0.5, 0, 0.5, 1);

    double controlTime = 0.0;
    Drawable *controlObject = NULL;
    double kdTime = 0.0;
    Drawable *kdObj = NULL;

    simpleIntersection(complexScene, testRay, controlTime, &controlObject);
    tree.intersection(testRay, kdTime, &kdObj);
    EXPECT_EQ(controlObject, kdObj) << "Should intersect the same object";
    if(controlObject != NULL)
        EXPECT_DOUBLE_EQ(controlTime, kdTime);

    // test random rays near the center of the scene
    int intersectionCounter = 0;
    for(int i = 0; i < 500; i++) {
        testRay.dir = Vector4d::Random();
        testRay.dir(3) = 0.0;
        testRay.dir.normalize();

        testRay.orig = Vector4d::Random();
        testRay.orig(3) = 0.0;
        testRay.orig.normalize();
        testRay.orig(3) = 1.0;

        if(testRay.dir.norm() < EPSILON) {
            std::cout << "Too close to 0 vector, skipping " << testRay.dir <<
                std::endl;
            continue;
        }

        simpleIntersection(complexScene, testRay, controlTime, &controlObject);
        tree.intersection(testRay, kdTime, &kdObj);

        EXPECT_EQ(controlObject, kdObj) << "KDTree and control test should "
            "intersect the same object";
        if(controlObject != NULL) {
            EXPECT_DOUBLE_EQ(controlTime, kdTime) << "KDTree and control test "
                "should agree on the distance to intersection";
            intersectionCounter++;
        }

    }
    EXPECT_LT(10, intersectionCounter) <<
        "There were few object intersections for a good test. Run the test "
        "again, or add more objects to the test scenes";
}

TEST_F(KDTreeTest, freeAllObjects) {
    vector<Drawable *> testObjects;
    ObjLoader loader;
    loader.load_to_list(testObjects, "Tests/obj_example.obj");
    KDTree testTree(testObjects);
    testTree.freeAllObj();
}

} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
