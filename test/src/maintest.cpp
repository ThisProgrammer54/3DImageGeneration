#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "helper.hpp"

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

bool rayTest() {
  Vector4D pos(10, 20, 30, 40);
  Vector4D d(1, 1, 1, 34);
  Ray ray(pos, d);

  assert(ray.d == d && ray.pos == pos && ray.d(3) == d(3));

  return true;
}

// ------- Test ende -------

int main(void) {
  Ray ray; // testing
  Tester tester;
  tester.addTest("testing ray construction", rayTest);
  tester.runAll();

  cout << "hello world" << endl;
}
