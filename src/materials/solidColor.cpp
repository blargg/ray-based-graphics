#include "solidColor.h"

SolidColor::SolidColor():myProperties(Color(0.0,1.0,0.0))
{ /* Do nothing */ }

SolidColor::SolidColor(Properties p):myProperties(p)
{ /* do nothing */ }

Material * SolidColor::create() const
{
    return new SolidColor();
}

Material * SolidColor::clone() const
{
    return new SolidColor(*this);
}

Properties SolidColor::getProperties(Vector4d loc) const
{
    return myProperties;
}
