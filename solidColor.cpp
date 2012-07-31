#include "solidColor.h"

SolidColor::SolidColor():myProperties(Color(0.0,1.0,0.0))
{ /* Do nothing */ }

SolidColor::SolidColor(Properties p):myProperties(p)
{ /* do nothing */ }

Material * SolidColor::create() const
{
	return new SolidColor();
}

Material * SolidColor::clone() const
{
	return new SolidColor(*this);
}

Properties SolidColor::getProperties(point<3> loc) const
{
	return myProperties;
}



// none of the values change based on the location, so just return them then
/*
color SolidColor::getColor(point<3> loc) const
{
	return myColor;
}

double SolidColor::getReflection(point<3> loc) const
{
	return reflect;
}

double SolidColor::getIndexOfRefraction(point<3> loc) const
{
	return i_refraction;
}
*/
