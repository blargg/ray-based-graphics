#include "AreaLight.h"
#include "func.h"


AreaLight::AreaLight(ray<3> orientation, vectre<3> up, double size,
        int numlights, double brightness) {
    this->orientation = orientation;
    this->orientation.dir = this->orientation.dir.unit_vectre();
    this->up = up.unit_vectre();
    right = this->orientation.dir.cross_prod(up);

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
            point<3> position;
            position = orientation.orig;
            position = add(position, dx * x * right);
            position = add(position, dy * y * up);
            l.location = position;
            lights.push_back(l);
        }
    }

    return lights;
}
