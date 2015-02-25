#ifndef SRC_RENDER_METROPOLIS_LIGHT_TRANSPORT_H_
#define SRC_RENDER_METROPOLIS_LIGHT_TRANSPORT_H_

#include <vector>
#include <tuple>
#include "drawables/drawable.h"
#include "render/film.h"
#include "render/camera.h"
#include "kdtree/kdtree.h"

enum ShaderType { Diffuse, Specular, Transmissive, CameraShader };

struct PathPoint {
    Vector4d location;
    Vector4d normal;
    Properties properties;
    ShaderType shader;
};

class LightPath {
 public:
    LightPath(Vector4d lightPoint, Color emittedLight,
            vector<PathPoint> bounces, Vector4d cameraPoint);

    std::tuple<Vector4d, Color> getLight();
    Vector4d getCameraPoint();

    PathPoint getPoint(int index);
    // for a path point, get the direction that the light is coming from
    Vector4d getLightDirection(int index);
    // for a path point, get the direction that light is going out to
    Vector4d getViewDirection(int index);
    int size();

    void setClearPath(bool isclear);
    bool isClearPath();
    // record what part of the film this sample contributes to
    void setSampleLocation(double x, double y);
    // return the location of the sample on the film.
    // from 0 to 1
    double getSampleX();
    // return the location of the sample on the film.
    // from 0 to 1
    double getSampleY();

 private:
    vector<PathPoint> objectPoints;
    Vector4d lightLocation;
    Color emitted;
    Vector4d cameraLocation;
    double sampleX;
    double sampleY;
    bool clearPath;
};

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
