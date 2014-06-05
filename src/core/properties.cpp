#include "core/properties.h"

Properties operator*(const Properties& p, const double x)
{
	Properties ret(p);
	ret *= x;
	return ret;
}

Properties operator*(const double x, const Properties& p)
{
	Properties ret(p);
	ret *= x;
	return ret;
}
Properties operator+(const Properties& prop1, const Properties& prop2)
{
	Properties ret(prop1);
	ret += prop2;
	return ret;
}
