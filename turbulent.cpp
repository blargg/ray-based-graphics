#include "turbulent.h"
#include <math.h>

Material* Turbulent::create() const
{ return new Turbulent(); }

Material* Turbulent::clone() const
{ return new Turbulent(*this); }

Properties Turbulent::getProperties(point<3> loc) 
{
	double noiseLevel = PerlinNoise3D(loc[0], loc[1], loc[2], 1.1, 0.5, 9);
	//double noiseLevel = Noise::noise(loc[0],loc[1],loc[2]);
	//double noiseLevel = noise.Get(loc);
	// \todo
	//double noiseLevel = (double) noise.Get(loc[0], loc[1]);
	/*
	double noiseLevel = 0.0;
	for(int i = 1; i < 25; ++i)
	{
		noiseLevel += (1.0 / i) * 
				 fabs(Noise::noise( i * 0.05 * loc[0],
							 i * 0.05 * loc[1],
							 i * 0.05 * loc[2]));
	}
	*/

	// Blend together all the properties from the two different Properties objects.
	return noiseLevel * prop1 + (1.0 - noiseLevel) * prop2;

}
