#ifndef SOLIDCOLOR_H
#define SOLIDCOLOR_H

#include "../material.h"
#include "../properties.h"
#include "../common.h"
#include <Eigen/Dense>

using namespace Eigen;

/**
 * @brief A type of Material with the same properties (color, reflectiveness, etc.) at all points.
 *
 * Simply store the color reflection and index of refraction information and returns them no matter what location is called.
 */
class SolidColor: public Material
{
	private:
	Properties myProperties;

	public:
	/**
	 * @brief Default constructor for SolidColor
	 * Defaults to the color Green\n reflection = 0.0\n i_refraction = 1.33.
	 */
	SolidColor();

	/**
	 * @brief Basic constructor, allowing you to set the color and other properties
	 *
	 * This constructor allows you to set all the values of the SolidColor material.
	 *
	 * @param newColor The color of the material
	 * @param reflection_index How reflective the material is. (0 is no reflections, 1 is a perfect mirror)
	 * @param index_of_refraction Sets the index of refraction of the material which controls how light bends as it passes through the material. The default is 1.33, which is the same as water
	 */
	SolidColor(Properties p);

	virtual ~SolidColor() { /* Do nothing */ }

	/**
	 * @brief Overrides the Material's create() and makes the default SolidColor on the heap and returns the pointer.
	 * This is the virtual constructor. It can be called from a Material pointer and returns a pointer to a Material. 
	 * This allows you to use it when you are using polymorphism and you need a way to make a new object of the same type.
	 * Unfortunately, this requires the user of this class to manage memory. This method creates a new object
	 * that must be delete when appropriate.\n 
	 * Also, any class extending this one should override this method to return a default object of its own type
	 *
	 * @returns A pointer to the new object
	 */
	virtual Material * create() const;
	/**
	 * @brief Virtual copy constructor of the calling object.
	 *
	 * Overrides the Material clone() method to return an object of type SolidColor that is a copy of the calling object
	 * Used in situations that require polymorphism. This simply calls the copy constructor on itself and returns the pointer.
	 * This creates a new object on the stack, so the user of this class must delete the object when appropriate.
	 *
	 * @returns A pointer to a copy of the calling object. The actual type is a SolidColor.
	 */
	virtual Material * clone() const;

	/*
	virtual color getColor(point<3> loc) const;
	virtual double getReflection(point<3> loc) const;
	virtual double getIndexOfRefraction(point<3> loc) const;
	*/

	virtual Properties getProperties(Vector4d loc) const;
};

#endif
