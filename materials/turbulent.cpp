#include "turbulent.h"
#include <math.h>

Material* Turbulent::create() const
{ return new Turbulent(); }

Material* Turbulent::clone() const
{ return new Turbulent(*this); }

Properties Turbulent::getProperties(point<3> loc) const
{
	double noiseLevel = 0.0;
	for(int i = 1; i < 10; ++i)
	{
		noiseLevel += (1.0 / i) * 
				 fabs(noise( i * 0.01 * loc[0],
							 i * 0.01 * loc[1],
							 i * 0.01 * loc[2]));
	}

	// Keep noiseLevel between 0.0 and 1.0
	noiseLevel = std::max( std::min(noiseLevel, 1.0), 0.0);

	// Blend together all the properties from the two different Properties objects.
	return noiseLevel * prop1 + (1.0 - noiseLevel) * prop2;

}
