
#include "render/metropolis_light_transport.h"
#include <vector>
#include "util/log.h"
#include "util/debug.h"
#include "render/shader.h"

using std::vector;
using std::tuple;
using std::tie;
using std::make_tuple;

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

        depositSample(imageFilm, x, 1.0 - accProb);
        depositSample(imageFilm, y, accProb);

        if (randomRange(0, 1) < accProb) {
            x = y;
        }
        // LOG_IF_D(i % 100 == 0, "Path size = %d", x.totalSize());
        LOG_D("Path size = %d", x.totalSize());
    }
}

void MetropolisRenderer::depositSample(Film *imageFilm, LightPath p, double weight) {
        int filmx = (int)(imageFilm->getWidth() * p.getSampleX());
        int filmy = (int)(imageFilm->getHeight() * p.getSampleY());
        imageFilm->addColorWeighted(lightOfPath(p), filmx, filmy, weight);
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
        time = -1.0;
        obj = NULL;
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
        currentRay.dir = sampleBSDF(p.shader, p.normal, start.dir);
    }

    return path;
}

bool MetropolisRenderer::isVisable(Vector4d a, Vector4d b) {
    Vector4d aToB = b - a;
    ray view(a, aToB);
    double time = -1.0;
    Drawable *obj = NULL;
    objTree.intersection(view, time, &obj);

    if (obj == NULL || time < 0)
        return false;

    double distToB = (b - view(time)).norm();
    return distToB < EPSILON * 100;
}

bool MetropolisRenderer::pointsAreClose(Vector4d a, Vector4d b) {
    return (a - b).squaredNorm() < 1000 * EPSILON;
}

/**
 * Randomly samples the given bsdf.
 *
 * dist is the type of bsdf
 * normal is the normal vector at the surface
 * view is the direction of the incoming view direction
 */
Vector4d MetropolisRenderer::sampleBSDF(ShaderType dist, Vector4d normal, Vector4d view) {
    if (dist == Diffuse) {
        // choose a random direction in the hemisphere of the normal
        return perturb(normal, M_PI/2.0);
    } else {
        LOG_E("Unaccounted for shader: %d", dist);
        ASSERT(false, "This shader was not coded for");
        return Vector4d(1,0,0,0);
    }
}

double MetropolisRenderer::probabilityOfSample(ShaderType dist, Vector4d view, Vector4d normal, Vector4d out) {
    if (dist == Diffuse) {
        // the out sample direction is uniformly distributed in the hemisphere
        // of the surface normal
        if (normal.dot(out) > 0.0)
            return 1.0;
        else
            return 0.0;
    } else {
        LOG_E("Shader not programed");
        ASSERT(false, "This should not execute");
        return 0.0;
    }
}

LightPath MetropolisRenderer::randomPath() {
    // generate the eye path
    CamPartialPath camp;
    camp.exists = true;
    double camx = randomRange<double>(0,1);
    double camy = randomRange<double>(0,1);
    ray viewRay = cam.getViewRay(camx, camy);
    camp.samplex = camx;
    camp.sampley = camy;
    camp.cameraLocation = viewRay;
    camp.bounces = tracePath(viewRay, skewedGeometricRandom());

    // generate the light path
    LightPartialPath lightp;
    lightp.exists = true;

    ray lightRay;
    Color emittedLight;
    std::tie(lightRay, emittedLight) = randomLightEmission();
    lightp.emitted = emittedLight;
    lightp.lightLocation = lightRay.orig;
    lightp.bounces = tracePath(lightRay, 0); // TODO increase

    // check if the path can be joined
    Vector4d lightEnd;
    if (lightp.bounces.size() > 0) {
        PathPoint tmp = lightp.bounces[lightp.bounces.size() - 1];
        lightEnd = tmp.location;
    } else {
        lightEnd = lightRay.orig;
    }

    Vector4d eyeEnd;
    if (camp.bounces.size() > 0) {
        eyeEnd = camp.bounces[camp.bounces.size() - 1].location;
    } else {
        eyeEnd = viewRay.orig;
    }

    // TODO also check if the right object is hit
    bool clearPath = isVisable(lightEnd, eyeEnd);

    // join the paths and return
    LightPath fullPath(lightp, camp);
    fullPath.setClearPath(clearPath);
    return fullPath;
}

