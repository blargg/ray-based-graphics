#ifndef RT_KDTREE_H
#define RT_KDTREE_H

#include <vector>
#include "ray.h"
#include "drawable.h"
using std::vector;

/**
 * A node for the KDTree.
 * This should not be used outside of KDTree, as
 * this stuct will change without notice.
 */
struct KDNode {
public:

    vector<Drawable *> objects;
    KDNode *left;
    KDNode *right;

    int axis;
    double split_pos;
    bool is_leaf;
};

/**
 * KDTree for drawable objects to speed ray intersection tests.
 * This allows you to add a set of drawables and it will handle
 * ray intersection tests for the objects.
 */
class KDTree {
public:
    /**
     * Build a new KDTree that holds all the given objects
     *
     * @param  objList a list of object for the KDTree to manage
     */
    KDTree (vector<Drawable *> objList);

    /**
     * Removes the KDTree.
     */
    virtual ~KDTree ();

    /**
     * Frees all the obj managed by this KDTree.
     */
    void freeAllObj();


    /**
     * Tests for intersection with all objects in the KDTree.
     * @note instead of returning values, you pass in references to change.
     * @pre time is set to the minimum time accepted for intersections (usually 0.0)
     * @param viewRay the ray to test intersections for.
     * @param time will get set to the time of the intersection
     * @param object will point to the object or null if there was no intersection
     */
    void intersection(ray viewRay, double &time, Drawable &*object);

private:
    KDNode *root;

    void buildTree(KDNode *node);
};

#endif // RT_KDTREE_H
