#include "loader.h"

#include <Eigen/Dense>
#include "simpleObject.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/triangle.h"

#include "material.h"
#include "materials/solidColor.h"

#include "properties.h"
#include "light.h"
#include "ray.h"

using namespace Eigen;


Raytracer loadScene(const char *filename) {
    string line;
    std::ifstream fs(filename);

    Raytracer render;
    Properties currentProperties;
    currentProperties.color = Color(0.0, 1.0, 0.0);
    currentProperties.reflect = 0.3;
    currentProperties.specular = Color(0.0,0.0,0.0);
    currentProperties.spec_power = 0.0;


    while(getline(fs, line)){
        std::istringstream iss(line);
        if(line[0] == 'm'){
            char start;
            double v[3];
            iss >> start >> v[0] >> v[1] >> v[2];
            currentProperties.color = Color(v[0], v[1], v[2]);
        } else if (line[0] == 't') {
            char start;
            double a[3],b[3],c[3];
            iss >> start >>
                   a[0] >> a[1] >> a[2] >>
                   b[0] >> b[1] >> b[2] >>
                   c[0] >> c[1] >> c[2];
            Triangle tri(Vector4d(a[0], a[1], a[2], 1), Vector4d(b[0], b[1], b[2], 1), Vector4d(c[0], c[1], c[2], 1));
            SolidColor color(currentProperties);
            render.objList.push_back(new SimpleObject(tri, color));

        } else if(line[0] == 's') {
            char start;
            double r;
            double p[3];
            iss >> start >>
                   p[0] >> p[1] >> p[2] >> r;
            Sphere s(Vector4d(p[0], p[1], p[2], 1), r);
            render.objList.push_back(new SimpleObject(s, SolidColor(currentProperties)));
        } else if (line[0] == 'l') {
            char start;
            double p[3];
            double i;
            iss >> start >> p[0] >> p[1] >> p[2] >> i;
            Light l;
            l.location = Vector4d(p[0], p[1], p[2], 1);
            l.color = Color(i,i,i);
            render.lightList.push_back(l);
        }
    }

    return render;
}
