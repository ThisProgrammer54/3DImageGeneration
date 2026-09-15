#include "helper.hpp"
#include "space.hpp"
#include <cstddef>
#include <screen.hpp>

Pixel::Pixel(int x, int y, RGB value, Vector4D visPoint)
    : x(x), y(y), value(value), visPoint(visPoint){};
Pixel::Pixel() : x(0), y(0), value(RGB()), visPoint() {}

IntersectionResult Pixel::getIntersectionPoint(const Space &space,
                                               const Ray &ray) const {
  // iterate over every object in the space
  double lowestT = INFINITY;
  IntersectionResult finalResult;
  for (size_t i = 0; i < space.objects.size(); ++i) {
    IntersectionResult result = space.objects[i]->intersectPoint(ray);
    if (result.object == nullptr)
      continue;

    if (result.point.t < lowestT) {
      lowestT = result.point.t;
      finalResult = std::move(result);
    }
  }
  if (!finalResult.hit) {
    return finalResult;
  }
  return finalResult;
}

RGB Pixel::getColor(const Space &space,
                    const IntersectionResult &intersectionResult,
                    const Ray &ray) const {

  if (!intersectionResult.hit)
    return RGB(0, 0, 0);

  const PrimitiveObject *object = intersectionResult.object;
  Vector4D normal = object->getNormal();
  // V = Vektor vom Schnittpunkt zum Ursprung
  Vector4D V = -ray.d;
  V.normalize();

  double offset = 0.01;

  RGB ambient =
      object->ambientReflectionComponent * space.ambientLightIntensity;

  RGB sumLightComponents(0.0, 0.0, 0.0); // i.e. diffuse + specular

  for (const auto &light : space.lightsource) {

    Vector4D toLight = light->pos - intersectionResult.point.pos;
    double distanceToLight = toLight.norm();
    Vector4D direction = toLight.normalized();

    Ray shadowRay(intersectionResult.point.pos + direction * offset, direction);

    // Diffuser Anteil: lambertsches Gesetz: Ed = Kd * IL * (L.N)
    double lambert = scalarProduct(direction, normal);
    if (lambert <= 0) {
      continue;
    } // Fläche Zeigt vom Licht weg, wird also nicht angeläuchtet

    IntersectionResult shadowHit = getIntersectionPoint(space, shadowRay);

    bool schatten =
        shadowHit.object != nullptr && shadowHit.point.t < distanceToLight;
    if (schatten) {
      continue;
    }

    RGB diffuse =
        object->diffuseReflectionComponent * light->intensity * lambert;

    // Spekularer Anteil: Es = Ks * IL * (R.V)^alpha
    RGB spekular(0.0, 0.0, 0.0);
    if (object->shininess > 0.0) {

      Vector4D R = normal * (2 * lambert) - direction;
      double RV = scalarProduct(R, V);
      double RVFactor = std::pow(RV, object->shininess);
      spekular =
          object->specularReflectionComponent * light->intensity * RVFactor;
    }

    sumLightComponents = sumLightComponents + (diffuse + spekular);
  }

  // reflektierende und gebrochener Anteil fehlen noch
  RGB reflected = RGB(0, 0, 0); //  E_r = K_r * I_r
  RGB refracted = RGB(0, 0, 0); //  E_f = K_f * I_f

  RGB allComponents = ambient + sumLightComponents + reflected + refracted;
  return allComponents;
}

void Pixel::shootRay(const Space &space, const Ray &ray) {
  IntersectionResult res = this->getIntersectionPoint(space, ray);
  this->value = getColor(space, res, ray);
}

Screen::Screen(int height, int width, int dpi, Vector4D observer,
               Vector4D position)
    : height(height), width(width), dpi(dpi), observer(observer),
      position(position), values(height * dpi, vector<Pixel>(width * dpi)){};

/*void Screen::setDimensions(int height, int width){
  this->height = height;
  this->width = width;
}*/

void Screen::calculate(const Space &space) {

  double dx = 1.0 / dpi;

  for (size_t y = 0; y < this->values.size(); ++y) {
    for (size_t x = 0; x < this->values[y].size(); ++x) {

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
