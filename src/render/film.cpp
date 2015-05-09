#include "render/film.h"
#include "util/log.h"
#include <algorithm>

using std::min;
using std::max;

Film::Film(int width, int height) {
    this->height = height;
    this->width = width;
    color_grid.reserve(width * height);
    for(int i = 0; i < width * height; i++) {
        color_grid.push_back(Color(0, 0, 0));
    }
    count_grid.reserve(width * height);
    for (int i = 0; i < width * height; i++) {
        count_grid.push_back(0.0);
    }
}

void Film::addColor(Color c, int x, int y) {
    addColorWeighted(c, x, y, 1.0);
}

void Film::addColorWeighted(Color c, int x, int y, double weight) {
    color_grid[getIndex(x,y)] += weight * c;
    count_grid[getIndex(x,y)] += weight;
}

PNG Film::writeImage() {
    PNG output(width, height);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            Color imgColor = color_grid[getIndex(x,y)];
            imgColor *= (1.0 / count_grid[getIndex(x,y)]);
            (*output(x,height - y - 1)) = makePixel(imgColor);
        }
    }
    return output;
}

void Film::writeFile(std::string filename) {
    PNG pic = writeImage();
    pic.writeToFile(filename);
}

void Film::addSamples(Film other) {
    LOG_IF_W(height != other.height, "combining films with different dimensions");
    LOG_IF_W(width != other.width, "combining films with different dimensions");
    int x = std::min(height, other.height);
    int y = std::min(width, other.width);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            color_grid[getIndex(i, j)] +=
                other.color_grid[other.getIndex(i, j)];
            count_grid[getIndex(i, j)] +=
                other.count_grid[other.getIndex(i, j)];
        }
    }
}

RGBAPixel Film::makePixel(Color c) {
    return RGBAPixel(max(min(c.red * 255, 255.), 0.),
            max(min(c.green * 255, 255.), 0.),
            max(min(c.blue * 255, 255.), 0.));
}

int Film::getIndex(int x, int y) const {
    return x + y * width;
}

int Film::getHeight() {
    return height;
}

int Film::getWidth() {
    return width;
}
