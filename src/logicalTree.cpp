#include "helper.hpp"
#include <cstddef>
#include <tree.hpp>

using std::vector;

IntersectionResult LogicalTree::intersectPoint(Ray &ray) {
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
    if (this->inside(possible.point)) {
      return possible;
    }
  }
  return IntersectionResult();
}

bool LogicalTree::intersec(Ray &ray) { return this->intersectPoint(ray).hit; }

bool Intersection::inside(IntersectPoint &a) {
  for (size_t i = 0; i < this->nodes.size(); i++) {
    if (!this->nodes[i]->inside(a))
      return false;
  }
  return true;
}

bool Union::inside(IntersectPoint &a) {
  for (size_t i = 0; i < this->nodes.size(); i++) {
    if (this->nodes[i]->inside(a))
      return true;
  }
  return false;
}

bool Exclusion::inside(IntersectPoint &a) {
  int count = 0;

  // philip: habe es mal zu nur in einem node geaendert
  for (size_t i = 0; i < this->nodes.size(); i++) {
    if (this->nodes[i]->inside(a)) {
      count++;
    }
    if (count > 1)
      return false;
  }
  return count == 1;
}
