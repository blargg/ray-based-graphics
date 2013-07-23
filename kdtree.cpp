#include "kdtree.h"
#include <assert.h>
#include <algorithm>
#include <stdio.h>
#include <limits>

const double KDTree::cost_traversal = 0.3;
const double KDTree::cost_intersection = 1.0;

KDTree::KDTree() {
    root = new KDNode;
    bounds.maxCorner = Vector3d(1,1,1);
    bounds.minCorner = Vector3d(0,0,0);
}

KDTree::KDTree(vector<Drawable *> objList) {
    assert(objList.size() > 0);

    root = new KDNode;
    root->objects = objList;
    double max[3], min[3];
    for(int i = 0; i < 3; i++) {
        max[i] = objList[0]->getMaxBound(i);
        min[i] = objList[0]->getMinBound(i);
    }

    for(unsigned int i = 1; i < objList.size(); i++) {
        for(int j = 0; j < 3; j++) {
            max[j] = std::max(max[j], objList[i]->getMaxBound(j));
            min[j] = std::min(min[j], objList[i]->getMinBound(j));
        }
    }
    bounds.maxCorner = Vector3d(max[0], max[1], max[2]) + Vector3d::Constant(EPSILON);
    bounds.minCorner = Vector3d(min[0], min[1], min[2]) - Vector3d::Constant(EPSILON);
    buildTree(root, bounds, 0);
}

KDTree::~KDTree() {
}

void KDTree::freeAllObj() {
}

void KDTree::intersection(ray viewRay, double &time, Drawable **obj) {
    IntersectionData searchResult;
    double tmin, tmax;
    if(bounds.intersection(viewRay, tmin, tmax)) {
        searchResult = searchNode(root, viewRay, tmin, tmax, 0);
    } else {
        IntersectionData intersection;
        intersection.obj = NULL;
        intersection.time = -1 * std::numeric_limits<double>::max();
        searchResult = intersection;
    }
    time = searchResult.time;
    *obj = searchResult.obj;
}

IntersectionData KDTree::searchNode(KDNode *node, const ray &viewRay, double tmin, double tmax, int curAxis) {
    assert(tmin <= tmax);
    assert(curAxis >= 0 && curAxis < 3);
    if(node->is_leaf) {
        return closestIntersection(node->objects, viewRay);
    }

    int nextAxis = (curAxis + 1) % 3;
    double tSplit = (node->split_pos - viewRay.orig(curAxis)) / viewRay.dir(curAxis);
    KDNode *nearNode, *farNode;
    if(viewRay.orig(curAxis) < node->split_pos) {
        nearNode = node->left;
        farNode = node->right;
    } else {
        nearNode = node->right;
        farNode = node->left;
    }


    if(tSplit > tmax) {
        return searchNode(nearNode, viewRay, tmin, tmax, nextAxis);
    } else if (tSplit < tmin) {
        if(tSplit > 0)
            return searchNode(farNode, viewRay, tmin, tmax, nextAxis);
        else if(tSplit < 0)
            return searchNode(nearNode, viewRay, tmin, tmax, nextAxis);
        else {
            if(viewRay.dir(curAxis) < 0)
                return searchNode(node->left, viewRay, tmin, tmax, nextAxis);
            else
                return searchNode(node->right, viewRay, tmin, tmax, nextAxis);
        }
    } else {
        if(tSplit > 0) {
            IntersectionData test = searchNode(nearNode, viewRay, tmin, tSplit, nextAxis);
            if(test.obj != NULL && test.time < tSplit + EPSILON)
                return test;
            else
                return searchNode(farNode, viewRay, tSplit, tmax, nextAxis);
        } else {
            return searchNode(nearNode, viewRay, tSplit, tmax, nextAxis);
        }
    }
}

IntersectionData KDTree::closestIntersection(const vector<Drawable *> &objList, const ray &viewRay) {
    IntersectionData intersection;
    intersection.obj = NULL;
    intersection.time = std::numeric_limits<double>::max();

    double tmp_time;
    for(unsigned int i = 0; i < objList.size(); i++) {
        Drawable *object = objList[i];
        tmp_time = object->intersection(viewRay);
        if(tmp_time > EPSILON && tmp_time < intersection.time) {
            intersection.time = tmp_time;
            intersection.obj = object;
        }
    }

    return intersection;
}

void KDTree::buildTree(KDNode *node, AABB curBounds, int curAxis) {
    if(node->objects.size() <= 5) {
        node->is_leaf = true;
        node->left = NULL;
        node->right = NULL;
        return;
    }

    node->is_leaf = false;

    double bestCost;
    double potentialSplit = bestSplitPos(node, curBounds, curAxis, bestCost);

    if(bestCost > cost_intersection * node->objects.size()){
        node->is_leaf = true;
        node->left = NULL;
        node->right = NULL;
        return;
    }

    node->split_pos =  potentialSplit;
    assert(node->split_pos < curBounds.maxCorner(curAxis));
    assert(node->split_pos > curBounds.minCorner(curAxis));
    node->left = new KDNode;
    node->right = new KDNode;

    AABB leftBounds = curBounds;
    leftBounds.maxCorner(curAxis) = node->split_pos;
    AABB rightBounds = curBounds;
    rightBounds.minCorner(curAxis) = node->split_pos;

    for(unsigned int i = 0; i < node->objects.size(); i++) {
        if(node->objects[i]->intersectsBox(leftBounds))
            node->left->objects.push_back(node->objects[i]);
        if(node->objects[i]->intersectsBox(rightBounds))
            node->right->objects.push_back(node->objects[i]);
    }

    int nextAxis = (curAxis + 1) % 3;
    buildTree(node->left, leftBounds, nextAxis);
    buildTree(node->right, rightBounds, nextAxis);
}

double KDTree::bestSplitPos(KDNode *node, AABB bounds, int axis, double &finalCost) {
    vector<double> splitPos;
    splitPos.reserve(node->objects.size() * 2);
    for(unsigned int i = 0; i < node->objects.size(); i++) {
        splitPos.push_back(node->objects[i]->getMinBound(axis));
        splitPos.push_back(node->objects[i]->getMaxBound(axis));
    }
    std::sort(splitPos.begin(), splitPos.end());

    double bestSplit = splitPos[0];
    double bestCost = costSplit(node->objects, bounds, splitPos[0], axis);
    for(unsigned int i = 1; i < splitPos.size(); i++) {
        if(splitPos[i] < bounds.minCorner(axis) || splitPos[i] > bounds.maxCorner(axis))
            continue;
        double currentCost = costSplit(node->objects, bounds, splitPos[i], axis);
        if(currentCost < bestCost) {
            bestSplit = splitPos[i];
            bestCost = currentCost;
        }
    }
    finalCost = bestCost;
    return bestSplit;
}

int KDTree::numIntersections(const vector<Drawable *> &objList, AABB bounds) {
    int ret = 0;
    for(unsigned int i = 0; i < objList.size(); i++) {
        if(objList[i]->intersectsBox(bounds))
            ret++;
    }
    return ret;
}

double KDTree::costSplit(const vector<Drawable *> &objList, AABB bounds, double split, int axis) {
    double SAV = 1.0 / bounds.surfaceArea();
    AABB left = bounds;
    left.maxCorner(axis) = split;
    double leftArea = left.surfaceArea();
    int leftCount = numIntersections(objList, left);

    AABB right = bounds;
    right.minCorner(axis) = split;
    double rightArea = right.surfaceArea();
    int rightCount = numIntersections(objList, right);

    return cost_traversal +
        cost_intersection * (leftArea * leftCount * SAV + rightArea * rightCount * SAV);
}
