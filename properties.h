#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "color.h"

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
	 * \brief The color of the specular light reflected
	 */
	Color specular;

	/* \brief The strength of the specular light reflected.
	 * This is used for blinn-phong for simulating specular
	 * light reflection (I think). I actually want to 
	 * remove blinn-phong lighting, so hopefully this 
	 * will get depreciated.
	 */
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

	/// Scales the properties by x.
	Properties& operator*=(double x)
	{
		// just multiply by all the properties
		color *= x;
		reflect *= x;
		tranparency *= x;
		i_refraction *= x;
		//specular *= x;
		//spec_power *= x;
		return *this;
	}

	/// Adds the values of the properties together.
	Properties& operator+=(Properties other)
	{
		color += other.color;
		reflect += other.reflect;
		tranparency += other.tranparency;
		i_refraction += other.i_refraction;
		//specular += other.specular;
		//spec_power += other.spec_power;
		return *this;
	}
};

Properties operator*( const Properties& p, const double x);
Properties operator*(const double x, const Properties& p);

Properties operator+(const Properties& prop1, const Properties& prop2);

#endif // PROPERTIES_H
