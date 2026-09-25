#include "tree.hpp"
#include <helper.hpp>

RGB::RGB(double red, double green, double blue)
    : red(red), green(green), blue(blue){};
RGB::RGB() : red(0.0), green(0.0), blue(0.0){};
Ray::Ray(Vector4D pos, Vector4D d) : pos(pos), d(d){};

RGB operator+(const RGB &a, const RGB &b) {
  return RGB(1 - (1 - a.red) * (1 - b.red), 1 - (1 - a.green) * (1 - b.green),
             1 - (1 - a.blue) * (1 - b.blue));
}
RGB operator*(const RGB &a, const RGB &b) {
  return RGB(a.red * b.red, a.green * b.green, a.blue * b.blue);
}
RGB operator*(const RGB &a, double b) {
  return RGB(a.red * b, a.green * b, a.blue * b);
}
RGB operator*(double b, const RGB &a) {
  return RGB(a.red * b, a.green * b, a.blue * b);
}
bool operator==(const RGB &a, const RGB &b) {
  return (a.red == b.red && a.blue == b.blue && a.green == b.green);
}

Material::Material()
    : ambient(), diffuse(), specular(), reflected(), refracted(),
      shininess(DEFSHINY){};
Material::Material(RGB ambient, RGB diffuse, RGB specular, RGB reflected,
                   RGB refracted, double shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular),
      reflected(reflected), refracted(refracted), shininess(shininess){};

IntersectPoint::IntersectPoint(Vector4D pos, double t)
    : pos(pos), t(t), color(){};

IntersectPoint::IntersectPoint(Vector4D pos, double t, RGB color)
    : pos(pos), t(t), color(color){};

IntersectionResult::IntersectionResult(bool hit, IntersectPoint intersectPoint,
                                       const PrimitiveObject *obj,
                                       Vector4D normal)
    : hit(hit), point(intersectPoint), object(obj), normal(normal){};

IntersectionResult::IntersectionResult(bool hit, IntersectPoint intersectPoint,
                                       const PrimitiveObject *obj)
    : hit(hit), point(intersectPoint), object(obj),
      normal(obj->getNormal(intersectPoint.pos)){};
IntersectionResult::IntersectionResult()
    : hit(false), point(Vector4D(0, 0, 0, -1), -1){};

double scalarProduct(Vector4D a, Vector4D b) {
  return a(0) * b(0) + a(1) * b(1) + a(2) * b(2);
}
