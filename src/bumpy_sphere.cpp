#include "bumpy_sphere.h"

double BumpySphere::intersection(const ray<3> viewRay) const {
    return sphere.intersection(viewRay);
}

vectre<3> BumpySphere::normal_vector(const point<3> surface) const {
    vectre<3> up(0,1,0);
    vectre<3> normal = sphere.normal_vector(surface).unit_vectre();
    vectre<3> right = up.cross_prod(normal);
    up = normal.cross_prod(up);

    double r = sqrt( surface[0] * surface[0] + surface[1] * surface[1] + surface[2] * surface[2] );
    double s = acos(surface[2] / r);
    double t = atan(surface[1] / surface[0]);

    s = (s * bumpMap.height() / M_PI);
    t = (t + M_PI / 2) * bumpMap.width() / M_PI ;

    int tex_x = std::min((int)t, bumpMap.width() - 1);
    int tex_y = std::min((int)s, bumpMap.height() - 1);
    const RGBAPixel *pix = bumpMap(tex_x, tex_y);
    double red = pix->red / 256.0;
    double green = pix->green / 256.0;
    double blue = pix->blue / 256.0;

    return right * red + up * green + normal * blue;
}

Properties BumpySphere::getProperties(const point<3> location) const {
    return mat.getProperties(location);
}
