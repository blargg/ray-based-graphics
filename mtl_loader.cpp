
#include "mtl_loader.h"
#include <iostream>
#include <fstream>
#include "properties.h"

MtlLoader::MtlLoader() {
    unsetData = true;
};

MtlLoader::~MtlLoader() { /* do nothing */ };

void MtlLoader::add_to_map(map<string, SolidColor> &map, string filename) {
    string line;
    string command;

    std::ifstream fs(filename.c_str());
    if(!fs) {
        std::cerr << "MtlLoader: Cannot load " << filename << ". File does not exist." << std::endl;
        return;
    }

    while(getline(fs, line)){
        std::istringstream iss(line);

        if(line[0] == '#')
            continue;

        iss >> command;
        if(command.compare("newmtl") == 0){
            if(!unsetData) {
                pushMat(map);
            }
            iss >> name;
            unsetData = false;
        }
        else if(command.compare("Ns") == 0){
            iss >> curProp.spec_power;
        }
        else if(command.compare("Ka") == 0) {
            double r, g, b;
            iss >> r >> g >> b;
            curProp.emittance = Color(r,g,b);
        }
        else if(command.compare("Kd") == 0) {
            double r, g, b;
            iss >> r >> g >> b;
            curProp.color = Color(r,g,b);
        }
        else if(command.compare("Ks") == 0) {
            double r, g, b;
            iss >> r >> g >> b;
            curProp.specular = Color(r,g,b);
        }
        else if(command.compare("d") == 0 ||
                command.compare("Tr") == 0) {
            iss >> curProp.tranparency;
        }
    }

    pushMat(map);
}

void MtlLoader::pushMat(map<string, SolidColor> &map) {
    map[name] = SolidColor(curProp);
}
