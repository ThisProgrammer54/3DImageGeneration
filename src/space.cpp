#include "helper.hpp"
#include "tree.hpp"
#include <memory>
#include <space.hpp>

Object::Object(std::unique_ptr<Tree> t) : start(std::move(t)){};
IntersectionResult Object::intersectPoint(Ray ray) {
  return start->intersectPoint(ray);
};
LightSource::LightSource(Vector4D pos, RGB intensity)
    : pos(pos), intensity(intensity){};

Space::Space(RGB ambientLightIntensity)
    : ambientLightIntensity(ambientLightIntensity), index(1), recursion(1){};

Space::Space(RGB ambientLightIntensity, int index, int recursion)
    : ambientLightIntensity(ambientLightIntensity), index(index), recursion(recursion){};

void Space::add(std::unique_ptr<Object> obj) {
  this->objects.push_back(std::move(obj));
}
void Space::add(std::unique_ptr<LightSource> ls) {
  this->lightSources.push_back(std::move(ls));
}
