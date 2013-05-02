#include "raytracer.h"

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
Color Raytracer::getColor(const ray<3>& viewRay, int depth)
{
    //int closestObj = -1;
    double bestTime = -1.0;
    Drawable *obj = NULL;
    getClosestObject(viewRay, &obj, bestTime);
    point<3> intersection = viewRay(bestTime);

    // no object was intersected and the ray has left the scene. return this color.
    if(obj == NULL)
        return Color(0,0,0);

    Color retColor(0.0, 0.0, 0.0);
    Properties objProp = obj->getProperties( intersection );
    vectre<3> unit_normal = obj->normal_vectre( intersection).unit_vectre();
    if( unit_normal.dot_prod(viewRay.dir) > 0.0 )
        unit_normal = unit_normal * -1.0;

    vector<Light> lights = generateLights();
    for(unsigned int i = 0; i < lights.size(); ++i)
    {
        vectre<3> lightPath(intersection, lights[i].location);

        if(unit_normal.dot_prod(lightPath) <= 0.0)
            continue;
        if(lightPath.length_sq() <= 0.0)
            continue;

        vectre<3> lightDir = lightPath.unit_vectre();
        ray<3> lightRay(intersection, lightDir);

        // If not in the shadow of another object, to the lighting
        if(!intersectsObject(lightRay))
        {
            double att = attenuation(lightPath.length());
            double lambert = (lightRay.dir.dot_prod(unit_normal));
            retColor += att * lambert * lights[i].color * objProp.color;

            retColor += att * blinnPhong(viewRay, unit_normal, lightRay, lights[i].color, objProp);
        }
    }

    // cast more rays for reflections for a certain depth
    if(objProp.reflect > 0.0 && depth < 10)
    {
        ray<3> reflect_ray(intersection, viewRay.dir - (2.0 * (viewRay.dir.dot_prod(unit_normal))) * unit_normal);
        Color reflect_color = getColor(reflect_ray, depth + 1);
        retColor += objProp.reflect * reflect_color;
    }

    return retColor;
}

Color Raytracer::pathtraceColor(const ray<3>& viewRay, int depth)
{
    if(depth >= 10)
        return Color(0,0,0);
    double bestTime = -1.0;
    Drawable *obj = NULL;
    getClosestObject(viewRay, &obj, bestTime);
    point<3> intersection = viewRay(bestTime);

    // no object was intersected and the ray has left the scene. return this color.
    if(obj == NULL)
        return Color(0,0,0.25);

    Color retColor(0.0, 0.0, 0.0);
    Properties objProp = obj->getProperties( intersection );
    vectre<3> unit_normal = obj->normal_vectre( intersection).unit_vectre();
    if( unit_normal.dot_prod(viewRay.dir) > 0.0 )
        unit_normal = unit_normal * -1.0;

    vectre<3> up(0,1,0);
    vectre<3> right = up.cross_prod(unit_normal).unit_vectre();
    up = right.cross_prod(unit_normal);

    double theta = ((double)rand() / (double)RAND_MAX) * (M_PI / 2.0);
    double thi = ((double)rand() / (double)RAND_MAX) * (2.0 * M_PI);
    double x = sin(theta) * cos(thi);
    double y = sin(theta) * sin(thi);
    double z = cos(theta);

    ray<3> random_ray;
    random_ray.dir = x * right + y * up + z * unit_normal;
    random_ray.orig = intersection;

    Color lighting = pathtraceColor(random_ray, depth + 1);
    retColor += objProp.color * lighting * unit_normal.dot_prod(random_ray.dir);
    retColor += objProp.emittance;

    return retColor;
}


//********************** Private Helper Functions **************************
/**
 * finds the closest object and time to get there by the ray in the object list.
 * viewRay the ray to test
 * closestObject the index of the current closest object (gets modified)
 * bestTime the current best time of the closest object
 */
void Raytracer::getClosestObject(const ray<3>& viewRay, Drawable **closestObj, double& bestTime)
{
    vector<Drawable *> allObj = generateObjectList(viewRay);

    for(unsigned int i = 0; i < allObj.size(); ++i)
    {
        if( allObj[i]->intersects(viewRay) )
        {
            double currentTime = allObj[i]->intersection(viewRay);
            if(bestTime < 0.0 || currentTime < bestTime)
            {
                // Store the time of intersection and the object intersected.
                bestTime = currentTime;
                *closestObj = allObj[i];
            }
        }
    }
}

vector<Drawable *> Raytracer::generateObjectList(const ray<3> viewRay) {
    vector<Drawable *> allObj = objList;

    for(unsigned int i = 0; i < boundries.size(); i++) {
        Drawable *tmp = boundries[i].intersectionObj(viewRay);
        if(tmp != NULL)
            allObj.push_back(tmp);
    }
    return allObj;
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

bool Raytracer::intersectsObject( const ray<3> &viewRay ){
    for(unsigned int j = 0; j < objList.size(); j++){
        if(objList[j]->intersects(viewRay))
        {
            return true;
        }
    }
    return false;
}

double Raytracer::attenuation(const double dist) {
    //return 1.0;
    return 1.0 / (0.01 + 0.03 * dist);
}

Color Raytracer::blinnPhong(const ray<3> &viewRay, const vectre<3> &unit_normal,
        const ray<3> lightRay, const Color lightColor, const Properties &objProp){

    double viewProj = viewRay.dir.dot_prod(unit_normal);
    double lightProj = lightRay.dir.dot_prod(unit_normal);
    vectre<3> blinnDir = lightRay.dir - viewRay.dir;
    double temp = blinnDir.dot_prod(blinnDir);
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
