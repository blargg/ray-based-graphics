#ifndef VECTRE_H
#define VECTRE_H

#include <cstdarg>
#include <assert.h>
#include <stdexcept>
#include <iostream>
#include <math.h>

#include "point.h"

/*
 * a vector class for mathmatical applications
 * behaves as a vector as defined mathematically
 * holds double values only
 *
 * SEE ALSO: vectre.cpp, point.h, point.cpp
 */
template <int DIM>
class vectre
{
	private:
	double vals[DIM];

	public:
	/*** Constructors ***/
	vectre();
	vectre(double array[DIM]);
	vectre(double x, ...);
	vectre(double x, double y, double z);
	vectre(point<DIM> start, point<DIM> end);
	vectre(point<DIM> endpoint);

	/*** Mathematical Operations ***/
	double dot_prod(const vectre<DIM> other) const;
	vectre<DIM> cross_prod(const vectre<DIM> other) const;

	double length() const;
	double length_sq() const;
	vectre<DIM> unit_vectre() const;

		//operators
	vectre<DIM>& operator+=(const vectre<DIM>& rhs);
	vectre<DIM> operator+(const vectre<DIM>& rhs) const;
	vectre<DIM>& operator-=(const vectre<DIM>& rhs);
	vectre<DIM> operator-(const vectre<DIM>& rhs) const;
	vectre<DIM>& operator*=(const double x);

	bool operator==(const vectre<DIM> other) const;
	bool operator!=(const vectre<DIM> other) const;

	/*** Access Operators ***/
	double& operator[](int index);
	double operator[](int index) const;

	int size() const;


	void print(std::ostream & out) const;
};

// declare the functions that need global scope here
template <int DIM>
std::ostream & operator<<(std::ostream & out, const vectre<DIM> vect);

template <int DIM>
vectre<DIM> operator*(const vectre<DIM> vect, const double x);

template <int DIM>
vectre<DIM> operator*(const double x, const vectre<DIM> vect);
#include "vectre.cpp"

#endif
