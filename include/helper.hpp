#pragma once

#include "Eigen/Core"
#include <iostream>

using Vector4D = Eigen::Vector<double, 4>;
using Matrix4D = Eigen::Matrix4d;

#define DEFSHINY 1

struct RGB {
  double red, green, blue;
  RGB(double, double, double);
  RGB();

  friend RGB operator+(const RGB &, const RGB &);
  friend RGB operator*(const RGB &, const RGB &);
  friend RGB operator*(const RGB &, double);
  friend RGB operator*(double, const RGB &);
  friend bool operator==(const RGB &, const RGB &);
};

class Material {
public:
  RGB ambient;
  RGB diffuse;
  RGB specular;
  RGB reflected;
  RGB refracted;
  double shininess;

  Material(RGB, RGB, RGB, RGB, RGB, double);

  /**
   * @brief constructs an material with default values for RGB and default for
   * shininess
   */
  Material();
};

class Ray {
public:
  Vector4D pos;
  Vector4D d;
  Ray(Vector4D, Vector4D);
};

class IntersectPoint {
public:
  Vector4D pos;
  double t;
  RGB color;

  /**
   * @brief uses default RGB
   */
  IntersectPoint(Vector4D, double);
  IntersectPoint(Vector4D, double, RGB);
};

class PrimitiveObject;

struct IntersectionResult {
  bool hit;
  IntersectPoint point;
  const PrimitiveObject *object = nullptr;
  Vector4D normal;

  IntersectionResult(bool, class IntersectPoint IntersectPoint,
                     const PrimitiveObject *, Vector4D normal);
  /**
   * @brief uses the normal function of the PrimitiveObject to determine normal
   */

  IntersectionResult(bool, class IntersectPoint IntersectPoint,
                     const PrimitiveObject *);

  /**
   * @brief constructor for not existing Interesection
   */
  IntersectionResult();
};

double scalarProduct(Vector4D, Vector4D);
