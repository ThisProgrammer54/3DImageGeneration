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

bool sphereTest() {
  auto sphere = std::make_unique<Sphere>(Vector4D(0, 0, 100, 1), 1);

  Ray ray(Vector4D(0, 0, 0, 1), Vector4D(1, 0, 0, 0));
  Ray ray2(Vector4D(0, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray3(Vector4D(10, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray4(Vector4D(1, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray5(Vector4D(2, 0, 98, 1), Vector4D(0, 0, 1, 0));

  assert(!sphere->intersec(ray));
  assert(sphere->intersec(ray2));
  assert(!sphere->intersec(ray3));
  assert(sphere->intersec(ray4));
  assert(!sphere->intersec(ray5));

  return true;
}
bool unionTest() {
  auto sphere = std::make_unique<Sphere>(Vector4D(0, 0, 100, 1), 1);
  auto plane = std::make_unique<Halfspace>(Vector4D(0, 0, 200, 1),
                                           Vector4D(0, 0, -1, 0));

  auto test = std::make_unique<Union>();
  test->add(std::move(sphere));
  test->add(std::move(plane));

  Ray ray(Vector4D(0, 0, 0, 1), Vector4D(1, 0, 0, 0));
  Ray ray2(Vector4D(0, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray3(Vector4D(10, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray4(Vector4D(0, 0, 110, 1), Vector4D(1, 1, 0, 0));
  Ray ray5(Vector4D(2, 0, 98, 1), Vector4D(0, 0, 1, 0));
  Ray ray6(Vector4D(0, 0, 150, 1), Vector4D(1, 0, 0, 0));

  assert(test->intersec(ray2));
  assert(test->intersec(ray3));
  assert(test->intersec(ray5));

  assert(!test->intersec(ray));
  assert(!test->intersec(ray4));
  assert(!test->intersec(ray6));
  return true;
}
bool exclusionTest() {
  auto plane1 = std::make_unique<Halfspace>(Vector4D(0, 0, 100, 1),
                                            Vector4D(0, 0, -1, 0));
  auto plane2 =
      std::make_unique<Halfspace>(Vector4D(0, 0, 50, 1), Vector4D(0, 0, -1, 0));

  auto test = std::make_unique<Exclusion>();
  test->add(std::move(plane1));
  test->add(std::move(plane2));

  Ray ray(Vector4D(0, 0, 0, 1), Vector4D(1, 0, 0, 0));
  Ray ray2(Vector4D(0, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray3(Vector4D(10, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray4(Vector4D(1, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray5(Vector4D(0, 0, 105, 1), Vector4D(0, 0, -1, 0));
  Ray ray6(Vector4D(0, 0, 150, 1), Vector4D(1, 0, 0, 0));

  assert(test->intersec(ray2));
  assert(test->intersec(ray3));
  assert(test->intersec(ray4));
  assert(test->intersec(ray5));

  assert(!test->intersec(ray));
  assert(!test->intersec(ray6));
  return true;
}
bool intersectionTest() {
  auto plane1 = std::make_unique<Halfspace>(Vector4D(0, 0, 100, 1),
                                            Vector4D(0, 0, -1, 0));
  auto plane2 =
      std::make_unique<Halfspace>(Vector4D(0, 0, 50, 1), Vector4D(0, 0, -1, 0));

  auto test = std::make_unique<Intersection>();
  test->add(std::move(plane1));
  test->add(std::move(plane2));

  Ray ray(Vector4D(0, 0, 0, 1), Vector4D(1, 0, 0, 0));
  Ray ray2(Vector4D(0, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray3(Vector4D(10, 0, 0, 1), Vector4D(0, 0, 1, 0));
  Ray ray4(Vector4D(1, 0, 70, 1), Vector4D(0, 0, 1, 0));
  Ray ray5(Vector4D(0, 0, 70, 1), Vector4D(1, 1, 0, 0));
  Ray ray6(Vector4D(0, 0, 80, 1), Vector4D(1, 1, -1, 0));

  assert(test->intersec(ray2));
  assert(test->intersec(ray3));
  assert(test->intersec(ray4));

  assert(!test->intersec(ray));
  assert(!test->intersec(ray5));
  assert(!test->intersec(ray6));

  return true;
}

// ------- Test ende -------

int main(void) {
  Tester tester;
  tester.addTest("construction", constructorTest);
  tester.addTest("Object", objectTest);
  tester.addTest("intersection getter (TODO!)", pixelIntersectGetTest);
  tester.addTest("screen setup", screenSetupTest);
  tester.addTest("sphere", sphereTest);
  tester.addTest("union", unionTest);
  tester.addTest("exclusion", exclusionTest);
  tester.addTest("intersection", intersectionTest);
  tester.runAll();
}
