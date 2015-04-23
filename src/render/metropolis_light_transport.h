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
     * Sets the objects in the scene with a preconstructed kdtree
     */
    void setObjectsByKDTree(KDTree objectTree);

    /**
     * Sets the lights in the scene
     */
    void setLights(std::vector<Drawable *> lights);

    /**
     * Samples the scene using the MLT algorithm
     * Deposits samples into the film
     */
    void sampleImage(Film *imageFilm, int numSamples);

 private:
    Camera cam;
    KDTree objTree;
    vector<Drawable *> lightList;
    Color exitSceneColor;

    // deposits a light path onto the film with the given weight
    void depositSample(Film *imageFilm, LightPath p, double weight);

    // generates a random beam of light emitted from a source in the scene
    std::tuple<ray, Color, Drawable *> randomLightEmission();
    /**
     * Generates a random path with bidirectional path tracing
     */
    LightPath randomPath();

    /**
     * Mutates a light path by deleting a segment and tracing a new path
     */
    LightPath bidirectionalMutation(LightPath p);

    double probOfBidirectionalTransition(LightPath original, LightPath mutant);
    double probOfLengthChange(int lengthDeleted, int lengthAdded);
    double probOfAddingSamples(LightPath path, int s, int t);

    /**
     * returns (s, t), the segment deleted from original, and replaced to make
     * mutant.
     *
     * Everything point at index i s.t. i <= s is in both paths
     * Everything point at index i s.t. t <= i is in both paths
     *
     * if the paths are different then s < t
     *
     * Indecies of matching points may not be the same (eg. the paths are
     * different lenghts and the last point in each path match)
     */
    std::tuple<int, int> segmentChanged(LightPath original, LightPath mutant);

    int lengthToAdd(int lengthDeleted);

    // generates a random series of bounces in the scene
    std::vector<PathPoint> tracePath(ray start, int size);
    ShaderType randomShader(Properties p);
    double probOfShader(Properties p, ShaderType shader);
    vector<ShaderType> getShaders(Properties p);

    /**
     * returns true if point a is mutually visable from point b
     *
     * isVisable(x, y) <==> isVisable(y, x)
     */
    bool isVisable(Vector4d a, Vector4d b);

    /**
     * true if the distance between the points is less than machine precision
     */
    bool pointsAreClose(Vector4d a, Vector4d b);

    Vector4d sampleBSDF(ShaderType dist, Vector4d normal, Vector4d view);
    double probabilityOfSample(ShaderType dist, Vector4d view, Vector4d normal, Vector4d out);

    Color lightOfPath(LightPath p);

    /**
     * returns true if the path is in the current stratum
     */
    bool inStratum(LightPath p);
    double importance(LightPath p);

    LightPath mutate(LightPath original);
    double probabilityOfMutation(LightPath original, LightPath mutated);
};

#endif  // SRC_RENDER_METROPOLIS_LIGHT_TRANSPORT_H_
