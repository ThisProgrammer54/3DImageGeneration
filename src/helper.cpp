#include <helper.hpp>

RGB::RGB(double red, double green, double blue) : red(red), green(green), blue(blue) {};
RGB::RGB() : red(0.0), green(0.0), blue(0.0) {};
Ray::Ray(Vector4D pos, Vector4D d) : pos(pos), d(d){};

IntersectPoint::IntersectPoint(Vector4D pos, double t)
    : pos(pos), t(t), color(RGB{-1, -1, -1}){};

IntersectPoint::IntersectPoint(Vector4D pos, double t, RGB color)
    : pos(pos), t(t), color(color){};

double scalarProduct(Vector4D a, Vector4D b) {
  return a(0) * b(0) + a(1) * b(1) + a(2) * b(2);
}
