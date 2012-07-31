#include "checkerBoardTexture.h"

Material * CheckerBoardTexture::create() const
{ return new CheckerBoardTexture(); }

Material * CheckerBoardTexture::clone() const
{ return new CheckerBoardTexture(*this); }

Properties CheckerBoardTexture::getProperties(point<3> loc) const
{
	bool useFirst = true;

	double temp = loc[0] / size;
	if( (int) temp % 2 == 1) // if odd, alternate useFirst
		useFirst = !useFirst;

	temp = loc[1] / size;
	if( (int) temp % 2 == 1) // if odd, alternate useFirst
		useFirst = !useFirst;

	temp = loc[2] / size;
	if( (int) temp % 2 == 1) // if odd, alternate useFirst
		useFirst = !useFirst;

	if(useFirst)
		return prop1;

	return prop2;
}
