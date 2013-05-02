#include "film.h"
#include <algorithm>

using std::min;
using std::max;

Film::Film(int width, int height) {
    this->height = height;
    this->width = width;
    color_grid = new Color*[width];
    count_grid = new int*[width];

    for(int i=0; i < width; i++) {
        color_grid[i] = new Color[height];
        count_grid[i] = new int[height];
    }
}

void Film::addColor(Color c, int x, int y) {
    color_grid[x][y] += c;
    count_grid[x][y] += 1;
}

PNG Film::writeImage() {
    PNG output(width, height);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            Color imgColor = color_grid[x][y];
            imgColor *= (1.0 / count_grid[x][y]);
            (*output(x,y)) = makePixel(imgColor);
        }
    }
    return output;
}

RGBAPixel Film::makePixel(Color c) {
    return RGBAPixel(max(min(c.red * 255, 255.), 0.),
            max(min(c.green * 255, 255.), 0.),
            max(min(c.blue * 255, 255.), 0.));
}
