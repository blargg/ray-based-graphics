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

#include "materials/material.h"
#include "properties.h"
#include "materials/solidColor.h"

using std::vector;

/**
 * A class for loading wavefront obj files.
 * Automatically Loads MTL information when needed
 * @see MtlLoader
 */
class ObjLoader
{
public:
    /**
     * Instantiates the loader.
     */
    ObjLoader ();
    virtual ~ObjLoader () {};

    /**
     * Loads a file into a vector.
     * Adds each drawable parsed from the file to the given vector.
     * @param objList the vector to add the drawables to.
     * @param filename a realative name to the file (assumes unix style)
     */
    void load_to_list(vector<Drawable*> &objList, std::string filename);

private:
    vector<Vector4d> verts;
    SolidColor curMaterial;
    MtlLoader mloader;
    map<string, SolidColor> materials;
};

#endif // LOADER_H
