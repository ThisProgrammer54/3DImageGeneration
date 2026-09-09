#pragma once

#include "helper.hpp"
#include "space.hpp"
#include <vector>

using std::vector;

class Pixel {
public:
  int x;
  int y;
  RGB value;
  Vector4D visPoint;

  void shootRay(const Space &, const Ray &);

private:

  Pixel::Pixel(int x, int y, RGB value, Vector4D visPoint);


  IntersectPoint getIntersectionPoint(const Space &) const;
  RGB getColor(const Space &, const IntersectPoint &) const;
  void calcNewRay(const Space &, const Ray &, const IntersectPoint &) const;
};

class Screen {
private:
  int heigth;
  int width;
  int dpi;
  Vector4D observer;
  Vector4D position;
  vector<vector<Pixel>> values;

public:

  Screen::Screen(int heigth, int width, int dpi, Vector4D observer, Vector4D position);

  void setDimensions(int height, int width);
  void calculate();
  void generateImage();
};
