#include "helper.hpp"
#include <iostream>
#include <tree.hpp>

Halfspace::Halfspace(Vector4D position, Vector4D normal)
    : position(position), normal(normal){};

IntersectPoint Halfspace::intersectPoint(Ray &ray) {
  std::cout << "SHOULD HAPPEN";
}
bool Halfspace::intersec(Ray &ray) { std::cout << "AHHH"; }
double Halfspace::Polynom(Vector4D in) { std::cout << "GOD NO"; }
