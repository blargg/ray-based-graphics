#ifndef RT_MTL_LOADER_H
#define RT_MTL_LOADER_H

#include <string>
#include <map>
#include "core/color.h"
#include "materials/solidColor.h"
#include "properties.h"

using std::map;
using std::string;

/**
 * A class for loading MTL files.
 * Loads them into a map for later reference.
 */
class MtlLoader
{
public:
    /**
     * Instantiates the loader
     */
    MtlLoader ();
    virtual ~MtlLoader ();

    /**
     * adds all materials from filename to the given map.
     * The key is the name of the material, the value is a SolidColor with
     * properties parsed from the given file.
     *
     * @param map the map to load the materials into
     * @filename the name of the file to load (assumes Unix)
     */
    void add_to_map(map<string, SolidColor> &map, string filename);

private:
    bool unsetData;
    string name;
    Properties curProp;

    /**
     * adds the current discription of the material to the map
     */
    void pushMat(map<string, SolidColor> &map);
};

#endif // RT_MTL_LOADER_H
