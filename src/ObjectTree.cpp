#include "Eigen/Core"
#include "helper.hpp"
#include <memory>
#include <tree.hpp>

Halfspace::Halfspace(Vector4D position, Vector4D normal) {
  Vector4D normedNormal = normal / normal.norm();
  this->normal = normedNormal;

  double r = this->Polynom(position);
  this->position = r * normedNormal;
  this->position(3) = 1;
}

std::unique_ptr<IntersectPoint> Halfspace::intersectPoint(Ray &ray) {
  double r = this->normal(3);
  double dScalar = scalarProduct(this->normal, ray.d);

  // parallel
  if (dScalar == 0) {
    if (Polynom(ray.pos) == r) {
      return std::make_unique<IntersectPoint>(ray.pos, 0);
    }
    return nullptr;
  }
  double t =
      (scalarProduct(this->position - ray.pos, this->normal)) / (dScalar);
  if (t < 0) {
    return nullptr;
  }
  return std::make_unique<IntersectPoint>(ray.pos + t * ray.d, t);
}
bool Halfspace::intersec(Ray &ray) {
  std::unique_ptr<IntersectPoint> iP = this->intersectPoint(ray);

  if (iP == nullptr)
    return false;

  return true;
}
double Halfspace::Polynom(Vector4D in) {
  return scalarProduct(this->normal, in);
}
