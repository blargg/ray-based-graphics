
#include "render/lightpath.h"
#include <vector>
#include "util/log.h"
#include "util/debug.h"
#include "render/shader.h"

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
    if (index == size() - 1)
        next = cameraLocation;
    else
        next = getPoint(index + 1).location;

    Vector4d current = getPoint(index).location;
    return (next - current).normalized();
}

double LightPath::G(int index) {
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

double LightPath::GCam() {
    if (size() == 0) {
        Vector4d lightdir = (cameraLocation - lightLocation).normalized();
        return geometric(lightLocation, lightdir, cameraLocation, -1 * lightdir);
    } else {
        PathPoint p = getPoint(size() - 1);
        // TODO need to rethink what the camera's normal exactly is
        Vector4d camNorm = (p.location - cameraLocation).normalized();
        return geometric(p.location, p.normal, cameraLocation, camNorm);
    }
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
