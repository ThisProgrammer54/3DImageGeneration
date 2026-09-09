#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "helper.hpp"
#include "space.hpp"
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
      cout << "now running: " << test.name << endl;

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

// ------- Test ende -------

int main(void) {
  Tester tester;
  tester.addTest("testing ray construction", constructorTest);
  tester.runAll();

  cout << "hello world" << endl;
}
