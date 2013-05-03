#include "shape.h"

Shape::Shape()
{ /* Do Nothing */ }

Shape::Shape(const point<3> loc):location(loc)
{ /* do nothing */ }

Shape::Shape(const point<3> loc, const point<3> rot):location(loc),rotation(rot)
{ /* do nothing */ }

void Shape::move(const point<3>& newLoc)
{
	location = newLoc;
}

void Shape::move(const vectre<3>& dir)
{
	location[0] += dir[0];
	location[1] += dir[1];
	location[2] += dir[2];
}

point<3> Shape::getLocation() const
{
	return location;
}

void Shape::setRotation(const point<3>& newRotation)
{
	rotation = newRotation;
}

void Shape::rotate(const point<3>& angles)
{
	rotation[0] += angles[0];
	rotation[1] += angles[1];
	rotation[2] += angles[2];
}
