#include "AreaLight.h"
#include "common.h"


AreaLight::AreaLight(ray orientation, Vector4d up, double size,
        int numlights, double brightness) {
    this->orientation = orientation;
    this->orientation.dir.normalize();
    this->up = up.normalized();
    right = cross(this->orientation.dir, this->up);

    this->size = size;
    this->numLights = numlights;
    this->brightness = (brightness / numlights / numlights);
}

std::vector<Light> AreaLight::generateLights() {
    std::vector<Light> lights;

    Light l;
    l.color = Color(brightness, brightness, brightness);
    double dx = size / numLights;
    double dy = size / numLights;
    for(int x = 0; x < numLights; x++) {
        for(int y = 0; y < numLights; y++) {
            Vector4d position;
            position = orientation.orig + (dx * x * right) + (dy * y * up);

            l.location = position;
            lights.push_back(l);
        }
    }

    return lights;
}
