#include <cmath>
#include <algorithm>
#include "sphere_texture.h"

#define _USE_MATH_DEFINES

SphereTexture::SphereTexture() {
    reflect = 0.0;
    transparency = 0.0;
    i_refraction = 1.0;
    specular = Color(1, 1, 1);
    spec_power = 25.0;
}

SphereTexture::SphereTexture(string const & filename):texture(filename) {
    reflect = 0.0;
    transparency = 0.0;
    i_refraction = 1.0;
    specular = Color(1, 1, 1);
    spec_power = 25.0;
}

SphereTexture::~SphereTexture() {
}

Material * SphereTexture::create() const {
    return new SphereTexture();
}

Material * SphereTexture::clone() const {
    return new SphereTexture(*this);
}

Properties SphereTexture::getProperties(Vector4d loc) const {
    double r = sqrt( loc[0] * loc[0] + loc[1] * loc[1] + loc[2] * loc[2] );
    double s = acos(loc[2] / r);
    double t = atan(loc[1] / loc[0]);

    s = (s * texture.height() / M_PI);
    t = (t + M_PI / 2) * texture.width() / M_PI ;

    int tex_x = std::min((int)t, texture.width() - 1);
    int tex_y = std::min((int)s, texture.height() - 1);
    const RGBAPixel *pix = texture(tex_x, tex_y);
    Color texColor(pix->red / 256.0, pix->green / 256.0, pix->blue / 256.0);

    Properties p;
    p.color = texColor;
    p.i_refraction = i_refraction;
    p.reflect = reflect;
    p.spec_power = spec_power;
    p.specular = specular;
    p.tranparency = transparency;
    return p;
}
