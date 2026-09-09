#include "helper.hpp"
#include <memory>
#include <vector>

#pragma once

class Tree {
public:
  std::vector<std::unique_ptr<Tree>> nodes;

  virtual IntersectPoint intersectPoint(Ray &) = 0;
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

  Halfspace(Vector4D, Vector4D);
};
