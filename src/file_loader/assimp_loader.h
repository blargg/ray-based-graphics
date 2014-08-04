#ifndef ASSIMP_LOADER_H
#define ASSIMP_LOADER_H

#include <string>
#include <vector>
#include <Eigen/Dense>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "drawables/drawable.h"
#include "drawables/simpleObject.h"
#include "shapes/triangle.h"
#include "materials/material.h"
#include "materials/solidColor.h"
#include "render/camera.h"

// uses some default options
const aiScene *getScene(std::string filename);

// adds the basic geometry data of the scene to the list given
void assimp_append(const aiScene *sc, std::vector<Drawable*> &list,
        std::vector<Drawable*> &lights);

// gets the first camera in the scene
Camera assimp_getCamera(const aiScene *sc);

Matrix4d convert_matrix(aiMatrix4x4 orig);
Color convert_color(aiColor3D original);


#endif // ASSIMP_LOADER_H
