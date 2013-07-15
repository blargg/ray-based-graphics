#include "kdtree.h"

KDTree::KDTree(vector<Drawable *> objList) {
    root = new KDNode;
    root->objects = objList;
    buildTree(root);
}

KDTree::~KDTree() {
}

void KDTree::freeAllObj() {
}

void KDTree::intersection(ray viewRay, double &time, Drawable &*object) {
}

void KDTree::buildTree(KDNode *node) {
    if(node->objects.size() < 3)
        return;

}
