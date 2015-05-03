
#include "render/lightpath.h"
#include <vector>
#include "util/log.h"
#include "util/debug.h"
#include "render/shader.h"

using std::vector;

LightPath::LightPath(Drawable *lightObj, Vector4d lightPoint, Color emittedLight,
        vector<PathPoint> bounces, Vector4d cameraPoint) {

    lightObject = lightObj;
    lightLocation = lightPoint;
    emitted = emittedLight;
    objectPoints = bounces;
    cameraLocation = cameraPoint;

    origin = "not set";
    clearPath = false;
}

LightPath::LightPath(LightPartialPath lightPath, CamPartialPath camPath) {
    lightObject = lightPath.lightObject;
    lightLocation = lightPath.lightLocation;
    emitted = lightPath.emitted;
    cameraLocation = camPath.cameraLocation.orig;
    sampleX = camPath.samplex;
    sampleY = camPath.sampley;
    clearPath = false;
    objectPoints.reserve(lightPath.bounces.size() + camPath.bounces.size());
    objectPoints.insert(objectPoints.begin(),
                   lightPath.bounces.begin(),
                   lightPath.bounces.end());
    std::reverse(camPath.bounces.begin(), camPath.bounces.end());
    objectPoints.insert(objectPoints.end(),
                   camPath.bounces.begin(),
                   camPath.bounces.end());
    origin = "not set";
}

std::tuple<Vector4d, Color> LightPath::getLight() {
    return std::make_tuple(lightLocation, emitted);
}

Drawable* LightPath::getLightObject() {
    return lightObject;
}

Vector4d LightPath::getCameraPoint() {
    return cameraLocation;
}

PathPoint LightPath::getPoint(int index) {
    ASSERT(index >= 0, "needs a positive index");
    ASSERT(index < objectPoints.size(), "index out of bounds");
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
    if (index == numberOfBounces() - 1)
        next = cameraLocation;
    else
        next = getPoint(index + 1).location;

    Vector4d current = getPoint(index).location;
    return (next - current).normalized();
}

double LightPath::previousG(int index) {
    Vector4d s1, s2, n1, n2;
    PathPoint p2 = getPoint(index);
    if (index == 0) {
        s1 = lightLocation;
        n1 = (p2.location - lightLocation).normalized();
    } else {
        PathPoint p1 = getPoint(index - 1);
        s1 = p1.location;
        n1 = p1.normal;
    }
    s2 = p2.location;
    n2 = p2.normal;
    return geometric(s1, n1, s2, n2);
}

double LightPath::nextG(int index) {
    Vector4d s1, s2, n1, n2;
    PathPoint p1 = getPoint(index);
    if (index == numberOfBounces() - 1) {
        s2 = cameraLocation;
        n2 = (p1.location - s2).normalized();
    } else {
        PathPoint p2 = getPoint(index + 1);
        s2 = p2.location;
        n2 = p2.normal;
    }
    s1 = p1.location;
    n1 = p1.location;
    return geometric(s1, n2, s2, n2);
}

double LightPath::GCam() {
    if (numberOfBounces() == 0) {
        Vector4d lightdir = (cameraLocation - lightLocation).normalized();
        return geometric(lightLocation, lightdir, cameraLocation, -1 * lightdir);
    } else {
        PathPoint p = getPoint(numberOfBounces() - 1);
        // TODO need to rethink what the camera's normal exactly is
        Vector4d camNorm = (p.location - cameraLocation).normalized();
        return geometric(p.location, p.normal, cameraLocation, camNorm);
    }
}

int LightPath::numberOfBounces() {
    return objectPoints.size();
}

int LightPath::totalSize() {
    return numberOfBounces() + 2;
}

Vector4d LightPath::getCameraDirection() {
    if (numberOfBounces() > 0) {
        return getPoint(numberOfBounces() - 1).location - getCameraPoint();
    } else {
        return lightLocation - getCameraPoint();
    }
}

std::tuple<LightPartialPath, CamPartialPath> LightPath::deleteSubpath(int s, int t) {
    // TODO test
    ASSERT(s < t, "s must be less than t");
    LightPartialPath lightP;
    if (s <= -2) {
        lightP.exists = false;
    } else {
        lightP.exists = true;
        lightP.lightObject = lightObject;
        lightP.emitted = emitted;
        lightP.lightLocation = lightLocation;

        for (int i = 0; i <= s; i++) {
            lightP.bounces.push_back(getPoint(i));
        }
    }

    CamPartialPath camP;
    if (t >= numberOfBounces() + 2) {
        camP.exists = false;
    } else {
        camP.exists = true;
        camP.samplex = getSampleX();
        camP.sampley = getSampleY();
        camP.cameraLocation.orig = getCameraPoint();
        camP.cameraLocation.dir = getCameraDirection();
        camP.cameraLocation.dir.normalize();

        // TODO, this no longer follows pointsDeleted = t - s - 1
        for (int i = numberOfBounces() - 1; i >= t && i >= 0; i--) {
            camP.bounces.push_back(getPoint(i));
        }
    }
    return std::make_tuple(lightP, camP);
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