LightPath MetropolisRenderer::bidirectionalMutation(LightPath p) {
    int deleteLength = skewedGeometricRandom() + 1;
    if (deleteLength >= p.totalSize()) {
        return randomPath();
    }
    int s = randomRangeInt(-2, p.numberOfBounces() - deleteLength);
    int t = s + deleteLength;

    LightPartialPath lightp;
    CamPartialPath camp;
    std::tie(lightp, camp) = p.deleteSubpath(s, t);

    int lengthAdd = lengthToAdd(deleteLength);
    int s_add = randomRangeInt(0, lengthAdd - 1);
    int t_add = lengthAdd - s_add - 1;
    ASSERT(s_add >= 0, "light path add must be greater than or equal to 0");
    ASSERT(t_add >= 0, "light path add must be greater than or equal to 0");

    // TODO: case hanling here is huge
    // some of the design doesn't really fit c++ style well
    ray lightCastDir;
    if (!lightp.exists) {
        ray lightdir;
        Color emittedlight;
        std::tie(lightdir, emittedlight) = randomLightEmission();

        lightCastDir = lightdir;
        lightp.lightLocation = lightdir.orig;
        lightp.emitted = emittedlight;
        lightp.exists = true;
        lightp.bounces.clear();
    } else {
        if (lightp.bounces.size() == 0) {
            lightCastDir.orig = lightp.lightLocation;
            lightCastDir.dir = randomUnitVector();
        } else {
            PathPoint lastBounce = lightp.bounces.back();
            lightCastDir.orig = lastBounce.location;

            Vector4d incomingDir;
            if (lightp.bounces.size() == 1) {
                incomingDir = lightp.bounces[0].location - lightp.lightLocation;
            } else if (lightp.bounces.size() > 1) {
                int index = lightp.bounces.size() - 1;
                incomingDir = lightp.bounces[index].location - lightp.bounces[index - 1].location;
            }
            lightCastDir.dir =
                sampleBSDF(lastBounce.shader, lastBounce.normal, incomingDir);
        }
    }

    // get new light path points
    vector<PathPoint> addedToLight = tracePath(lightCastDir, s_add);

    // add points to light path
    lightp.bounces.insert(lightp.bounces.end(),
                          addedToLight.begin(),
                          addedToLight.end());

    ray camCastDir;
    if (!camp.exists) {
        double camx = randomRange<double>(0,1);
        double camy = randomRange<double>(0,1);
        camCastDir = cam.getViewRay(camx, camy);

        camp.samplex = camx;
        camp.sampley = camy;
        camp.cameraLocation = camCastDir;
        camp.bounces.clear();
        camp.exists = true;
    } else {
        if (camp.bounces.size() == 0) {
            camCastDir = camp.cameraLocation;
        } else {
            PathPoint lastBounce = camp.bounces.back();
            camCastDir.orig = lastBounce.location;
            Vector4d viewDir;
            if (camp.bounces.size() == 1) {
                viewDir =
                    camp.bounces[0].location - camp.cameraLocation.orig;
            } else {
                int index = camp.bounces.size() - 1;
                viewDir =
                    camp.bounces[index].location - camp.bounces[index - 1].location;
            }
            camCastDir.dir =
                sampleBSDF(lastBounce.shader, lastBounce.normal, viewDir);
        }
    }

    vector<PathPoint> addedToCam = tracePath(camCastDir, t_add);
    camp.bounces.insert(camp.bounces.end(),
                        addedToCam.begin(),
                        addedToCam.end());

    // check if the light path is clear
    Vector4d lightEnd;
    if (lightp.bounces.size() == 0)
        lightEnd = lightp.lightLocation;
    else
        lightEnd = lightp.bounces.back().location;
    Vector4d camEnd;
    if (camp.bounces.size() == 0)
        camEnd = camp.cameraLocation.orig;
    else
        camEnd = camp.bounces.back().location;

    bool pathClear = s_add == addedToLight.size() &&
                     t_add == addedToCam.size() &&
                     isVisable(lightEnd, camEnd);

    LightPath fullPath(lightp, camp);
    fullPath.setClearPath(pathClear);
    return fullPath;
}

