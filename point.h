#ifndef POINT_H
#define POINT_H

#include <cstdarg>
#include <assert.h>
#include <iostream>
#include <stdexcept>

template <int DIM>
class point
{
	private:
	double vals[DIM];

	public:
	point();
	point(double a[DIM]);
	point(double x, ...);
	point(double x, double y, double z);

	double operator[](int index) const;
	double & operator[](int index);
	bool operator==(const point<DIM> other) const;
	bool operator!=(const point<DIM> other) const;

	void print(std::ostream & out) const;
};

template<int DIM>
std::ostream & operator<<(std::ostream & out, const point<DIM> & p);

#include "point.cpp"

#endif
