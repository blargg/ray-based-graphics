#ifndef LOADER_H
#define LOADER_H


#include <sstream>
#include <string>
#include <vector>
#include <Eigen/Dense>

#include "mtl_loader.h"

#include "drawable.h"
#include "simpleObject.h"
#include "shapes/triangle.h"

#include "material.h"
#include "properties.h"
#include "materials/solidColor.h"

using std::vector;

class ObjLoader
{
public:
    ObjLoader ();
    virtual ~ObjLoader () {};

    void load_to_list(vector<Drawable*> &objList, std::string filename);

private:
    vector<Vector4d> verts;
    SolidColor curMaterial;
    MtlLoader mloader;
    map<string, SolidColor> materials;
};

#endif // LOADER_H
