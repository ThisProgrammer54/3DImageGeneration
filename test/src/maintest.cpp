#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "helper.hpp"
#include "space.hpp"
#include "tree.hpp"
#include <screen.hpp>

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

  Screen screen(100, 200, 200, Vector4D(1, 2, 3, 4), Vector4D(1, 2, 3, 4));

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
  assert(obj.intersectPoint(ray4)->pos == Vector4D(-1, 2, 0, 0));
  assert(obj.intersectPoint(ray4)->t == 101);

  return true;
}

bool pixelIntersectGetTest() { return true; }

bool screenSetupTest() {
  //Instantiate the screen
  Screen testScreen(10, 10, 64, Vector4D(0, 0, 0, 1), Vector4D(-5, -5, 10, 1));

  //testScreen.setDimensions(20, 20);

  Space emptySpace;

  testScreen.calculate(emptySpace);

  return true;
}

// ------- Test ende -------

int main(void) {
  Tester tester;
  tester.addTest("testing ray construction", constructorTest);
  tester.addTest("Object", objectTest);
  tester.addTest("testing intersection getter (TODO!)", pixelIntersectGetTest);
  tester.runAll();
}
