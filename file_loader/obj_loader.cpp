#include "obj_loader.h"

// TODO remove
#include <iostream>
#include <fstream>
#include <ios>
#include <cmath>
#include <assert.h>
#include <cerrno>

using std::string;

ObjLoader::ObjLoader() {
}

void ObjLoader::load_to_list(vector<Drawable*> &objList, std::string filename){
    string line;
    string command;

    std::ifstream fs(filename.c_str());
    if(!fs) {
        std::cerr << "ObjLoader: Cannot load " << filename << " does not exist." << std::endl;
    }
    string baseDir = filename.substr(0, filename.find_last_of('/') + 1);
    while(getline(fs, line)){
        std::istringstream iss(line);

        if(line[0] == '#')
            continue;

        iss >> command;
        if(command.compare("v") == 0){
            float vert[3];
            iss >> vert[0] >> vert[1] >> vert[2];

            verts.push_back(Vector4d(vert[0],vert[1],vert[2], 1.0));
        }
        else if(command.compare("f") == 0){
            int a,b,c;
            iss >> a >> b >> c;

            // offset, so it's in a 0 based numbering like std::vector
            a--;
            b--;
            c--;

            objList.push_back(new SimpleObject(Triangle(verts[a],verts[b],verts[c]),
                                               curMaterial));
        }
        else if(command.compare("mtllib") == 0) {
            string file;
            iss >> file;
            file = baseDir + file;
            mloader.add_to_map(materials, file);
        }
        else if(command.compare("usemtl") == 0) {
            string materialName;
            iss >> materialName;
            curMaterial = materials[materialName];
        }
    }
}
