#include "core/ray.h"

ray::ray(const Vector4d origin, const Vector4d direction):orig(origin),dir(direction)
{ /* do nothing */ }

Vector4d ray::operator()(const double time) const
{
    return dir * time + orig;
}
