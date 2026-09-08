#pragma once

#include "Eigen/Core"
#include "tree.hpp"
#include "helper.hpp"

#define Vector4D Eigen ::Vector<double, 4>

class Space {
    Object primObject;
    LightSource lightsource;
};

class LightSource {
    Vector4D pos;
    RGB intensity;
};

class Object {
    Tree* start;
};