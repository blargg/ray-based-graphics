#include "file_loader/cam_loader.h"

#include <iostream>
#include <fstream>
#include <string>

#include "util/log.h"

using std::string;

Camera loadCameraFromFile(string filename) {
    string line;
    Camera cam;

    std::ifstream fs(filename.c_str());
    LOG_IF_W(!fs, "Cannot load %s", filename.c_str());

    while (getline(fs, line)) {
        std::istringstream iss(line);
        string command;
        iss >> command;

        LOG_D("line: %s", line.c_str());
        if (command.compare("location") == 0) {
            float x, y, z;
            iss >> x >> y >> z;
            cam.position.orig = Vector4d(x, y, z, 1.0);
            LOG_D("location: %f, %f, %f", x, y, z);
        } else if (command.compare("direction") == 0.0) {
            float x, y, z;
            iss >> x >> y >> z;
            cam.position.dir = Vector4d(x, y, z, 0.0);
            LOG_D("direction: %f, %f, %f", x, y, z);
        } else if (command.compare("up") == 0.0) {
            float x, y, z;
            iss >> x >> y >> z;
            cam.up = Vector4d(x, y, z, 0.0);
            LOG_D("up: %f, %f, %f", x, y, z);
        } else if (command.compare("dimensions") == 0.0) {
            double w, h;
            iss >> w >> h;
            cam.worldWidth = w;
            cam.worldHeight = h;
            LOG_D("width X height: %f, %f", w, h);
        } else {
            LOG_D("undefined command %s", line.c_str());
        }
    }

    return cam;
}
