#pragma once

#include "Eigen/Core"
#include "helper.hpp"
#include "tree.hpp"

#define Vector4D Eigen ::Vector<double, 4>

class LightSource {
  Vector4D pos;
  RGB intensity;
};

class Object {
  Tree *start;
};

class Space {
  Object primObject;
  LightSource lightsource;
};

