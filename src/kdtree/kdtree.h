#ifndef RT_KDTREE_H
#define RT_KDTREE_H

#include <vector>
#include <unordered_set>
#include "core/ray.h"
#include "drawable.h"
#include "kdtree/aabb.h"
using std::vector;

/**
 * A node for the KDTree.
 * This should not be used outside of KDTree, as
 * this stuct will change without notice.
 */
struct KDNode {
public:
    KDNode():left(NULL),right(NULL),is_leaf(false)
    { /* do nothing */};

    vector<Drawable *> objects;
    KDNode *left;
    KDNode *right;

    double split_pos;
    bool is_leaf;
};


/**
 * Describes the intersection with a ray.
 */
struct IntersectionData {
    /// time to the intersection
    double time;
    /// the object that was intersected
    Drawable *obj;
};

/**
 * KDTree for drawable objects to speed ray intersection tests.
 * This allows you to add a set of drawables and it will handle
 * ray intersection tests for the objects.
 */
class KDTree {
public:

    /**
     * Makes a default tree with no objects.
     */
    KDTree();

    /**
     * Copyies another KDTree into this one.
     */
    KDTree(const KDTree &other);

    /**
     * Build a new KDTree that holds all the given objects
     *
     * @param  objList a list of object for the KDTree to manage
     */
    KDTree (vector<Drawable *> objList);

    /**
     * Assignment operator.
     */
    KDTree & operator=(const KDTree &other);

    /**
     * Cleans up tree, then rebuilds it with a new object list.
     * Does not free the Drawables given to it.
     */
    void rebuildTree(vector<Drawable *> objList);

    /**
     * Removes the KDTree.
     */
    virtual ~KDTree ();

    /**
     * Frees all the obj managed by this KDTree.
     * This is not a very efficient operation, it would
     * be better to hold onto the vector used to build
     * the tree to free the objects.
     */
    void freeAllObj();


    /**
     * Tests for intersection with all objects in the KDTree.
     * @note instead of returning values, you pass in references to change.
     * @pre time is set to the minimum time accepted for intersections (usually 0.0)
     * @param viewRay the ray to test intersections for.
     * @returns IntersectionData for the found intersection
     */
    void intersection(ray viewRay, double &time, Drawable **obj);

private:
    KDNode *root;

    /// An AABB that can surround all objects in the scene.
    AABB bounds;

    static const double cost_traversal;
    static const double cost_intersection;

    /**
     * Recursively copies the node and returns the pointer.
     * Allocates the node structure.
     */
    KDNode* copyTree(KDNode *node);

    /**
     * Recursively deletes the tree nodes, but not the object held inside.
     */
    void deleteTree(KDNode *node);

    /**
     * Recursively searches the tree for Drawable*, frees them, then
     * adds them to the freeList
     */
    void freeObjects(KDNode *node, std::unordered_set<Drawable *> &freeList);

    /**
     * recusively assembles the tree
     */
    void buildTree(KDNode *node, AABB curBounds, int curAxis);

    /**
     * Takes a list of objects and returns the smallest
     * AABB that contians them all
     */
    AABB findBounds(vector<Drawable *> objList);

    /**
     * Recursively search for the closest intersection in the tree.
     * @param node the node to search from
     * @param bounds the bounds for the node
     * @param curAxis the axis that this node operates on
     */
    IntersectionData searchNode(KDNode *node, const ray &viewRay, double tmin, double tmax, int curAxis);

    /**
     * Finds the closest intersection with the ray if one exists.
     */
    IntersectionData closestIntersection(const vector<Drawable *> &objList, const ray &viewRay);

    /**
     * determines the best split position for a node
     */
    double bestSplitPos(KDNode *node, AABB bounds, int axis, double &finalCost);

    /**
     * counts the number of primatives in the list that intersect
     * the AABB
     */
    int numIntersections(const vector<Drawable *> &objList, AABB bounds);

    double costSplit(const vector<Drawable *> &objList, AABB bounds, double split, int axis);
};

#endif // RT_KDTREE_H
