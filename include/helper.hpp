#pragma once

#include "Eigen/Core"

#define Vector4D Eigen ::Vector<double, 4>

struct RGB {
  double red, green, blue;
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
  Vector4D t;
  RGB color;

  IntersectPoint(Vector4D, Vector4D, RGB);
};
