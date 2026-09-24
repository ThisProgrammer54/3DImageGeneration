#pragma once

#include "Eigen/Core"
#include <iostream>

using Vector4D = Eigen::Vector<double, 4>;
using Matrix4D = Eigen::Matrix4d;

struct RGB {
  double red, green, blue;
  RGB(double, double, double);
  RGB();

  // Simon: hab mal die Operatoren für die Farben hinzugefügt
  friend RGB operator+(const RGB &, const RGB &);
  friend RGB operator*(const RGB &, const RGB &);
  friend RGB operator*(const RGB &, double);
  friend RGB operator*(double, const RGB &);
  friend bool operator==(const RGB &, const RGB &);
};

class Material {
public:
  RGB ambient;
  RGB diffuse;
  RGB specular;
  RGB reflected;
  RGB refracted;
  double shininess;

  Material(RGB, RGB, RGB, RGB, RGB, double);
  Material();
};

class Ray {
public:
  Vector4D pos;
  Vector4D d;
  Ray(Vector4D, Vector4D);
};

class IntersectPoint {
public:
  Vector4D pos;
  double t;
  RGB color;

  IntersectPoint(Vector4D, double);
  IntersectPoint(Vector4D, double, RGB);
};

class PrimitiveObject;

struct IntersectionResult {
  bool hit;
  IntersectPoint point;
  const PrimitiveObject *object = nullptr;
  Vector4D normal;

  IntersectionResult(bool, class IntersectPoint IntersectPoint,
                     const PrimitiveObject *, Vector4D normal);
  IntersectionResult(bool, class IntersectPoint IntersectPoint,
                     const PrimitiveObject *);

  IntersectionResult();
};

double scalarProduct(Vector4D, Vector4D);
