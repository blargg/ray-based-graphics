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
#include "drawables/simpleObject.h"

void assimp_append(std::string filename, std::vector<Drawable*> &list);
Matrix4d convert_matrix(aiMatrix4x4 orig);

#endif // ASSIMP_LOADER_H
