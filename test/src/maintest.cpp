#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "helper.hpp"
#include "space.hpp"
#include "tree.hpp"
#include <screen.hpp>
#include <xmmintrin.h>

using std::string, std::vector, std::cout, std::endl;

class Test {
public:
  string name;
  bool (*func)();
  Test(const string &name, bool (*func)()) : name(name), func(func){};
};

class Tester {
  vector<Test> tests;

public:
  void addTest(const string &name, bool (*func)()) {
    this->tests.push_back(Test(name, func));
  }
  bool runAll() {
    cout << "starte Tests" << endl;
    for (auto &test : this->tests) {
      cout << "now running: " << test.name << " test" << endl;

      if (!test.func()) {
        return false;
      }
    }
    return true;
  }
};

// ----- Test ----

bool constructorTest() {

  Vector4D pos(10, 20, 30, 40);
  Vector4D d(1, 1, 1, 34);
  Ray ray(pos, d);

  assert(ray.d == d && ray.pos == pos && ray.d(3) == d(3));

  Pixel pixel(10, 20, RGB{0, 1, 0}, Vector4D(1, 100, 1, 1));

  assert(pixel.x == 10 && pixel.y == 20 && pixel.value.green == 1 &&
         pixel.visPoint(1) == 100);

  double dimX = 2;
  double dimY = 1;
  double dpi = 200;

  Screen screen(dimY, dimX, dpi, Vector4D(1, 2, 3, 4), Vector4D(1, 2, 3, 4));
  Space space(RGB{0, 0, 0});
  screen.calculate(space);
  for (int y = 0; y < dimY * dpi; y++) {
    for (int x = 0; x < dimX * dpi; x++) {
      Pixel &pix = screen.values[y][x];
      assert(pix.x == x && pix.y == y && pix.value == RGB(0, 0, 0));
    }
  }

  return true;
}

bool objectTest() {
  std::unique_ptr<Halfspace> l = std::make_unique<Halfspace>(
      Vector4D(-1, 100, 200, 1), Vector4D(1, 0, 0, 0));

  assert(l->position == Vector4D(-1, 0, 0, 1));
  Ray ray1(Vector4D(0, 0, 0, 0), Vector4D(-1, 0, 0, 0));
  Ray ray2(Vector4D(100, 0, 0, 0), Vector4D(1, 0, 0, 0));
  Ray ray3(Vector4D(100, 0, 0, 0), Vector4D(-1, 0, 0, 0));
  Ray ray4(Vector4D(100, 2, 0, 0), Vector4D(-1, 0, 0, 0));

  assert(l->intersec(ray1));
  assert(!l->intersec(ray2));
  assert(l->intersec(ray3));

  Object obj(std::move(l));
  assert(obj.intersectPoint(ray4).point.pos == Vector4D(-1, 2, 0, 0));
  assert(obj.intersectPoint(ray4).point.t == 101);

  return true;
}

bool pixelIntersectGetTest() {
  auto lbackward = std::make_unique<Halfspace>(Vector4D(-1, 100, 200, 1),
                                               Vector4D(1, 0, 0, 0));
  auto lforward = std::make_unique<Halfspace>(Vector4D(0, 100, 200, 1),
                                              Vector4D(1, 0, 0, 0));

  auto obj1 = std::make_unique<Object>(std::move(lbackward));
  auto obj2 = std::make_unique<Object>(std::move(lforward));

  Space space(RGB(0, 0, 0));
  space.add(std::move(obj1));
  space.add(std::move(obj2));

  Ray ray1(Vector4D(100, 2, 67, 0), Vector4D(-1, 0, 0, 0));

  Pixel pix(0, 0, {0, 0, 0},
            Vector4D{
                0,
                0,
                0,
                0,
            });
  IntersectionResult res = pix.getIntersectionPoint(space, ray1);

  assert(res.point.pos == Vector4D(0, 2, 67, 0));

  return true;
}

bool screenSetupTest() {
  // Instantiate the screen
  Screen testScreen(10, 10, 64, Vector4D(0, 0, 0, 1), Vector4D(-5, -5, 10, 1));

  // testScreen.setDimensions(20, 20);

  Space emptySpace(RGB(0, 0, 0));

  testScreen.calculate(emptySpace);

  return true;
}

bool SphereTest() {
  auto sphere = std::make_unique<Sphere>(Vector4D(0, 0, 100, 1), 10);
  return true;
}

// ------- Test ende -------

int main(void) {
  Tester tester;
  tester.addTest("construction", constructorTest);
  tester.addTest("Object", objectTest);
  tester.addTest("intersection getter (TODO!)", pixelIntersectGetTest);
  tester.addTest("screen setup", screenSetupTest);
  tester.runAll();
}
