#ifndef RT_BIDIRECTIONAL_PATHTRACER_H
#define RT_BIDIRECTIONAL_PATHTRACER_H

#include <vector>
#include <Eigen/Dense>

#include "drawable.h"
#include "kdtree.h"
#include "easypng.h"
#include "ray.h"
#include "common.h"

/**
 * Rendering algrithm that generates paths from both the camera and light
 * sources.
 */
class BidirectionalPathTracer {
public:
    BidirectionalPathTracer ();
    virtual ~BidirectionalPathTracer () { /* do nothing */ };

    /**
     * Evaluates the color output for the given view ray.
     */
    Color trace(const ray& viewRay);

    /**
     * Sets the objects for the scene in the path tracer.
     */
    void setObjects(vector<Drawable *> objList);

    /**
     * Sets the Color of light for rays that exit the scene.
     */
    void setExitSceneColor(Color c) { exitSceneColor = c; };

    /**
     * Sets the index of refraction for empty space in the scene.
     */
    void setIndexOfRefraction(double iRefraction) {
        indexRefractionScene = iRefraction;
    };

    /**
     * Sets the limit on how many bounces eye rays will take
     */
    void setMaxEyeDepth(int depth) { maxEyeDepth = depth;};

    /**
     * sets the limit on how many bounces light rays will take
     */
    void setMaxLightDepth(int depth) { maxLightDepth = depth;};

private:

    KDTree objTree;

    int maxEyeDepth;

    int maxLightDepth;

    Color exitSceneColor;

    double indexRefractionScene;

    Color trace(const ray& viewRay, int depth, double curIndexRefraction);
};

#endif // RT_BIDIRECTIONAL_PATHTRACER_H
