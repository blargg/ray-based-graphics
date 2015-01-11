
#include <vector>
#include "render/pathtracer.h"
#include "util/log.h"
#include "core/common.h"

PathTracer::PathTracer() {
    maxDepth = 5;
    exitSceneColor = Color(0.3, 0.3, 0.3);
    indexRefractionScene = 1.0;
}

Color PathTracer::trace(const ray& viewRay) {
    return trace(viewRay, 0, indexRefractionScene);
}

Color PathTracer::trace(const ray& viewRay, int depth, double curIndexRefraction) {
    if(depth >= maxDepth)
        return Color(0,0,0);
    assert(isUnitVector<Vector4d>(viewRay.dir));
    double bestTime = -1.0;
    Drawable *obj = NULL;
    objTree.intersection(viewRay, bestTime, &obj);
    Vector4d intersection = viewRay(bestTime);

    // no object was intersected and the ray has left the scene. return this color.
    if(obj == NULL)
        return exitSceneColor;

    Color retColor(0.0, 0.0, 0.0);
    Properties objProp = obj->getProperties( intersection );
    Vector4d unit_normal = obj->normal_vector( intersection).normalized();

    bool inside_obj;
    if( unit_normal.dot(viewRay.dir) > 0.0 ) {
        unit_normal = unit_normal * -1.0;
        inside_obj = true;
    } else {
        inside_obj = false;
    }

    ray random_ray;
    random_ray.dir = perturb(unit_normal, M_PI/2.0);
    assert(isUnitVector<Vector4d>(random_ray.dir));
    random_ray.orig = intersection;

    // emittance
    retColor += objProp.emittance;

    double pathChoice = randomRange<double>(0, 1);
    int numPaths = 0;
    bool isDiffuse = true;
    if (isDiffuse) numPaths++;
    bool isReflective =
        max3<double>(
                objProp.specular.red,
                objProp.specular.green,
                objProp.specular.blue) > EPSILON;
    if (isReflective) numPaths++;
    bool isTransmissive = objProp.tranparency > EPSILON;
    if (isTransmissive) numPaths++;

    // diffuse
    if (isDiffuse && pathChoice < (1.0 / (double)numPaths)) {
        Color diffuse_lighting = trace(random_ray, depth + 1, curIndexRefraction);
        retColor += (M_PI / 1.0) * ((double)numPaths) * objProp.color * diffuse_lighting * unit_normal.dot(random_ray.dir);
        return retColor;
    } else {
        pathChoice -= (1.0 / numPaths);
    }


    ray reflect_ray;
    reflect_ray.orig = intersection;
    reflect_ray.dir = reflectVector(viewRay.dir, unit_normal);
    assert(isUnitVector<Vector4d>(reflect_ray.dir));

    // reflection
    if (isReflective && pathChoice < (1.0 / (double)numPaths)) {
        Color specular_lighting = trace(reflect_ray, depth + 1, curIndexRefraction);
        retColor += ((double)numPaths) * objProp.specular * specular_lighting;
        return retColor;
    } else {
        pathChoice -= (1.0 / numPaths);
    }

    // refraction
    if(isTransmissive && pathChoice < (1.0 / (double)numPaths)) {
        double next_index_refraction;
        if(inside_obj)
            next_index_refraction = indexRefractionScene;
        else
            next_index_refraction = objProp.i_refraction;

        double n = curIndexRefraction / next_index_refraction;
        double cosI = unit_normal.dot(viewRay.dir);
        double cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
        if(cosT2 > 0.0) {
            double r0 = (curIndexRefraction - next_index_refraction) /
                (curIndexRefraction + next_index_refraction);
            r0 = r0 * r0;
            double reflection_coef = r0 + (1-r0) * pow(1 + viewRay.dir.dot(unit_normal), 5);

            if(1.0 - reflection_coef > EPSILON) {
                ray transmit;
                transmit.dir = (n * viewRay.dir) + (n * cosI - sqrt(cosT2)) * unit_normal;
                transmit.dir.normalize();
                transmit.orig = intersection;
                retColor += ((double)numPaths) * (1.0 - reflection_coef) * trace(transmit, depth + 1, next_index_refraction);
            }

            if(reflection_coef > EPSILON) {
                retColor += ((double)numPaths) * reflection_coef * trace(reflect_ray, depth+1, curIndexRefraction);
            }
        } else {
            retColor += ((double)numPaths) * trace(reflect_ray, depth + 1, curIndexRefraction);
        }
        return retColor;
    } else {
        pathChoice -= (1.0 / numPaths);
        LOG_W("pathChoice=%f, skipped all paths", pathChoice);
    }

    return retColor;
}

void PathTracer::setObjects(vector<Drawable *> objList) {
    objTree.rebuildTree(objList);
}

void PathTracer::setLights(vector<Drawable *> lights) {
    lightList = lights;
}
