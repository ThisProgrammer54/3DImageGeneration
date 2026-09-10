#include <cstddef>
#include <screen.hpp>

Pixel::Pixel(int x, int y, RGB value, Vector4D visPoint)
    : x(x), y(y), value(value), visPoint(visPoint){};
Pixel::Pixel() : x(0), y(0), value(RGB()), visPoint() {}



IntersectPoint Pixel::getIntersectionPoint(const Space &space,
                                           const Ray &ray) const {
  // iterate over every object in the space
  std::vector<double> tCandidates;
  double lowestT = INFINITY;
  int tIndex = -1;
  for (size_t i = 0; i < space.objects.size(); ++i) {
    tCandidates.push_back(space.objects[i].get()->intersectPoint(ray)->t); // vorher: space.objects[i].get()->intersectPoint(ray).t
    if (*tCandidates.end() < lowestT) {
      lowestT = *tCandidates.end();
      tIndex = i;
    }
  }
  if (tIndex != -1) {
    return space.objects[tIndex].get()->intersectPoint(ray);
  }
  // else { return NULL; }  // cannot convert intersectPoint to NULL, special
  // method/constructor needed? what to return when there's no intersection
  // point? does Ray ray assume it has hit something?
  // --
  // we could return a uniq pointer and a nullptr like object/Halfspace
  // Simon: was spricht gegen kein return also nur: return; ?

}

RGB Pixel::getColor(const Space &space, const IntersectPoint &intersectionPoint) const{
  // eigentlich ne funktion ob ein licht etwas anscheind und wenn nicht wird die Farbe schwarz zurückgegeben. die Funktion intersectionpoint gibt eigentlich schon die farbe an
  
  bool anscheinen = false;

  for (const auto &light : space.lightsource){
    Vector4D toLight = light->pos - intersectionPoint.pos;
    double distanceToLight = toLight.norm();
    Vector4D direction = toLight.normalized();

    Ray shadowRay(intersectionPoint.pos, direction);

    IntersectPoint shadowHit = getIntersectionPoint(space, shadowRay);

    if (shadowHit.t >= distanceToLight) {
      anscheinen = true;
      break;
    }
  }

  if (anscheinen) {return intersectionPoint.color;} else {return RGB(0.0, 0.0, 0.0);}
}

Screen::Screen(int height, int width, int dpi, Vector4D observer, Vector4D position): height(height), width(width), dpi(dpi), observer(observer), position(position), values(height, vector<Pixel>(width)){};

/*void Screen::setDimensions(int height, int width){
  this->height = height;
  this->width = width;
}*/


void Screen::calculate(const Space& space){

  double dx = 1.0 / dpi;

  for (int y = 0; y < height; ++y){
    for (int x = 0; x < width; ++x){

      //3d center point of the current pixel 
      double px = position(0) + (x+0.5)*dx;
      double py = position(1) + (y+0.5)*dx;
      double pz = position(2);

      Vector4D pixelCenter(px,py,pz,1.0);

      Vector4D direction = (pixelCenter - observer);
      direction.normalize();

      //create a ray
      Ray currentRay(observer, direction);

      values[y][x].x = x; 
      values[y][x].y = y; 
      values[y][x].visPoint = pixelCenter;
      values[y][x].shootRay(space, currentRay);

    }
  }
};