#ifndef RT_AREALIGHT_H
#define RT_AREALIGHT_H

#include <vector>
#include "core/ray.h"
#include "core/light.h"
#include "core/color.h"
#include "core/common.h"

class AreaLight {
    public:
    AreaLight(ray orientation, Vector4d up, double size,
            int numlights, double brightness);

    std::vector<Light> generateLights();

    private:
    ray orientation;
    Vector4d right;
    Vector4d up;
    int size;
    int numLights;
    double brightness;
};

#endif // RT_AREALIGHT_H
