/*
 * this should be all the definisions for the methods of the vectre class (see: vectre.h)
 */


/****************** CONSTRUCTORS *************/

/*
 * constructs a default vectre is one of all 0's
 */
template <int DIM>
vectre<DIM>::vectre()
{
	//TODO check if this really does anything (portability?)
	for(int i = 0; i < DIM; ++i)
		vals[i] = 0.0;
}

/*
 * makes a vectre using the values from the given array
 */
template <int DIM>
vectre<DIM>::vectre(double array[DIM])
{
	for(int i = 0; i < DIM; ++i)
		vals[i] = array[i];
}

template<int DIM>
vectre<DIM>::vectre(double x, ...)
{
	vals[0] = x;
	va_list ap;
	va_start(ap,x);
	for(int i = 1; i < DIM; ++i)
		vals[i] = va_arg(ap, double);
	va_end(ap);
}

template<int DIM>
vectre<DIM>::vectre(double x, double y, double z)
{
	vals[0] = x;
	vals[1] = y;
	vals[2] = z;
}

/*
 * makes a vectre between the two given points
 */
template <int DIM>
vectre<DIM>::vectre(point<DIM> start, point<DIM> end)
{
	for(int i = 0; i < DIM; ++i)
		//vals[i] = start[i] - end[i];
		vals[i] = end[i] - start[i];
}

/*
 * makes a vectre from the origin to the given endpoint
 */
template <int DIM>
vectre<DIM>::vectre(point<DIM> endpoint)
{
	for(int i = 0; i < DIM; ++i)
		vals[i] = endpoint[i];
}

/************* OPERATORS AND OPERATIONS *************/
/*
 * calculates and returns the dot product between the two vectors
 */
template <int DIM>
double vectre<DIM>::dot_prod(const vectre<DIM> other) const
{
	double ret_val = 0.0;
	for(int i = 0; i < DIM; ++i)
		ret_val += vals[i] * other.vals[i];

	return ret_val;
}

/*
 * calculates and returns the cross product
 *
 * only defined for 3 dimentional vectors (as far as I know)
 * so it will error for any other dimentions
 */
template <int DIM>
vectre<DIM> vectre<DIM>::cross_prod(const vectre<DIM> other) const
{
	if(DIM != 3)
	{
		std::domain_error e("cross_product is only defined on 3 dimentional vectors");
		throw e;
	}
	// calculate the i, j, and k components of the vectre
	vectre<DIM> ret;
	ret[0] = this->vals[1] * other.vals[2] - this->vals[2] * other.vals[1];
	ret[1] = this->vals[2] * other.vals[0] - this->vals[0] * other.vals[2];
	ret[2] = this->vals[0] * other.vals[1] - this->vals[1] * other.vals[0];
	return ret;
}

/*
 * returns the lenght of the vectre
 */
template <int DIM>
double vectre<DIM>::length() const
{
	// not sure if this is optimal...
	return sqrt(length_sq());
}

/*
 * returns the square of the length of the vectre
 * this is public in case the user doesn't want to 
 * use the sqrt() function
 */
template <int DIM>
double vectre<DIM>::length_sq() const
{
	//sum up all the squares of the lengths
	double len_sq = 0;
	for(int i = 0; i < DIM; ++i)
	{
		len_sq += vals[i] * vals[i];
	}
	return len_sq;
}

/*
 * returns the unit vectre (vectre of length 1) in the same dircetion
 */
template <int DIM>
vectre<DIM> vectre<DIM>::unit_vectre() const
{
	double myLength = length();
	double unit_values[DIM];
	for(int i = 0; i < DIM; ++i)
		unit_values[i] = vals[i] / myLength;

	return vectre<DIM>(unit_values);
}

/*
 * defines the += operator for the vectre class
 * add together the vectres by adding their components together
 */
template <int DIM>
vectre<DIM>& vectre<DIM>::operator+=(const vectre<DIM>& rhs)
{
	for(int i = 0; i < DIM; ++i)
		vals[i] += rhs.vals[i];

	return *this;
}

template <int DIM>
vectre<DIM> vectre<DIM>::operator+(const vectre<DIM>& rhs) const
{
	//TODO test to see if optimization can make this faster
	vectre<DIM> ret_vectre = *this;
	ret_vectre += rhs;
	return ret_vectre;
}

template <int DIM>
vectre<DIM>& vectre<DIM>::operator-=(const vectre<DIM>& rhs)
{
	for(int i = 0; i < DIM; ++i)
		vals[i] -= rhs.vals[i];

	return *this;
}

template <int DIM>
vectre<DIM> vectre<DIM>::operator-(const vectre<DIM>& rhs) const
{
	//TODO test to see if optimization can make this faster
	vectre<DIM> ret_vectre = *this;
	ret_vectre -= rhs;
	return ret_vectre;
}

template <int DIM>
vectre<DIM>& vectre<DIM>::operator*=(const double x)
{
	for(int i = 0; i < DIM; ++i)
		vals[i] *= x;
	return *this;
}

template <int DIM>
vectre<DIM> operator*(const vectre<DIM> vect, const double x)
{
	vectre<DIM> ret = vect;
	ret *= x;
	return ret;
}

template <int DIM>
vectre<DIM> operator*(const double x, const vectre<DIM> vect)
{
	vectre<DIM> ret = vect;
	ret *= x;
	return ret;
}

template <int DIM>
bool vectre<DIM>::operator==(const vectre<DIM> other) const
{
	return !operator!=(other);
}

template <int DIM>
bool vectre<DIM>::operator!=(const vectre<DIM> other) const
{
	for(int i = 0; i < DIM; ++i)
	{
		if(vals[i] != other.vals[i])
			return true;
	}
	return false;
}
/********************* Access Operators *******************/
template<int DIM>
double& vectre<DIM>::operator[](int index)
{
	if(index >= DIM)
	{
		std::out_of_range e("Vectre: index out of range");
		throw e;
	}
	return vals[index];
}

template<int DIM>
double vectre<DIM>::operator[](int index) const
{
	if(index >= DIM)
	{
		std::out_of_range e("Vectre: index out of range");
		throw e;
	}
	return vals[index];
}

template<int DIM>
int vectre<DIM>::size() const
{
	return DIM;
}

/************* IO functions *************/
/*
 * outputs the vectre onto the ostream
 */
template <int DIM>
void vectre<DIM>::print(std::ostream & out) const
{
	out << "<";
	for(int i = 0; i < DIM -1; ++i)
		out << vals[i] << ", ";

	out << vals[DIM - 1] << ">";
}

/*
 * GLOBAL SCOPE
 * defines how the operator<< works for printing out the vectre
 * simply calls the vectre's print method
 */
template <int DIM>
std::ostream & operator<<(std::ostream & out, const vectre<DIM> vect)
{
	vect.print(out);
	return out;
}
