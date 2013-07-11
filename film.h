#ifndef RT_FILM_H
#define RT_FILM_H

#include <string>
#include "common.h"
#include "color.h"
#include "easypng.h"

/**
 * Manages collected image data.
 * Accumulates samples of colors for and image
 */
class Film {
    public:
        /**
         * Initializes the image for the given dimetions.
         */
        Film(int width, int height);
        /// Adds color contribution to pixel (x,y)
        void addColor(Color c, int x, int y);

        /**
         * Converts the film into a PNG image.
         * @returns the PNG constructed
         */
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
