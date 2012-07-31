
template <int DIM>
ray<DIM>::ray(const point<DIM>& origin, const vectre<DIM>& direction):orig(origin),dir(direction)
{ /* do nothing */ }

template <int DIM>
point<DIM> ray<DIM>::operator()(const double time) const
{
	vectre<DIM> scaled_vect = dir * time;
	point<DIM> ret;
	for(int i = 0; i < DIM; ++i)
		ret[i] = orig[i] + scaled_vect[i];

	return ret;
}
