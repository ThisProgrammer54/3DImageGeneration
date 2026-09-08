#pragma once

#include "Eigen/Core"
#include "helper.hpp"
#include "tree.hpp"
#include <memory>

#define Vector4D Eigen ::Vector<double, 4>

class LightSource {
  Vector4D pos;
  RGB intensity;
};

class Object {
  std::unique_ptr<Tree> start;
};

class Space {
  std::vector<std::unique_ptr<Object>> Objects;
  std::vector<std::unique_ptr<LightSource>> lightsource;
};

