#include "Eigen/Core"
#include "helper.hpp"
#include <strings.h>
#include <tree.hpp>

Halfspace::Halfspace(Vector4D position, Vector4D normal, RGB ambRC,
                     RGB diffuseReflectionComponent,
                     RGB specularReflectionComponent, RGB reflec, RGB refrac,
                     double shininess) {
  Vector4D normedNormal = normal / normal.norm();
  this->normal = normedNormal;

  double r = scalarProduct(this->normal, position);
  this->position = r * normedNormal;
  this->position(3) = 1;

  this->ambientReflectionComponent = ambRC;
  this->diffuseReflectionComponent = diffuseReflectionComponent;
  this->specularReflectionComponent = specularReflectionComponent;
  this->reflectionComponent = reflec;
  this->refractedComponent = refrac;

  this->shininess = shininess;
}
Halfspace::Halfspace(Vector4D position, Vector4D normal, RGB color)
    : Halfspace(position, normal, color, color, color, color, color,
                DEFSHINY){};

Halfspace::Halfspace(Vector4D position, Vector4D normal)
    : Halfspace(position, normal, DEFCOLOR){};

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

  if (!iP.hit)
    return false;

  return true;
}
double Halfspace::Polynom(Vector4D in) const {
  return scalarProduct(this->normal, this->position - in);
}
bool Halfspace::inside(IntersectPoint &a) {
  return this->Polynom(a.pos) >= -1e-5;
}

Vector4D Halfspace::getNormal(Vector4D point) const {
  point * 1; // damit die Warnung weg ist
  return this->normal;
}

Sphere::Sphere(Vector4D position, double radius, RGB ambRC, RGB specRC,
               RGB diffuseRC, RGB reflec, RGB refrac, double shininess) {
  this->position = position;
  this->radius = radius;

  this->ambientReflectionComponent = ambRC;
  this->specularReflectionComponent = specRC;
  this->diffuseReflectionComponent = diffuseRC;
  this->reflectionComponent = reflec;
  this->refractedComponent = refrac;
  this->shininess = shininess;
};

Sphere::Sphere(Vector4D position, double radius, RGB color)
    : Sphere(position, radius, color, color, color, color, color, DEFSHINY){};
Sphere ::Sphere(Vector4D position, double radius)
    : Sphere(position, radius, DEFCOLOR){};

IntersectionResult Sphere::intersectPoint(Ray &ray) {
  return IntersectionResult();
}

bool Sphere::intersec(Ray &ray) { return true; }

bool Sphere::inside(IntersectPoint &a) { return true; }

double Sphere::Polynom(Vector4D in) const {
  double x = in(0) - this->position(0);
  double y = in(1) - this->position(1);
  double z = in(2) - this->position(2);

  return x * x + y * y + z * z - this->radius * this->radius;
}

Vector4D Sphere::getNormal(Vector4D point) const {
  return (this->position * this->Polynom(point)) / this->radius;
}
