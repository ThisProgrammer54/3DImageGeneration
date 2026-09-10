#include "helper.hpp"
#include <memory>
#include <vector>

#pragma once

class Tree {
public:
  std::vector<std::unique_ptr<Tree>> nodes;

  virtual std::unique_ptr<IntersectPoint> intersectPoint(Ray &) = 0;
  virtual bool intersec(Ray &) = 0;

  void add(std::unique_ptr<Tree>);
};

class LogicalTree : public Tree {};
class Union : public LogicalTree {};
class Intersection : public LogicalTree {};
class Exclusion : public LogicalTree {};

class TransformationTree : public Tree {};
class Translation : public TransformationTree {};
class Rotation : public TransformationTree {};
class Scaling : public TransformationTree {};

class ObjectTree : public Tree {};
class PrimitiveObject : public ObjectTree {
public:
  virtual double Polynom(Vector4D) = 0;
};
class Sphere : public PrimitiveObject {
public:
  Vector4D Position;
  double radius;
};
class Halfspace : public PrimitiveObject {
public:
  Vector4D position;
  Vector4D normal;

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
  std::unique_ptr<IntersectPoint> intersectPoint(Ray &ray) override;
  bool intersec(Ray &) override;
  double Polynom(Vector4D) override;
};
