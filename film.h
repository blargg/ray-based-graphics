#ifndef RT_FILM_H
#define RT_FILM_H

#include <string>
#include "common.h"
#include "color.h"
#include "easypng.h"

class Film {
    public:
        Film(int width, int height);
        /// Adds color contribution to pixel (x,y)
        void addColor(Color c, int x, int y);
        PNG writeImage();
    private:
        Color *color_grid;
        int *count_grid;
        int height;
        int width;

        RGBAPixel makePixel(Color c);
        int getIndex(int x, int y) const;
};

#endif // RT_FILM_H
