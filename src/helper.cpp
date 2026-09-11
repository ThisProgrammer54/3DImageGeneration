#include <helper.hpp>

RGB::RGB(double red, double green, double blue) : red(red), green(green), blue(blue) {};
RGB::RGB() : red(0.0), green(0.0), blue(0.0) {};
Ray::Ray(Vector4D pos, Vector4D d) : pos(pos), d(d){};

RGB operator+(const RGB &a, const RGB &b){
    return RGB(1 - (1 - a.red) * (1 - b.red), 1 - (1 - a.green) * (1 - b.green), 1 - (1 - a.blue) * (1 - b.blue));
}
RGB operator*(const RGB &a, const RGB &b){
    return RGB(a.red * b.red, a.green * b.green, a.blue * b.blue);
}
RGB operator*(const RGB &a, double b){
    return RGB(a.red * b, a.green * b, a.blue * b);
}
RGB operator*(double b, const RGB &a){
    return RGB(a.red * b, a.green * b, a.blue * b);
}

IntersectPoint::IntersectPoint(Vector4D pos, double t)
    : pos(pos), t(t), color(RGB{-1, -1, -1}){};

IntersectPoint::IntersectPoint(Vector4D pos, double t, RGB color)
    : pos(pos), t(t), color(color){};

double scalarProduct(Vector4D a, Vector4D b) {
  return a(0) * b(0) + a(1) * b(1) + a(2) * b(2);
}
