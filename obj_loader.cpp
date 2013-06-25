#include "obj_loader.h"

// TODO remove
#include <iostream>
#include <fstream>
#include <ios>
#include <cmath>
#include <assert.h>
#include <cerrno>

using std::string;
using std::cout;

ObjLoader::ObjLoader() {
    curProp.color = Color(1,0.5,0.5);
    //curProp.specular = Color(0.5,0.5,0.5);
    curProp.emittance = Color(0,0,0);
}

void ObjLoader::load_to_list(vector<Drawable*> &objList, const char * filename){
    string line;
    string command;

    std::ifstream fs(filename);
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
                SolidColor(curProp)));
        }
    }
}
