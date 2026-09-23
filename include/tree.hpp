#include "helper.hpp"
#include "opencv2/core/matx.hpp"
#include <memory>
#include <vector>

#pragma once

#define DEFSHINY 0
#define DEFCOLOR (RGB(1, 0, 0))

class Tree {
public:
  /**
   *@brief returns Intersection with tree
   */
  virtual IntersectionResult intersectPoint(Ray &) = 0;
  /**
  / @brief check if ray intersects with tree
  */
  virtual bool intersec(Ray &);

  /**
   * @brief checks if point is in tree
   * @param point
   */
  virtual bool inside(IntersectPoint &) = 0;
};

class LogicalTree : public Tree {
public:
  std::vector<std::unique_ptr<Tree>> nodes;

  IntersectionResult intersectPoint(Ray &) override;
  void add(std::unique_ptr<Tree>);
};

class Union : public LogicalTree {
public:
  bool inside(IntersectPoint &) override;
};
class Intersection : public LogicalTree {
  bool inside(IntersectPoint &) override;
};
class Exclusion : public LogicalTree {
public:
  bool inside(IntersectPoint &) override;
};

class TransformationTree : public Tree {
  virtual void buildMatrices(const Vector4D in) = 0;
  virtual void buildInverses() = 0;
  virtual Vector4D applyInverse(const Vector4D in) const = 0;
  virtual Vector4D applyTransform(const Vector4D in) const = 0;

public:
  std::unique_ptr<Tree> node;

public:
  IntersectionResult intersectPoint(Ray &) override;
  bool inside(IntersectPoint &a) override;
};

class Translation : public TransformationTree {
protected:
  Matrix4D mat;
  Matrix4D inv;
  void buildMatrices(const Vector4D in) override;
  void buildInverses() override;
  Vector4D applyInverse(const Vector4D in) const override;
  Vector4D applyTransform(const Vector4D in) const override;

public:
  Translation(std::unique_ptr<Tree> node, Vector4D tranformation);
};
class Rotation : public TransformationTree {
protected:
  Matrix4D matX;
  Matrix4D matY;
  Matrix4D matZ;

  Matrix4D invX;
  Matrix4D invY;
  Matrix4D invZ;

  Vector4D rotation;
  void buildMatrices(const Vector4D in) override;
  void buildInverses() override;
  Vector4D applyInverse(const Vector4D in) const override;
  Vector4D applyTransform(const Vector4D in) const override;

public:
  Rotation(std::unique_ptr<Tree> node, Vector4D tranformation);
};
class Scaling : public TransformationTree {

protected:
  Matrix4D mat;
  Matrix4D inv;
  void buildMatrices(const Vector4D in) override;
  void buildInverses() override;
  Vector4D applyInverse(const Vector4D in) const override;
  Vector4D applyTransform(const Vector4D in) const override;

public:
  Scaling(std::unique_ptr<Tree> node, Vector4D tranformation);
};

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
  bool inside(IntersectPoint &a) override;

  double Polynom(Vector4D) const override;
  Vector4D getNormal(Vector4D point) const override;
};
