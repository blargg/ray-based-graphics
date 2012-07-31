
/*** CONSTRUCTORS ***/ 
template <int DIM>
point<DIM>::point()
{
	for(int i = 0; i < DIM; i++)
	{
		vals[i] = 0.0;
	}
}

template <int DIM>
point<DIM>::point(double a[DIM])
{
	for(int i = 0; i < DIM; i++)
		vals[i] = a[i];
}

template <int DIM>
point<DIM>::point(double x, ...)
{
	vals[0] = x;
	va_list ap;
	va_start(ap,x);
	for(int i = 1; i < DIM; i++)
		vals[i] = va_arg(ap, double);
	va_end(ap);
}

template<int DIM>
point<DIM>::point(double x, double y, double z)
{
	assert(DIM >= 3);
	vals[0] = x;
	vals[1] = y;
	vals[2] = z;
}




/*** Access Operators ***/
template <int DIM>
double& point<DIM>::operator[](int index)
{
	if(index >= DIM)
	{
		std::out_of_range e("Point index out of range");
		throw e;
	}
	return vals[index];
}

template <int DIM>
double point<DIM>::operator[](int index) const
{
	if(index >= DIM)
	{
		std::out_of_range e("Point index out of range");
		throw e;
	}
	return vals[index];
}






/*** Equality Operations ***/
template <int DIM>
bool point<DIM>::operator!=(const point<DIM> other) const
{
	//if any of the values are different, return true immediately
	//otherwise return false;
	for(int i = 0; i < DIM; i++)
	{
		if(vals[i] != other.vals[i])
			return true;
	}
	return false;
}

template <int DIM>
bool point<DIM>::operator==(const point<DIM> other) const
{
	return !operator!=(other);
}






/*** IOSTREAM ***/
template <int DIM>
void point<DIM>::print(std::ostream & out) const
{
	out << '(';

	for (int i = 0; i < DIM - 1; i++)
		out << vals[i] << ", ";

	out << vals[DIM - 1];
	out << ')';
}

template<int DIM>
std::ostream & operator<<(std::ostream & out, const point<DIM> & p)
{
	p.print(out);
	return out;
}
