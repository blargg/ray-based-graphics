#ifndef RTSTRUCTS_H
#define RTSTRUCTS_H

#include <vector>

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

/**
 * @brief A struct to hold all the properties a material can have.
 * These properties can have different values at different points
 * of the same material. The material itself decides what points in 
 * space have what properties.
 */
struct Properties
{
	/**
	 * @brief The color of the material
	 */
	Color color;

	/**
	 * @brief How reflective the material is.
	 * 0.0 is no reflection\n
	 * 1.0 is completely reflective
	 */
	double reflect;

	/**
	 * @brief Controls how transparent the object is.
	 * This only has to do with light passing throught the object, 
	 * light reflected off \n
	 * 0.0 allows no light to pass through the object
	 * 1.0 allows all the light to pass thought the object
	 */
	double tranparency;
	/**
	 * @brief The index of refraction for a material.
	 * Controls how light bends when it passes through
	 * the material.\n
	 * 1.33 is about the same as water.
	 */
	double i_refraction;

	/*
	 * @brief The color of the specular light reflected
	 */
	Color specular;

	/// The strenght of the specular light reflected.
	double spec_power;


	/**
	 * @brief Default constructor
	 * Sets the values as follows
	 * - color is set to the default color
	 * - reflect is set to 0.0 (no reflection)
	 * - i_refraction is set to 1.33 (water)
	 * - specular is set to white
	 * - spec_power is set to 1.0
	 */
	Properties():color(Color()),reflect(0.0),i_refraction(1.33),specular(0.7,0.7,0.7),spec_power(25.55){ /* Do nothing */ }

	Properties(Color cColor, double reflection = 0.0, double index_of_refraction = 1.33, Color specularVal = Color(0.7,0.7,0.7), double power = 25.55):
		color(cColor),reflect(reflection),i_refraction(index_of_refraction),specular(specularVal),spec_power(power)
	{ /* Do nothing */ }
};


#endif
