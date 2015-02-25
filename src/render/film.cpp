#include "render/film.h"
#include <algorithm>

using std::min;
using std::max;

Film::Film(int width, int height) {
    this->height = height;
    this->width = width;
    color_grid = new Color [width * height];
    count_grid = new double [width * height];
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
