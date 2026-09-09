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

bool testTest() {
  assert(3 + 3 < 100);
  assert(10 + 20 == 30);
  return true;
}

// ------- Test ende -------

int main(void) {
  Ray ray; // testing
  Tester tester;
  tester.addTest("simple addition", testTest);
  tester.runAll();

  cout << "hello world" << endl;
}
