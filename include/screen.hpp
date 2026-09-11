#pragma once

#include "helper.hpp"
#include "space.hpp"
#include <ostream>
#include <vector>

using std::vector;

class Pixel {
public:
  int x;
  int y;
  RGB value;
  Vector4D visPoint;

  void shootRay(const Space &, const Ray &);

  Pixel();
  Pixel(int x, int y, RGB value, Vector4D visPoint);

  // Testing TODO REMOVE : private:
  IntersectionResult getIntersectionPoint(const Space &, const Ray &) const;
  RGB getColor(const Space &, const IntersectionResult &) const;
  void calcNewRay(const Space &, const Ray &, const IntersectPoint &) const;
};

class Screen {
private:
  int height;
  int width;
  int dpi;
  Vector4D observer;
  Vector4D position;
  vector<vector<Pixel>> values;

public:
  Screen(int heigth, int width, int dpi, Vector4D observer, Vector4D position);

  void setDimensions(int height, int width);
  void calculate(const Space &);
  void generateImage();
  friend std::ostream &operator<<(std::ostream &out, const Screen &in);
};
