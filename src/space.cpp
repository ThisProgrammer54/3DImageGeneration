#include "helper.hpp"
#include <memory>
#include <space.hpp>

Object::Object(std::unique_ptr<Tree> t) : start(std::move(t)){};
std::unique_ptr<IntersectPoint> Object::intersectPoint(Ray ray) {
  return start->intersectPoint(ray);
};

void Space::add(std::unique_ptr<Object> obj) {
  this->objects.push_back(std::move(obj));
}
void Space::add(std::unique_ptr<LightSource> ls) {
  this->lightsource.push_back(std::move(ls));
}
