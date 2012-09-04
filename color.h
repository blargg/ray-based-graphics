#ifndef COLOR_H
#define COLOR_H

/// \todo This file structure should really be cleaned up. Make this into color.h color.cpp and structs.h

/**
 * @brief Simple RGBA color struct.
 *
 * Represents colors as four doubles (one of each RGBA channel).
 * The real color values range from 0.0 to 1.0
 *
 * NOTE:  
 * In the ray tracing algorithm, it is at times necessary to have RGB vales
 * greater than 1.0.
 */
struct Color
{
	/**
	 * @brief red channel
	 */
	double red;

	/**
	 * @brief green channel
	 */
	double green;

	/**
	 * @brief blue channel
	 */
	double blue;

	/**
	 * @brief Constructs a black fully opaque color
	 */
	Color():red(0.0),green(0.0),blue(0.0){ /* Do nothing */ }

	/**
	 * @brief Constructs a color from the given R G B A values
	 *
	 * @param red red value
	 * @param green green value
	 * @param blue blue value
	 * @param alpha alpha value. Defaults to 1.0 (fully opaque).
	 */
	Color(double r,double g,double b):red(r),green(g),blue(b){ /* Do nothing */ }

	/// Multiplication operator for Color
	inline Color& operator*=(const double x)
	{
		red   *= x;
		green *= x;
		blue  *= x;
		return *this;
	}

	inline Color& operator*=(const Color& other)
	{
		red    *=  other.red;
		green  *=  other.green;
		blue   *=  other.blue;
		return *this;
	}

	inline Color operator*(const Color& other)
	{
		Color retColor(*this);
		retColor *= other;
		return retColor;
	}

	inline Color& operator+=(const Color& rhs)
	{
		red   += rhs.red;
		green += rhs.green;
		blue  += rhs.blue;
		return *this;
	}
};

// Global Scope operators for Color

inline Color operator*(const Color& theColor, const double x)
{
	Color retColor = theColor;
	retColor *= x;
	return retColor;
}

inline Color operator*(const double x, const Color& theColor)
{
	Color retColor = theColor;
	retColor *= x;
	return retColor;
}




#endif // COLOR_H
