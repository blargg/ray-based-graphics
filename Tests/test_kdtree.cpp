#include <gtest/gtest.h>
#include <vector>
#include "../kdtree.h"
#include "../obj_loader.h"

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


TEST_F(KDTreeTest, buildTree) {
    KDTree testTree(objList);
    KDTree anotherTree(complexScene);
}

} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
