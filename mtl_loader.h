#ifndef RT_MTL_LOADER_H
#define RT_MTL_LOADER_H

#include <string>
#include <map>
#include "color.h"
#include "materials/solidColor.h"

using std::map;
using std::string;

class MtlLoader
{
public:
    MtlLoader ();
    virtual ~MtlLoader ();

    /**
     * adds all materials from filename to the given map
     */
    void add_to_map(map<string, SolidColor> &map, const char * filename);

private:
    bool unsetData;
    string name;
    Color ambient;
    Color diffuse;
    Color specular;
    double spec_hardness;
    double dissolve;
    double index_refraction;

    /**
     * adds the current discription of the material to the map
     */
    void pushMat(map<string, SolidColor> &map);
};

#endif // RT_MTL_LOADER_H
