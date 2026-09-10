#include <cstddef>
#include <screen.hpp>

Pixel::Pixel(int x, int y, RGB value, Vector4D visPoint)
    : x(x), y(y), value(value), visPoint(visPoint){};
Pixel::Pixel() : x(0), y(0), value(), visPoint() {}



IntersectPoint Pixel::getIntersectionPoint(const Space &space,
                                           const Ray &ray) const {
  // iterate over every object in the space
  std::vector<double> tCandidates;
  double lowestT = INFINITY;
  int tIndex = -1;
  for (size_t i = 0; i < space.objects.size(); ++i) {
    tCandidates.push_back(space.objects[i].get()->intersectPoint(ray).t);
    if (*tCandidates.end() < lowestT) {
      lowestT = *tCandidates.end();
      tIndex = i;
    }
  }
  if (tIndex != -1) {
    return space.objects[tIndex].get()->intersectPoint(ray);
  }
  // else { return NULL; }  // cannot convert intersectPoint to NULL, special
  // method/constructor needed? what to return when there's no intersection
  // point? does Ray ray assume it has hit something?
  // --
  // we could return a uniq pointer and a nullptr like object/Halfspace

}

Screen::Screen(int heigth, int width, int dpi, Vector4D observer,
               Vector4D position)
    : heigth(heigth), width(width), dpi(dpi), observer(observer),
      position(position), values(heigth, vector<Pixel>(width)){};
