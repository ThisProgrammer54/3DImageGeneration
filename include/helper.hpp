#pragma once

#include "Eigen/Core"
#include <iostream>

#define Vector4D Eigen ::Vector<double, 4>

struct RGB {
  double red, green, blue;
  RGB(double, double, double);
  RGB();

  //Simon: hab mal die Operatoren für die Farben hinzugefügt
  friend RGB operator+(const RGB, const RGB);
  friend RGB operator*(const RGB, const RGB);
  friend RGB operator*(const RGB, double);
  friend RGB operator*(double, const RGB);
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
  std::unique_ptr<IntersectPoint> point;
  const PrimitiveObject *object = nullptr;
};

double scalarProduct(Vector4D, Vector4D);
