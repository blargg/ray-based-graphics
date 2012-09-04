#ifndef MATERIAL_H
#define MATERIAL_H

#include "properties.h"
#include "point.h"

/**
 * @brief An interface to get matirial properties at a given point in 3d space
 *
 * abstract class definition for a material,
 * which defines properties like opacity and color
 * and used to make the "texture" of an object
 */
class Material
{
	public:
	/**
	 * @brief Destructor for the class
	 * The destructor does nothing, but allows other classes to 
	 * overwrite later.
	 */
	virtual ~Material() { /* Do nothing */ }

	/**
	 * @brief a virtual default constructor. Allows you to create more of the object.
	 * In the classes that extend this one, this method must return a pointer to 
	 * a new default object of it's (the child class's) type.
	 */
	virtual Material * create() const = 0;

	/**
	 * @brief Overwrite to provide a sort of copy constructor.
	 * Makes a new copy of the current object and returns the pointer.
	 *
	 * @returns A pointer to the copy.
	 */
	virtual Material * clone() const = 0;

	/**
	 * @brief Returns the properties of the material at the given point.
	 * @param loc The location for the given properties.
	 */
	virtual Properties getProperties(point<3> loc) = 0;

	/*
	 * @brief returns the color of the material at a given point in 3-space
	 * Takes a point in 3-space and returns the color of this material at that
	 * point.
	 *
	 * @param loc The point in space to get the color of.
	 * @returns The color of the material at that point in space
	 */
	/*
	virtual color getColor(point<3> loc) const = 0;
	virtual double getReflection(point<3> loc) const = 0;
	virtual double getIndexOfRefraction(point<3> loc) const = 0;
	*/

};

#endif
