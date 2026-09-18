#include "Eigen/Core"
#include "helper.hpp"
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
    if (this->inside(ray.pos)) {
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

  if (iP.hit)
    return true;
  return false;
}
double Halfspace::Polynom(Vector4D in) {
  return scalarProduct(this->normal, in - this->position);
}
bool Halfspace::inside(IntersectPoint &a) {
  return this->Polynom(a.pos) >= -1e-5;
}
bool Halfspace::inside(Vector4D &pos) { return this->Polynom(pos) >= -1e-5; }

// simon:
Vector4D Halfspace::getNormal() const { return this->normal; }
