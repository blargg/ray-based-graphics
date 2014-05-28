/*================================================================
 *   epng.h
 *
 *   EasyPNG: a simple C++ png library using libpng as a backend  
 *    Created by Chase Geigle,                                    
 *       using zarb.org/~gc/html/libpng.html as a reference       
 *                                                                
 *   History:                                                     
 *       - created 01/07/2012
 *================================================================*/

#ifndef EASYPNG_H
#define EASYPNG_H

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include <string>
#include <iostream>
#include <sstream>

class RGBAPixel
{
	public:
		// storage
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;

		// operators
		bool operator==(RGBAPixel const & other) const;
		bool operator!=(RGBAPixel const & other) const;
		bool operator<(RGBAPixel const & other) const;

		// constructors
		RGBAPixel();
		RGBAPixel(unsigned char red, unsigned char green, unsigned char blue);
		RGBAPixel(unsigned char red, unsigned char green, unsigned char blue,
				unsigned char alpha);

};

std::ostream & operator<<(std::ostream & out, RGBAPixel const & pixel);

using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

class PNG
{
	public:
		// constructors
		PNG();
		PNG(int width, int height);
		PNG(string const & file_name);
		PNG(PNG const & other);

		// destructor
		~PNG();

		// operators
		PNG const & operator=(PNG const & other);
		bool operator==(PNG const & other) const;
		bool operator!=(PNG const & other) const;
		RGBAPixel * operator()(int x, int y);
		RGBAPixel const * operator()(int x, int y) const;

		// utility functions
		bool readFromFile(string const & file_name);
		bool writeToFile(string const & file_name);

		// getters
		int width() const;
		int height() const;

		// "setters" (though they are more destructive than changing one field)
		void resize(int width, int height);

	private:
		// storage
		int _width;
		int _height;
		RGBAPixel * _pixels;

		// private helper functions
		bool _read_file(string const & file_name);
		void _clear();
		void _copy(PNG const & other);
		void _blank();
		void _init();
		void _min_clamp_x(int & width) const;
		void _min_clamp_y(int & height) const;
		void _min_clamp_xy(int & width, int & height) const;
		void _clamp_xy(int & width, int & height) const;
		RGBAPixel & _pixel(int x, int y) const;
};

#endif // EASYPNG_H

