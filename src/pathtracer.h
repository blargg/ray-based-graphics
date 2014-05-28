#ifndef RT_PATHTRACER_H
#define RT_PATHTRACER_H

#include <vector>
#include <Eigen/Dense>

#include "drawable.h"
#include "kdtree.h"
#include "core/easypng.h"
#include "ray.h"
#include "core/common.h"

class PathTracer {
public:
    /**
     * Initializes the Pathtracer
     */
    PathTracer ();

    virtual ~PathTracer () { /* do nothing */ };

    /**
     * Evaluates the color output for the given view ray.
     */
    Color trace(const ray& viewRay);

    /**
     * Sets the objects for the scene in the path tracer.
     */
    void setObjects(vector<Drawable *> objList);

    /**
     * Sets the maximum recusion depth that a light path will bounce before
     * terminating.
     */
    void setMaxRecursionDepth(int depth) { maxDepth = depth; };

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

private:

    KDTree objTree;

    int maxDepth;

    Color exitSceneColor;

    double indexRefractionScene;

    Color trace(const ray& viewRay, int depth, double curIndexRefraction);
};

#endif // RT_PATHTRACER_H
