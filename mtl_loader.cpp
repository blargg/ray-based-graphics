
#include "mtl_loader.h"
#include <iostream>
#include <fstream>
#include "properties.h"

MtlLoader::MtlLoader() {
    unsetData = true;
};

MtlLoader::~MtlLoader() { /* do nothing */ };

void MtlLoader::add_to_map(map<string, SolidColor> &map, const char * filename) {
    string line;
    string command;

    std::ifstream fs(filename);
    while(getline(fs, line)){
        std::istringstream iss(line);

        if(line[0] == '#')
            continue;

        iss >> command;
        if(command.compare("newmtl") == 0){
            if(!unsetData) {
                pushMat(map);
            }
            string newName;
            iss >> newName;
            name = newName;
            unsetData = false;
        }
        else if(command.compare("Ns") == 0){
            double hardness;
            iss >> hardness;
            spec_hardness = hardness;
        }
        else if(command.compare("Ka") == 0) {
            double r, g, b;
            iss >> r >> g >> b;
            ambient = Color(r,g,b);
        }
        else if(command.compare("Kd") == 0) {
            double r, g, b;
            iss >> r >> g >> b;
            diffuse = Color(r,g,b);
        }
        else if(command.compare("Ks") == 0) {
            double r, g, b;
            iss >> r >> g >> b;
            specular = Color(r,g,b);
        }
        else if(command.compare("d") == 0 ||
                command.compare("Tr") == 0) {
            double d;
            iss >> d;
            dissolve = d;
        }
    }

    pushMat(map);
}

void MtlLoader::pushMat(map<string, SolidColor> &map) {
    Properties p;
    p.color = diffuse;
    p.specular = specular;
    p.spec_power = spec_hardness;
    p.emittance = ambient;
    p.i_refraction = index_refraction;
    p.tranparency = dissolve;

    map[name] = SolidColor(p);
}
