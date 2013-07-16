#include "simpleObject.h"

SimpleObject::SimpleObject()
{
	shape = new Sphere();
	texture = new SolidColor();
}

SimpleObject::SimpleObject(const Shape& s, const Material& mat)
{
	shape = s.clone();
	texture = mat.clone();
}

SimpleObject::SimpleObject(const SimpleObject & other)
{
	copy(other);
}

void SimpleObject::copy(const SimpleObject& other)
{
	assert(shape != NULL && texture != NULL);
	delete shape;
	shape = other.shape->clone();

	delete texture;
	texture = other.texture->clone();
}

SimpleObject& SimpleObject::operator=(SimpleObject const & rhs)
{
	// If this object and rhs are the same object, do nothing.
	if(this == &rhs)
		return *this;

	// otherwise just copy rhs
	copy(rhs);
	return *this;
}

SimpleObject::~SimpleObject()
{
	assert(shape != NULL && texture != NULL);
	delete shape;
	delete texture;
}

double SimpleObject::intersection(const ray viewRay) const
{
	assert(shape != NULL);
	return shape->intersection(viewRay);
}

Vector4d SimpleObject::normal_vectre(const Vector4d surface) const
{
	assert(shape != NULL);
	return shape->normal_vectre(surface);
}

Properties SimpleObject::getProperties(const Vector4d location) const
{
	assert(texture != NULL);
	return texture->getProperties(location);
}

double SimpleObject::getMinBound(int axis) const {
    return shape->getMinBound(axis);
}

double SimpleObject::getMaxBound(int axis) const {
    return shape->getMaxBound(axis);
}

bool SimpleObject::intersectsBox(AABB box) const {
    return shape->intersectsBox(box);
}

void SimpleObject::setShape(const Shape& s)
{
	assert(shape != NULL);
	delete shape;
	shape = s.clone();
}

void SimpleObject::setMaterial(const Material& mat)
{
	assert(texture != NULL);
	delete texture;
	texture = mat.clone();
}
