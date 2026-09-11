#include "helper.hpp"
#include "space.hpp"
#include <any>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <ostream>
#include <screen.hpp>

Pixel::Pixel(int x, int y, RGB value, Vector4D visPoint)
    : x(x), y(y), value(value), visPoint(visPoint){};
Pixel::Pixel() : x(0), y(0), value(RGB()), visPoint() {}

IntersectionResult Pixel::getIntersectionPoint(const Space &space,
                                               const Ray &ray) const {
  // iterate over every object in the space
  double lowestT = INFINITY;
  IntersectionResult finalResult{nullptr, nullptr};
  for (size_t i = 0; i < space.objects.size(); ++i) {
    IntersectionResult result = space.objects[i]->intersectPoint(ray);
    if (result.object == nullptr)
      continue;

    if (result.point->t < lowestT) {
      lowestT = result.point->t;
      finalResult = std::move(result);
    }
  }
  if (finalResult.object != nullptr) {
    return finalResult;
  }
  return IntersectionResult{nullptr, nullptr};
}

RGB Pixel::getColor(const Space &space,
                    const IntersectionResult &intersectionResult) const {
  // eigentlich ne funktion ob ein licht etwas anscheind und wenn nicht wird die
  // Farbe schwarz zurückgegeben. die Funktion intersectionpoint gibt eigentlich
  // schon die farbe an
  // philip: wir muessen noch die verschiedenen Farbkoeffizienten implemntieren
  // (darum habe ich es zu IntersectionResult ergaenzt)

  if (intersectionResult.object == nullptr)
    return RGB(0, 0, 0);

  bool anscheinen = false;
  double offset = 0.1; // philip: noch besprechen und testen aber scheint sich
                       // selber als hindernis wahrzunehmen

  for (const auto &light : space.lightsource) {
    Vector4D toLight = light->pos - intersectionResult.point->pos;
    double distanceToLight = toLight.norm();
    Vector4D direction = toLight.normalized();

    Ray shadowRay(intersectionResult.point->pos + direction * offset,
                  direction);

    IntersectionResult shadowHit = getIntersectionPoint(space, shadowRay);

    if (shadowHit.object == nullptr || shadowHit.point->t >= distanceToLight) {
      anscheinen = true;
      // TODO mehr als eine lightsource und operator overload

      RGB color =
          RGB(intersectionResult.object->diffuseReflectionComponent.red *
                  light->intensity.red,
              intersectionResult.object->diffuseReflectionComponent.green *
                  light->intensity.green,
              intersectionResult.object->diffuseReflectionComponent.blue *
                  light->intensity.blue);

      return color;
      break;
    }
  }
  return RGB(0.0, 0.0, 0.0);
}

void Pixel::shootRay(const Space &space, const Ray &ray) {
  IntersectionResult res = this->getIntersectionPoint(space, ray);
  this->value = getColor(space, res);
}

Screen::Screen(int height, int width, int dpi, Vector4D observer,
               Vector4D position)
    : height(height), width(width), dpi(dpi), observer(observer),
      position(position), values(height, vector<Pixel>(width)){};

/*void Screen::setDimensions(int height, int width){
  this->height = height;
  this->width = width;
}*/

void Screen::calculate(const Space &space) {

  double dx = 1.0 / dpi;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {

      // 3d center point of the current pixel
      double px = position(0) + (x + 0.5) * dx;
      double py = position(1) + (y + 0.5) * dx;
      double pz = position(2);

      Vector4D pixelCenter(px, py, pz, 1.0);

      Vector4D direction = (pixelCenter - observer);
      direction.normalize();

      // create a ray
      Ray currentRay(observer, direction);

      values[y][x].x = x;
      values[y][x].y = y;
      values[y][x].visPoint = pixelCenter;
      values[y][x].shootRay(space, currentRay);
    }
  }
};

std::ostream &operator<<(std::ostream &out, const Screen &in) {
  for (int y = in.height - 1; y >= 0; y--) {
    for (int x = 0; x < in.width; x++) {
      RGB val = in.values[y][x].value;
      if (val.red == 1) {
        out << "R";
      } else if (val.green == 1) {
        out << "G";
      } else if (val.blue == 1) {
        out << "B";
      } else {
        out << "S";
      }

      out << " ";
    }
    out << std::endl;
  }
  out << std::endl;
  return out;
}