double MetropolisRenderer::probOfBidirectionalTransition(LightPath original, LightPath mutant) {
    int s, t;
    tie(s, t) = segmentChanged(original, mutant);
    int s_a, t_a;
    tie(s_a, t_a) = segmentChanged(mutant, original);

    return probOfSkewedGeometric(t - s - 1)
           * probOfLengthChange(t - s, t_a - s_a)
           * probOfAddingSamples(mutant, s_a, t_a);
}

double MetropolisRenderer::probOfLengthChange(int lengthDeleted, int lengthAdded) {
    int diff = lengthDeleted - lengthAdded;
    if (diff == 0)
        return 0.5;
    if (diff == 1)
        return 0.25;
    ASSERT(lengthDeleted != 0, "Division by 0");
    return 0.25 * (1 / (lengthDeleted - 1));
}

double MetropolisRenderer::probOfAddingSamples(LightPath path, int s, int t) {
    ASSERT(s < t, "s = %d, t = %d", s, t);
    if (s < 0)
        s = 0;
    if (t > path.numberOfBounces() - 1)
        t = path.numberOfBounces() - 1;
    double totalProb = 0.0;
    // TODO there is duplicated work in the calculation here
    for (int split = s; split <= t; split++) {
        double prob = 1.0;
        for (int i = s+1; i < split; i++) {
            PathPoint point = path.getPoint(i);
            prob *=
                probabilityOfSample(
                        point.shader,
                        path.getLightDirection(i),
                        point.normal,
                        path.getViewDirection(i));
            prob *= path.nextG(i);
        }
        for (int i = split; i < t; i++) {
            PathPoint point = path.getPoint(i);
            prob *=
                probabilityOfSample(
                        point.shader,
                        path.getLightDirection(i),
                        point.normal,
                        path.getViewDirection(i));
            prob *= path.previousG(i);
        }

        totalProb += prob;
    }
    return totalProb;
}

tuple<int, int> MetropolisRenderer::segmentChanged(LightPath original, LightPath mutant) {
    int s = -2;
    int t = original.totalSize() - 1;

    if (pointsAreClose(original.getCameraPoint(), mutant.getCameraPoint())) {
        for (int i = 0; i < original.numberOfBounces() && i < mutant.numberOfBounces(); i++) {
            if (pointsAreClose(original.getPoint(i).location, mutant.getPoint(i).location))
                s = i;
            else
                break;
        }
    }

    Vector4d originalLight;
    Vector4d mutantLight;
    Color lightIntensity;

    tie(originalLight, lightIntensity) = original.getLight();
    tie(mutantLight, lightIntensity) = mutant.getLight();
    if (pointsAreClose(originalLight, mutantLight)) {
        for (int i = 1 ; 0 <= original.numberOfBounces() - i && 0 <= mutant.numberOfBounces() - i; i++) {
            int originalIndex = original.numberOfBounces() - i;
            int mutantIndex = mutant.numberOfBounces() - i;

            if (pointsAreClose(original.getPoint(originalIndex).location, mutant.getPoint(mutantIndex).location))
                t = i;
            else
                break;
        }
    }

    return make_tuple(s, t);
}

/**
 * Given the lenght deleted, return a random length to add.
 * lengthDeleted >= 1
 * return >= 1
 */
int MetropolisRenderer::lengthToAdd(int lengthDeleted) {
    ASSERT(lengthDeleted >= 1,
            "This value should always be at least 1");
    double r = (double) rand() / (double) RAND_MAX;
    if (r > 0.5) {
        return lengthDeleted;
    }

    if (lengthDeleted == 1)
        return lengthDeleted + 1;

    if (r > 0.25)
        return lengthDeleted + 1;
    else
        return randomRangeInt(1, lengthDeleted - 1);
}

Color MetropolisRenderer::lightOfPath(LightPath path) {
    if (!path.isClearPath())
        return Color(0, 0, 0);

    Vector4d lightpoint;
    Color total;
    std::tie(lightpoint, total) = path.getLight();

    for (int i = 0; i < path.numberOfBounces(); i++) {
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
    //
    Color light = lightOfPath(p);
    // return the luminace of the path
    return (0.2126 * light.red +
           0.7152 * light.green +
           0.0722 * light.blue);
}

LightPath MetropolisRenderer::mutate(LightPath original) {
    return bidirectionalMutation(original);
}

double MetropolisRenderer::probabilityOfMutation(LightPath original, LightPath mutated) {
    // assume all paths have the same probability of being generated
    return probOfBidirectionalTransition(original, mutated);
}
