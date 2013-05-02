#ifndef RT_SPHERE_TEXTURE_H
#define RT_SPHERE_TEXTURE_H

#include <string>
#include "../easypng.h"
#include "../material.h"
#include "../properties.h"

/**
 * @brief A material for a file loaded texture. Forms a sphere around the orign
 */
class SphereTexture : public Material {
    public:
    SphereTexture();
    SphereTexture(string const & filename);

    virtual ~SphereTexture();

    virtual Material * create() const;

    virtual Material * clone() const;

    virtual Properties getProperties(point<3> loc) const;

    private:
    PNG texture;
    double reflect;
    double transparency;
    double i_refraction;
    Color specular;
    double spec_power;

};

#endif // RT_SPHERE_TEXTURE_H
