#pragma once

#include "Eigen/Core"

#define Vector4D Eigen ::Vector<double, 4>

struct RGB {
  double red, green, blue;
};

class Ray {
  Vector4D pos;
  Vector4D d;
};

class IntersectPoint {
  Vector4D pos;
  Vector4D t;
  RGB color;
};
