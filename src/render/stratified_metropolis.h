#ifndef SRC_RENDER_STRATIFIED_METROPOLIS_H_
#define SRC_RENDER_STRATIFIED_METROPOLIS_H_

#include <vector>
#include "drawables/drawable.h"
#include "render/film.h"
#include "render/camera.h"
#include "render/lightpath.h"
#include "kdtree/kdtree.h"

/**
 * Renders and image with a stratified version of the Metropolis Light Transport
 * Algorithm
 */

class StratifiedMetropolis {
 public:
    StratifiedMetropolis();

    void setCamera(Camera c);

    void setObjects(std::vector<Drawable *> objList);

    void setLightStrata(std::vector< std::vector<Drawable *> > strata);
    void setStrataFromVector(std::vector<Drawable *> lights);

    void sampleImage(Film *imageFilm, int numSamples);

 private:
    Camera cam;
    KDTree objTree;
    vector< vector<Drawable *> > lightStrata;
};

#endif  // SRC_RENDER_STRATIFIED_METROPOLIS_H_
