#ifndef RT_FILM_H
#define RT_FILM_H

#include <string>
#include "core/common.h"
#include "core/color.h"
#include "core/easypng.h"

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

        /// Get the height of the image in pixels
        int getHeight();
        /// Get the width of the image in pixels
        int getWidth();

        /**
         * Converts the film into a PNG image.
         * @returns the PNG constructed
         */
        PNG writeImage();

        /**
         * Writes the film to the file given by filename
         */
        void writeFile(std::string filename);
    private:
        Color *color_grid;
        int *count_grid;
        int height;
        int width;

        RGBAPixel makePixel(Color c);
        int getIndex(int x, int y) const;
};

#endif // RT_FILM_H
