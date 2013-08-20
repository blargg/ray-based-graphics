#include "bidirectional_pathtracer.h"

BidirectionalPathTracer::BidirectionalPathTracer() {
    maxEyeDepth = 3;
    maxLightDepth = 3;
    exitSceneColor = Color(0.3, 0.3, 0.3);
    indexRefractionScene = 1.0;
}

Color BidirectionalPathTracer::trace(const ray& viewRay) {
    return trace(viewRay, 0, indexRefractionScene);
}

void BidirectionalPathTracer::setObjects(vector<Drawable *> objList) {
    objTree.rebuildTree(objList);
}

Color trace(const ray& viewRay, int depth, double curIndexRefraction) {
    return Color(0,0,0);
}
