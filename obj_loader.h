#ifndef LOADER_H
#define LOADER_H


#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Dense>
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

    void load_to_list(vector<Drawable*> &objList, const char * filename);

private:
    vector<Vector4d> verts;
    Properties curProp;
};

#endif // LOADER_H
