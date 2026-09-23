#include "helper.hpp"
#include <memory>
#include <tree.hpp>

bool Tree::intersec(Ray &ray) { return this->intersectPoint(ray).hit; }
