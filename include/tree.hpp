#include "helper.hpp"
#include "opencv2/core/matx.hpp"
#include <memory>
#include <vector>

#pragma once

#define DEFSHINY 0
#define DEFCOLOR (RGB(1, 0, 0))

class Tree {
public:
  std::vector<std::unique_ptr<Tree>> nodes;

  /**
   *@brief returns Intersection with tree
   */
  virtual IntersectionResult intersectPoint(Ray &) = 0;
  /**
  / @brief check if ray intersects with tree
  */
  virtual bool intersec(Ray &) = 0;

  /**
   * @brief checks if point is in tree
   * @param point
   */
  virtual bool inside(IntersectPoint &) = 0;

  void add(std::unique_ptr<Tree>);
};

class LogicalTree : public Tree {};

class Union : public LogicalTree {};
class Intersection : public LogicalTree {
  IntersectionResult intersectPoint(Ray &) override;
  bool intersec(Ray &) override;
  bool inside(IntersectPoint &) override;
};
class Exclusion : public LogicalTree {};

class TransformationTree : public Tree {};

class Translation : public TransformationTree {};
class Rotation : public TransformationTree {};
class Scaling : public TransformationTree {};

class PrimitiveObject : public Tree {
public:
  RGB ambientReflectionComponent;
  RGB diffuseReflectionComponent;
  RGB specularReflectionComponent;
  RGB reflectionComponent;
  RGB refractedComponent;

  double shininess;
  virtual double Polynom(Vector4D) const = 0;
  virtual Vector4D getNormal(Vector4D point) const = 0;
};
class Sphere : public PrimitiveObject {
public:
  Vector4D position;
  double radius;

  Sphere(Vector4D position, double radius, RGB ambRC, RGB specRC, RGB diffuseRC,
         RGB reflec, RGB refrac, double shininess);
  Sphere(Vector4D position, double radius, RGB color);
  Sphere(Vector4D position, double radius);

  IntersectionResult intersectPoint(Ray &ray) override;
  bool intersec(Ray &) override;
  bool inside(IntersectPoint &a) override;

  double Polynom(Vector4D) const override;
  Vector4D getNormal(Vector4D point) const override;
};
class Halfspace : public PrimitiveObject {
public:
  Vector4D position;
  Vector4D normal;

  Halfspace(Vector4D position, Vector4D normal, RGB ambRC, RGB specRC,
            RGB diffuseRC, RGB reflec, RGB refrac, double shininess);

  Halfspace(Vector4D position, Vector4D normal, RGB color);
  /**
   * @brief construcst the Halfspace
   * @param point on plane
   * @param normal normal vector away from plane
   */
  Halfspace(Vector4D position, Vector4D normal);

  /**
   * @brief calculates intersection of Ray with itself
   * @param ray the Ray
   * @return intersectionpointPtr or nullptr if not existing
   */
  IntersectionResult intersectPoint(Ray &ray) override;
  bool intersec(Ray &) override;
  bool inside(IntersectPoint &a) override;

  double Polynom(Vector4D) const override;
  // Simon:
  Vector4D getNormal(Vector4D point) const override;
};
