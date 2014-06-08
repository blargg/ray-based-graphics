#ifndef TURBULENT_H
#define TURBULENT_H

#include "materials/perlin.h"
#include "materials/material.h"
#include "common.h"

class Turbulent: public Material
{
	private:
	Properties prop1;
	Properties prop2;

	//Perlin noise;

	public:
	Turbulent(Properties firstProp  = Properties(Color(1.0, 0.0, 0.0)),
			  Properties secondProp = Properties(Color(0.0, 1.0, 0.0))):
			  prop1(firstProp),prop2(secondProp)
			  { /* do nothing */ }
	/// Returns a Material pointer to a new Turbulent
	virtual Material * create() const;

	/// Returns a Material pointer to a copy of this Turbulent
	virtual Material * clone() const;

	/// Returns the material properties for a given point in space
	virtual Properties getProperties(Vector4d loc) const;
};

#endif //TURBULENT_H
