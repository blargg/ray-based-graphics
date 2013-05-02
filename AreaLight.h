#ifndef RT_AREALIGHT_H
#define RT_AREALIGHT_H

#include <vector>
#include "ray.h"
#include "light.h"
#include "color.h"

class AreaLight {
    public:
    AreaLight(ray<3> orientation, vectre<3> up, double size,
            int numlights, double brightness);

    std::vector<Light> generateLights();

    private:
    ray<3> orientation;
    vectre<3> right;
    vectre<3> up;
    int size;
    int numLights;
    double brightness;
};

#endif // RT_AREALIGHT_H
