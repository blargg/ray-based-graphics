#include "raytracer.h"
#include "core/common.h"
#include <cassert>

Raytracer::Raytracer() {
    max_recursion_depth = 5;
}
void Raytracer::clear_scene()
{
    clear_objects();
    clear_lights();
}

void Raytracer::clear_objects()
{
    for(unsigned int i = 0; i < objList.size(); ++i)
        delete objList[i];
    objList.clear();
}

void Raytracer::clear_lights()
{
    lightList.clear();
}

// ********************* Main Rendering function ****************************
Color Raytracer::trace(const ray& viewRay) {
    return trace(viewRay, 0, 1.0);
}

Color Raytracer::trace(const ray& viewRay, int depth, double curIndexRefraction)
{
    double bestTime = -1.0;
    Drawable *obj = NULL;
    getClosestObject(viewRay, &obj, bestTime);
    Vector4d intersection = viewRay(bestTime);

    // no object was intersected and the ray has left the scene. return this color.
    if(obj == NULL)
        return Color(0,0,0);

    Color retColor(0.0, 0.0, 0.0);
    Properties objProp = obj->getProperties( intersection );
    Vector4d unit_normal = obj->normal_vector( intersection);
    unit_normal.normalize();

    if( unit_normal.dot(viewRay.dir) > 0.0 )
        unit_normal = unit_normal * -1.0;

    vector<Light> lights = generateLights();
    for(unsigned int i = 0; i < lights.size(); ++i)
    {
        Vector4d lightPath = lights[i].location - intersection;

        if(unit_normal.dot(lightPath) <= 0.0)
            continue;
        if(lightPath.norm() <= 0.0)
            continue;

        Vector4d lightDir = lightPath.normalized();
        ray lightRay(intersection, lightDir);

        // If not in the shadow of another object, to the lighting
        if(!intersectsObject(lightRay))
        {
            double att = attenuation(lightPath.norm());
            double lambert = (lightRay.dir.dot(unit_normal));
            retColor += att * lambert * lights[i].color * objProp.color;

            retColor += att * blinnPhong(viewRay, unit_normal, lightRay, lights[i].color, objProp);
        }
    }

    // cast more rays for reflections for a certain depth
    if(objProp.reflect > 0.0 && depth < max_recursion_depth)
    {
        ray reflect_ray(intersection, viewRay.dir - (2.0 * (viewRay.dir.dot(unit_normal))) * unit_normal);
        Color reflect_color = trace(reflect_ray, depth + 1, curIndexRefraction);
        retColor += objProp.reflect * reflect_color;
    }

    return retColor;
}

//********************** Private Helper Functions **************************
/**
 * finds the closest object and time to get there by the ray in the object list.
 * @param viewRay the ray to test
 * @param closestObject the index of the current closest object (gets modified)
 * @param bestTime the current best time of the closest object (is overwritten)
 *
 * @post *closestObj points to the closest object
 * @post bestTime is the time to the closest object
 */
void Raytracer::getClosestObject(const ray& viewRay, Drawable **closestObj, double& bestTime)
{
    objTree.intersection(viewRay, bestTime, closestObj);
}

vector<Light> Raytracer::generateLights() {
    vector<Light> genLights = lightList;

    for(unsigned int i = 0; i < areaLightList.size(); i++) {
        vector<Light> newLights = areaLightList[i].generateLights();
        for(unsigned int j = 0; j < newLights.size(); j++) {
            genLights.push_back(newLights[j]);
        }
    }

    return genLights;
}

bool Raytracer::intersectsObject( const ray &viewRay ){
    for(unsigned int j = 0; j < objList.size(); j++){
        if(objList[j]->intersection(viewRay) > MIN_INTERSECTION_DIST)
        {
            return true;
        }
    }
    return false;
}

double Raytracer::attenuation(const double dist) {
    return 1.0 / (0.01 + 0.03 * dist);
}

Color Raytracer::blinnPhong(const ray &viewRay, const Vector4d &unit_normal,
        const ray lightRay, const Color lightColor, const Properties &objProp){

    double viewProj = viewRay.dir.dot(unit_normal);
    double lightProj = lightRay.dir.dot(unit_normal);
    Vector4d blinnDir = lightRay.dir - viewRay.dir;
    double temp = blinnDir.dot(blinnDir);
    if(temp != 0.0)
    {
        double blinn = (1.0 / sqrt(temp)) * std::max(lightProj - viewProj, 0.0);
        blinn = pow(blinn, objProp.spec_power);
        return blinn * objProp.specular * lightColor;
    }
    return Color(0.0, 0.0, 0.0);
}

//*********************** Static Constant Vairables **********************
const Color Raytracer::DEBUG_COLOR = Color(1.0, 0.0, 0.4);
