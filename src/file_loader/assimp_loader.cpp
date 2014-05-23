#include "file_loader/assimp_loader.h"

#include <stdio.h>

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

Vector4d getPoint(const aiMesh *mesh, unsigned int index) {
    aiVector3D vertex = mesh->mVertices[index];
    return Vector4d(
            vertex[0],
            vertex[1],
            vertex[2],
            1.0
            );
}

void processFace(const aiMesh *mesh, unsigned int index, vector<Drawable *> &list) {
    aiFace face = mesh->mFaces[index];
    if (face.mNumIndices != 3) {
        printf("assimp_loader: cannot handle arbitrary polygons\n");
        return;
    }

    Vector4d a = getPoint(mesh, face.mIndices[0]);
    Vector4d b = getPoint(mesh, face.mIndices[1]);
    Vector4d c = getPoint(mesh, face.mIndices[2]);

    Properties prop;
    prop.color = Color(0.3, 0.05, 0.5);
    prop.i_refraction = 1.33;
    prop.emittance = Color(0,0,0);
    prop.reflect = 0.0;
    prop.specular = Color(0.0, 0.0, 0.0);
    prop.spec_power = 10.0;
    prop.tranparency = 1.0;

    SolidColor mat(prop);
    list.push_back(
            new SimpleObject(
                Triangle(a, b, c),
                mat));

}

void processMesh(const aiMesh *mesh, vector<Drawable *> &list) {
    for (int f = 0; f < mesh->mNumFaces; f++) {
        processFace(mesh, f, list);
    }
}

void recurse(const aiScene *sc, const aiNode *nd, vector<Drawable *> &list) {
    aiMatrix4x4 tmp = nd->mTransformation;
    Matrix4d m = convert_matrix(tmp);
    for (int i = 0; i < sc->mNumMeshes; i++) {
        processMesh(sc->mMeshes[i], list);
    }
}

void assimp_append(std::string filename, std::vector<Drawable *> &list) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile( filename,
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices);

    if (!scene) {
        printf("Assimp: could not load scene\n");
    }

    recurse(scene, scene->mRootNode, list);
}
