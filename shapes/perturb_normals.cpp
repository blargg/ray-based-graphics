#include "perturb_normals.h"

PerturbNormals::PerturbNormals(const Shape& s, double a){
    shape = s.clone();
    angle = a;
}

PerturbNormals::~PerturbNormals() {
    delete shape;
}

Shape* PerturbNormals::create() const {
    return new PerturbNormals(*shape, angle);
}

Shape* PerturbNormals::clone() const {
    return new PerturbNormals(*shape, angle);
}

double PerturbNormals::intersection(const ray& viewRay) const {
    return shape->intersection(viewRay);
}

Vector4d PerturbNormals::normal_vectre(const Vector4d& surface) const {
    return perturb(shape->normal_vectre(surface), angle);
}

double PerturbNormals::getMinBound(int axis) const {
    return shape->getMinBound(axis);
}

double PerturbNormals::getMaxBound(int axis) const {
    return shape->getMaxBound(axis);
}

bool PerturbNormals::intersectsBox(AABB box) const {
    return shape->intersectsBox(box);
}
