
#include "render/metropolis_light_transport.h"
#include <vector>
#include "util/log.h"
#include "util/debug.h"

using std::vector;

LightPath::LightPath(Vector4d lightPoint, Color emittedLight,
        vector<PathPoint> bounces, Vector4d cameraPoint) {
    lightLocation = lightPoint;
    emitted = emittedLight;
    objectPoints = bounces;
    cameraLocation = cameraPoint;

    clearPath = false;
}

std::tuple<Vector4d, Color> LightPath::getLight() {
    return std::make_tuple(lightLocation, emitted);
}

Vector4d LightPath::getCameraPoint() {
    return cameraLocation;
}

PathPoint LightPath::getPoint(int index) {
    return objectPoints[index];
}

Vector4d LightPath::getLightDirection(int index) {
    Vector4d previous;
    if (index == 0)
        previous = lightLocation;
    else
        previous = getPoint(index - 1).location;

    Vector4d current = getPoint(index).location;
    return (lightLocation - current).normalized();
}

Vector4d LightPath::getViewDirection(int index) {
    Vector4d next;
    if (index == size() - 1)
        next = cameraLocation;
    else
        next = getPoint(index + 1).location;

    Vector4d current = getPoint(index).location;
    return (next - current).normalized();
}

int LightPath::size() {
    return objectPoints.size();
}

void LightPath::setClearPath(bool isclear) {
    clearPath = isclear;
}

bool LightPath::isClearPath() {
    return clearPath;
}

void LightPath::setSampleLocation(double x, double y) {
    sampleX = x;
    sampleY = y;
}

double LightPath::getSampleX() {
    return sampleX;
}

double LightPath::getSampleY() {
    return sampleY;
}

MetropolisRenderer::MetropolisRenderer() {
    exitSceneColor = Color(0.3, 0.3, 0.3);
}

void MetropolisRenderer::setCamera(Camera c) {
    cam = c;
}

void MetropolisRenderer::setObjects(vector<Drawable *> objList) {
    objTree.rebuildTree(objList);
}

void MetropolisRenderer::setLights(vector<Drawable *> lights) {
    lightList = lights;
}

void MetropolisRenderer::sampleImage(Film *imageFilm) {
    ASSERT(lightList.size() > 0, "There must be a light in the scene");
    LightPath x = randomPath();

    for (int i = 0; i < 1000000; i++) {
        LightPath y = mutate(x);
        double accProb = std::min(1.0,
                importance(y) * probabilityOfMutation(x, y) /
                (importance(x) * probabilityOfMutation(y, x)));

        // TODO deposit both x and y on the film
        if (randomRange(0, 1) < accProb) {
            x = y;
        }

        int filmx = (int)(imageFilm->getWidth() * x.getSampleX());
        int filmy = (int)(imageFilm->getHeight() * x.getSampleY());
        imageFilm->addColor(lightOfPath(x), filmx, filmy);
    }
}

std::tuple<ray, Color> MetropolisRenderer::randomLightEmission() {
    int randIndex = rand() % lightList.size();
    Drawable *emittingLight = lightList[randIndex];
    ray emittedRay;
    emittedRay.orig = emittingLight->randomSurfacePoint();
    Vector4d normal = emittingLight->normal_vector(emittedRay.orig);
    emittedRay.dir = perturb(normal, M_PI/2.0);

    Properties prop = emittingLight->getProperties(emittedRay.orig);
    return std::make_tuple(emittedRay, prop.emittance);
}

/**
 * Traces a path of max depth size from the ray.
 * Follows the geomery and physical properties of the scene
 *
 * Path may get cut short because a ray exited the scene
 */
vector<PathPoint> MetropolisRenderer::tracePath(ray start, int size) {
    vector<PathPoint> path;
    ray currentRay = start;
    double time = -1.0;
    Drawable *obj = NULL;
    for (int i = 0; i < size; i++) {
        objTree.intersection(currentRay, time, &obj);

        if (obj == NULL) {
            // exited scene
            return path;
        }

        PathPoint p;
        p.location = currentRay(time);
        p.normal = obj->normal_vector(p.location);
        p.properties = obj->getProperties(p.location);
        p.shader = Diffuse;
        path.push_back(p);

        currentRay.orig = p.location;
        currentRay.dir = perturb(p.normal, M_PI/2.0);
    }

    return path;
}

LightPath MetropolisRenderer::randomPath() {
    // generate the eye path
    double camx = randomRange<double>(0,1);
    double camy = randomRange<double>(0,1);
    ray viewRay = cam.getViewRay(camx, camy);
    vector<PathPoint> eyePath = tracePath(viewRay, 3);

    // generate the light path
    ray lightRay;
    Color emittedLight;
    std::tie(lightRay, emittedLight) = randomLightEmission();
    vector<PathPoint> lightPath = tracePath(lightRay, 0); // TODO increase

    // check if the path can be joined
    Vector4d lightEnd;
    if (lightPath.size() > 0) {
        PathPoint tmp = lightPath[lightPath.size() - 1];
        lightEnd = tmp.location;
    } else {
        lightEnd = lightRay.orig;
    }

    Vector4d eyeEnd;
    if (eyePath.size() > 0) {
        eyeEnd = eyePath[eyePath.size() - 1].location;
    } else {
        eyeEnd = viewRay.orig;
    }

    ray checkRay;
    checkRay.orig = lightEnd;
    checkRay.dir = eyeEnd - lightEnd;
    double time;
    Drawable *intersectionObj;
    objTree.intersection(checkRay, time, &intersectionObj);

    // TODO also check if the right object is hit
    bool clearPath = (checkRay(time) - eyeEnd).norm() < EPSILON * 10;

    // join the paths and return
    vector<PathPoint> bounces;
    bounces.reserve(lightPath.size() + eyePath.size());
    bounces.insert(bounces.end(), lightPath.begin(), lightPath.end());
    std::reverse(eyePath.begin(), eyePath.end());
    bounces.insert(bounces.end(), eyePath.begin(), eyePath.end());

    LightPath generatedPath(
            lightRay.orig,
            emittedLight,
            bounces,
            viewRay.orig);
    generatedPath.setClearPath(clearPath);
    generatedPath.setSampleLocation(camx, camy);
    return generatedPath;
}

Color MetropolisRenderer::lightOfPath(LightPath path) {
    if (!path.isClearPath())
        return Color(0, 0, 0);

    Vector4d lightpoint;
    Color total;
    std::tie(lightpoint, total) = path.getLight();

    for (int i = 0; i < path.size(); i++) {
        PathPoint current = path.getPoint(i);
        ASSERT(isUnitVector(current.normal), "The normal should be a unit vector");

        // set up useful data
        Vector4d lightdirection = path.getLightDirection(i);
        Vector4d viewDirection = path.getViewDirection(i);

        if (viewDirection.dot(current.normal) < 0.0)
            current.normal *= -1.0;

        // calculate lighting based on the BRDF of the point
        if (current.shader == Diffuse) {
            total = current.properties.color * total *
                current.normal.dot(lightdirection);
        } else {
            LOG_E("Unaccounted for shader");
        }

        total += current.properties.emittance;
    }

    return total;
}

double MetropolisRenderer::importance(LightPath p) {
    // TODO better importance function
    Color light = lightOfPath(p);
    // return the luminace of the path
    return 0.2126 * light.red +
           0.7152 * light.green +
           0.0722 * light.blue;
}

LightPath MetropolisRenderer::mutate(LightPath original) {
    return randomPath();
}

double MetropolisRenderer::probabilityOfMutation(LightPath original, LightPath mutated) {
    // assume all paths have the same probability of being generated
    return 1;
}