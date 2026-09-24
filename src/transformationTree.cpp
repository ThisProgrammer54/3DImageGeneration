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

  Vector4D pos = applyTransform(res.point.pos);
  double newT =
      (pos - ray.pos)
          .norm(); // muss angepasst werden weil wir d vorher normieren
  return IntersectionResult(true, IntersectPoint(pos, newT), res.object);
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

Rotation::Rotation(std::unique_ptr<Tree> node, Vector4D transformation) {
  this->node = std::move(node);
  this->rotation = transformation;

  this->buildMatrices(transformation);
  this->buildInverses();
}
void Rotation::buildMatrices(Vector4D in) {
  double x = in(0);
  double y = in(1);
  double z = in(2);

  this->matX << 1.0, 0.0, 0.0, 0.0, 0.0, cos(x), -sin(x), 0.0, 0.0, sin(x),
      cos(x), 0.0, 0.0, 0.0, 0.0, 1.0;
  this->matY << cos(y), 0.0, sin(y), 0.0, 0.0, 1.0, 0.0, 0.0, -sin(y), 0.0,
      cos(y), 0.0, 0.0, 0.0, 0.0, 1.0;
  this->matZ << cos(z), -sin(z), 0.0, 0.0, sin(z), cos(z), 0.0, 0.0, 0.0, 0.0,
      1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
}
void Rotation::buildInverses() {
  double x = -this->rotation(0);
  double y = -this->rotation(1);
  double z = -this->rotation(2);

  this->invX << 1.0, 0.0, 0.0, 0.0, 0.0, cos(x), -sin(x), 0.0, 0.0, sin(x),
      cos(x), 0.0, 0.0, 0.0, 0.0, 1.0;
  this->invY << cos(y), 0.0, sin(y), 0.0, 0.0, 1.0, 0.0, 0.0, -sin(y), 0.0,
      cos(y), 0.0, 0.0, 0.0, 0.0, 1.0;
  this->invZ << cos(z), -sin(z), 0.0, 0.0, sin(z), cos(z), 0.0, 0.0, 0.0, 0.0,
      1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
}
Vector4D Rotation::applyTransform(const Vector4D in) const {
  return this->matX * this->matY * this->matZ * in;
}
Vector4D Rotation::applyInverse(const Vector4D in) const {
  return this->invX * this->invY * this->invZ * in;
}
