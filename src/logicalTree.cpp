#include "helper.hpp"
#include <cstddef>
#include <tree.hpp>

using std::vector;

IntersectionResult Intersection::intersectPoint(Ray &ray) {
  vector<IntersectionResult> allResults;

  for (size_t i = 0; i < this->nodes.size(); i++) {
    IntersectionResult res = this->nodes[i]->intersectPoint(ray);
    if (res.hit)
      allResults.push_back(res);
  }

  if (allResults.empty())
    return IntersectionResult();

  // nach t aufsteigend sortieren
  std::sort(allResults.begin(), allResults.end(),
            [](const IntersectionResult &a, const IntersectionResult &b) {
              return a.point.t < b.point.t;
            });
  for (auto &possible : allResults) {
    if (this->inside(possible.point))
      return possible;
  }
  return IntersectionResult();
}
bool Intersection::intersec(Ray &ray) {
  IntersectionResult res = this->intersectPoint(ray);

  if (res.hit)
    return true;
  return false;
}
bool Intersection::inside(IntersectPoint &a) {
  for (size_t i = 0; i < this->nodes.size(); i++) {
    if (!this->nodes[i]->inside(a))
      return false;
  }
  return true;
}
