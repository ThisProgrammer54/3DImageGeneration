#include <helper.hpp>

Ray::Ray(Vector4D pos, Vector4D d) : pos(pos), d(d){};

IntersectPoint::IntersectPoint(Vector4D pos, Vector4D t, RGB color) : pos(pos), t(t), color(color) {};