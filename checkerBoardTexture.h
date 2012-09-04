#ifndef CHECKE_BOARD_TEXTURE_H
#define CHECKE_BOARD_TEXTURE_H

#include "material.h"

/**
 * @brief A material of to different properties. Alternates between the two properties in a grid-like pattern
 *
 * Essentially, this texture covers all space in a cubes. The properties of each cube alternates when moving to an adjacent one.
 */
class CheckerBoardTexture: public Material
{
	private:
	/// The first set of properties to use
	Properties prop1;
	/// The second set.
	Properties prop2;

	/// The size of one length of the cube.
	double size;

	public:

	/**
	 * @brief Constructs the material from the given properties and size
	 *
	 * @param firstProp One of the properties of the pattern. Defaults to red.
	 * @param secondProp The other set of properties of the pattern. Defaults to blue.
	 * @param scale The size of the cube pattern. Defaults to 10.0
	 */
	CheckerBoardTexture(Properties firstProp = Properties(Color(1.0,0.0,0.0)),
			   			Properties secondProp = Properties(Color(0.0,0.0,1.0)), double scale = 10.0):
						prop1(firstProp),prop2(secondProp),size(scale)
	{ /* Do nothing */ }

	/// Allows the material to be constructed virtually.
	virtual Material * create() const;
	/// Allows the material to be copied virtually.
	virtual Material * clone() const;
	/// Returns the properties of the material at the given point.
	virtual Properties getProperties(point<3> loc) const;
};

#endif
