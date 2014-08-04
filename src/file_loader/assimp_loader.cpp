#include "file_loader/assimp_loader.h"

#include <stdio.h>
#include <algorithm>
#include "util/log.h"

using std::vector;

Matrix4d convert_matrix(aiMatrix4x4 tmp) {
    Matrix4d m;
    m
        << tmp.a1
        , tmp.a2
        , tmp.a3
        , tmp.a4
        , tmp.b1
        , tmp.b2
        , tmp.b3
        , tmp.b4
        , tmp.c1
        , tmp.c2
        , tmp.c3
        , tmp.c4
        , tmp.d1
        , tmp.d2
        , tmp.d3
        , tmp.d4;
    return m;
}

Color convert_color(aiColor3D original) {
    return Color(
            original.r,
            original.g,
            original.b);
}

SolidColor getMaterial(const aiScene *sc, unsigned int index) {
    Properties prop;
    aiMaterial *mat = sc->mMaterials[index];
    aiColor3D tmpColor(0.0, 0.0, 0.0);

    // diffuse
    if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_DIFFUSE, tmpColor)) {
        printf("problem with diffuse color\n");
    }
    prop.color = convert_color(tmpColor);

    // specular
    if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_SPECULAR, tmpColor)) {
        printf("problem with specular color\n");
    }
    prop.specular = convert_color(tmpColor);

    // emissive
    if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_EMISSIVE, tmpColor)) {
        printf("problem with emissive color\n");
    }
    prop.emittance = convert_color(tmpColor);

    // transparency
    if (AI_SUCCESS != mat->Get(AI_MATKEY_OPACITY, prop.tranparency)) {
        printf("problem with transparency\n");
    }

    return SolidColor(prop);
}

bool materialIsEmmissive(const aiScene *sc, unsigned int index) {
    aiMaterial *mat = sc->mMaterials[index];
    aiColor3D tmpColor(0.0, 0.0, 0.0);
    if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_EMISSIVE, tmpColor)) {
        return false;
    }
    return vmax(tmpColor.r, tmpColor.g, tmpColor.b) > 0.01;
}

Vector4d getPoint(const aiMesh *mesh, unsigned int index) {
    aiVector3D vertex = mesh->mVertices[index];
    return Vector4d(
            vertex[0],
            vertex[1],
            vertex[2],
            1.0
            );
}

void processFace(const aiScene *sc, const aiMesh *mesh,
        unsigned int index, vector<Drawable *> &list,
        vector<Drawable *> &lights) {
    aiFace face = mesh->mFaces[index];
    LOG_IF(face.mNumIndices != 3, "cannot handle arbitrary polygons\n");

    Vector4d a = getPoint(mesh, face.mIndices[0]);
    Vector4d b = getPoint(mesh, face.mIndices[1]);
    Vector4d c = getPoint(mesh, face.mIndices[2]);

    SolidColor mat = getMaterial(sc, mesh->mMaterialIndex);
    list.push_back(
            new SimpleObject(
                Triangle(a, b, c),
                mat));

    if (materialIsEmmissive(sc, mesh->mMaterialIndex)) {
        lights.push_back(
                new SimpleObject(
                    Triangle(a, b, c),
                    mat));
    }

}

void processMesh(const aiScene *sc, const aiMesh *mesh,
        vector<Drawable *> &list, vector<Drawable *> &lights) {
    for (int f = 0; f < mesh->mNumFaces; f++) {
        processFace(sc, mesh, f, list, lights);
    }
}

void recurse(const aiScene *sc, const aiNode *nd, vector<Drawable *> &list,
        vector<Drawable *> &lights) {
    aiMatrix4x4 tmp = nd->mTransformation;
    Matrix4d m = convert_matrix(tmp);
    for (int i = 0; i < sc->mNumMeshes; i++) {
        processMesh(sc, sc->mMeshes[i], list, lights);
    }
}

void assimp_append(const aiScene *sc, vector<Drawable *> &list,
        vector<Drawable *> &lights) {
    recurse(sc, sc->mRootNode, list, lights);
}

const aiScene *getScene(std::string filename) {
    Assimp::Importer importer;

    importer.ReadFile( filename,
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices);

    const aiScene* scene = importer.GetOrphanedScene();
    if (!scene) {
        printf("Assimp: could not load scene\n");
    }

    return scene;
}

Camera assimp_getCamera(const aiScene *sc) {
    // TODO check if there are any cameras
    Camera cam;
    aiCamera *aiCam = sc->mCameras[0];

    // TODO implement
    return cam;
}
