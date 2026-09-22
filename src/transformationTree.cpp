#include "helper.hpp"
#include "json_io.hpp"
#include <memory>
#include <tree.hpp>

IntersectionResult TransformationTree::intersectPoint(Ray &ray) {
  Ray transRay(applyInverse(ray.pos), applyInverse(ray.d));
  transRay.d = transRay.d / transRay.d.norm();

  IntersectionResult res = this->node->intersectPoint(transRay);

  if (!res.hit) {
    return IntersectionResult();
  }

  return IntersectionResult(
      true, IntersectPoint(applyTransform(res.point.pos), res.point.t),
      res.object);
}

bool TransformationTree::intersec(Ray &ray) {
  return this->intersectPoint(ray).hit;
}

bool TransformationTree::inside(IntersectPoint &a) {
  IntersectPoint transPoint = IntersectPoint(applyInverse(a.pos), -1);
  return node->inside(transPoint);
}

Translation::Translation(std::unique_ptr<Tree> node, Vector4D tranformation) {
  this->node = std::move(node);

  this->buildMatrices(tranformation);
  this->buildInverses();
};

void Translation::buildMatrices(Vector4D in) {
  this->mat << 1.0, 0.0, 0.0, in(0), 0.0, 1.0, 0.0, in(1), 0.0, 0.0, 1.0, in(2),
      0.0, 0.0, 0.0, 1.0;
}
void Translation::buildInverses() {
  this->inv << 1.0, 0.0, 0.0, -this->mat(0, 3), 0.0, 1.0, 0.0, -this->mat(1, 3),
      0.0, 0.0, 1.0, -this->mat(2, 3), 0.0, 0.0, 0.0, 1.0;
}
Vector4D Translation::applyInverse(const Vector4D in) const {
  return this->inv * in;
}
Vector4D Translation::applyTransform(const Vector4D in) const {
  return this->mat * in;
};

Scaling::Scaling(std::unique_ptr<Tree> node, Vector4D tranformation) {
  this->node = std::move(node);

  this->buildMatrices(tranformation);
  this->buildInverses();
};

void Scaling::buildMatrices(Vector4D in) {
  this->mat << in(0), 0.0, 0.0, 0.0, 0.0, in(1), 0.0, 0.0, 0.0, 0.0, in(2), 0.0,
      0.0, 0.0, 0.0, 1.0;
}
void Scaling::buildInverses() {
  this->inv << 1 / this->mat(0, 0), 0.0, 0.0, 0.0, 0.0, 1 / this->mat(1, 1),
      0.0, 0.0, 0.0, 0.0, 1 / this->mat(2, 2), 0.0, 0.0, 0.0, 0.0, 1.0;
}
Vector4D Scaling::applyInverse(const Vector4D in) const {
  return this->inv * in;
}
Vector4D Scaling::applyTransform(const Vector4D in) const {
  return this->mat * in;
};
