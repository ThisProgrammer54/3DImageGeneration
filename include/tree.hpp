#include "helper.hpp"
#include <memory>
#include <vector>

#pragma once

class Tree {
public:
  std::vector<std::unique_ptr<Tree>> nodes;

  virtual IntersectionResult intersectPoint(Ray &) = 0;
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
  RGB ambientReflectionComponent;
  RGB diffuseReflectionComponent;
  RGB specularReflectionComponent;
  //simon: brauchen wir noch für den glanz von objekten, dachte das macht hier am meisten sinn, erstmal nur auf nen Wert
  //muss später noch mit dem konstruktor gemacht werden, hab einfach mal nen default genommen
  double shininess = 30;
  virtual double Polynom(Vector4D) = 0;
  //simon:
  virtual Vector4D getNormal() const = 0;
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

  Halfspace(Vector4D position, Vector4D normal, RGB specRC, RGB diffuseRC);
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
  double Polynom(Vector4D) override;
  //Simon:
  Vector4D getNormal() const override;
};
