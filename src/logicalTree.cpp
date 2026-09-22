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
    if (this->inside(possible.point)) {
      return possible;
    }
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


IntersectionResult Union::intersectPoint(Ray &ray) {
  vector<IntersectionResult> allResults;

  for (size_t i = 0; i < this->nodes.size(); i++){
    IntersectionResult res = this->nodes[i]->intersectPoint(ray);
    if (res.hit){
      allResults.push_back(res);
    }
  }

  if (allResults.empty()){
    return IntersectionResult();
  }

  //aufsteigend sortiert
  std::sort(allResults.begin(), allResults.end(),[] (const IntersectionResult &a, const IntersectionResult &b) {return a.point.t < b.point.t;});

  for (auto &possible : allResults){
    if (this->inside(possible.point)) {
      return possible;
    }
  }

  return IntersectionResult();
}


bool Union::intersec(Ray &ray) {
  IntersectionResult res = this->intersectPoint(ray);

  if(res.hit){
    return true;
  }
  return false;
}


bool Union::inside(IntersectPoint &a){
  //Punkt gehört zur Union, wenn er in mindestens einem "Kind" liegt
  for (size_t i = 0; i < this->nodes.size(); i++){
    if (this->nodes[i]->inside(a)) {
      return true;
    }
  }
  return false;
}


IntersectionResult Exclusion::intersectPoint(Ray &ray) {
  vector<IntersectionResult> allResults;

  for(size_t i = 0; i < this->nodes.size(); i++) {
    IntersectionResult res = this->nodes[i]->intersectPoint(ray);
    if (res.hit) {
      allResults.push_back(res);
    }
  }

  if (allResults.empty()) {
    return IntersectionResult();
  }

  //nach t aufsteigend sortieren

  std::sort(allResults.begin(), allResults.end(), [] (const IntersectionResult &a, const IntersectionResult &b) {return a.point.t < b.point.t;});

  for (auto &possible : allResults) {
    if (this->inside(possible.point)) {
      return possible;
    }
  }
  return IntersectionResult();
}


bool Exclusion::intersec(Ray &ray) {
  IntersectionResult res = this->intersectPoint(ray);

  if (res.hit) {
    return true;
  }
  return false;
}


bool Exclusion::inside(IntersectPoint &a) {

  // Punkt gehört zu Exclusion wenn er in einer ungraden Anzahl "Kindern" liegt

  int count = 0;
  for (size_t i = 0; i < this->nodes.size(); i++) {
    if(this->nodes[i]->inside(a)) {
      count++;
    }
  }
  return (count % 2) == 1
}