#ifndef SRC_RENDER_METROPOLIS_LIGHT_TRANSPORT_H_
#define SRC_RENDER_METROPOLIS_LIGHT_TRANSPORT_H_

#include <vector>
#include <tuple>
#include "drawables/drawable.h"
#include "render/film.h"
#include "render/camera.h"
#include "render/lightpath.h"
#include "kdtree/kdtree.h"

/**
 * Renders an image using the Metropolis Light Transport Algorithm
 */
class MetropolisRenderer {
 public:
    MetropolisRenderer();

    /**
     * set the camera view to sample
     */
    void setCamera(Camera c);

    /**
     * Sets the objects in the scene
     */
    void setObjects(std::vector<Drawable *> objList);

    /**
     * Sets the lights in the scene
     */
    void setLights(std::vector<Drawable *> lights);

    /**
     * Samples the scene using the MLT algorithm
     * Deposits samples into the film
     */
    void sampleImage(Film *imageFilm);

 private:
    Camera cam;
    KDTree objTree;
    vector<Drawable *> lightList;
    Color exitSceneColor;

    // deposits a light path onto the film with the given weight
    void depositSample(Film *imageFilm, LightPath p, double weight);

    // generates a random beam of light emitted from a source in the scene
    std::tuple<ray, Color> randomLightEmission();
    /**
     * Generates a random path with bidirectional path tracing
     */
    LightPath randomPath();
    // generates a random series of bounces in the scene
    std::vector<PathPoint> tracePath(ray start, int size);
    Color lightOfPath(LightPath p);
    double importance(LightPath p);

    LightPath mutate(LightPath original);
    double probabilityOfMutation(LightPath original, LightPath mutated);
};

#endif  // SRC_RENDER_METROPOLIS_LIGHT_TRANSPORT_H_
