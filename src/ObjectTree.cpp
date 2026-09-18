#include "Eigen/Core"
#include "helper.hpp"
#include <cstddef>
#include <memory>
#include <tree.hpp>

Halfspace::Halfspace(Vector4D position, Vector4D normal,
                     RGB diffuseReflectionComponent,
                     RGB specularReflectionComponent, double shininess) {
  Vector4D normedNormal = normal / normal.norm();
  this->normal = normedNormal;

  double r = scalarProduct(this->normal, position);
  this->position = r * normedNormal;
  this->position(3) = 1;

  this->diffuseReflectionComponent = diffuseReflectionComponent;
  this->specularReflectionComponent = specularReflectionComponent;

  this->shininess = shininess;
}
Halfspace::Halfspace(Vector4D position, Vector4D normal)
    : Halfspace(position, normal, {1, 0, 0}, {1, 0, 0}, 0){};

IntersectionResult Halfspace::intersectPoint(Ray &ray) {
  double dScalar = scalarProduct(this->normal, ray.d);

  // parallel
  if (dScalar == 0) {
    if (Polynom(ray.pos) >= -1e-5) {
      return IntersectionResult{true, IntersectPoint(ray.pos, 0), this};
    }
    return IntersectionResult();
  }
  double t =
      (scalarProduct(this->position - ray.pos, this->normal)) / (dScalar);
  if (t < 0) {
    return IntersectionResult();
  }

  return IntersectionResult(true, IntersectPoint(ray.pos + t * ray.d, t), this);
}
bool Halfspace::intersec(Ray &ray) {
  IntersectionResult iP = this->intersectPoint(ray);

  if (iP.object == nullptr)
    return false;

  return true;
}
double Halfspace::Polynom(Vector4D in) {
  return scalarProduct(this->normal, this->position - in);
}

// simon:
Vector4D Halfspace::getNormal() const { return this->normal; }
