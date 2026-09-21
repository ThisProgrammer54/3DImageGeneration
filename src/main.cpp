
#include "helper.hpp"
#include "json_io.hpp"
#include "render.hpp"
#include "screen.hpp"
#include "space.hpp"
#include <fstream>

#include <Eigen/Dense>
#include <memory>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

using Eigen::MatrixXd;

std::unique_ptr<Object> makeCube(Vector4D start, double x, double y, double z,
                                 RGB color) {
  auto tmp = std::make_unique<Halfspace>(start, Vector4D(-1, 0, 0, 0), color);
  auto tmp2 = std::make_unique<Halfspace>(start, Vector4D(0, -1, 0, 0), color);
  auto tmp3 = std::make_unique<Halfspace>(start, Vector4D(0, 0, -1, 0), color);

  auto tmp4 = std::make_unique<Halfspace>(start + Vector4D(x, 0, 0, 0),
                                          Vector4D(1, 0, 0, 0), color);
  auto tmp5 = std::make_unique<Halfspace>(start + Vector4D(0, y, 0, 0),
                                          Vector4D(0, 1, 0, 0), color);
  auto tmp6 = std::make_unique<Halfspace>(start + Vector4D(0, 0, z, 0),
                                          Vector4D(0, 0, 1, 0), color);

  auto tmpCube = std::make_unique<Intersection>();
  tmpCube->add(std::move(tmp));
  tmpCube->add(std::move(tmp2));
  tmpCube->add(std::move(tmp3));
  tmpCube->add(std::move(tmp4));
  tmpCube->add(std::move(tmp5));
  tmpCube->add(std::move(tmp6));

  auto cube = std::make_unique<Object>(std::move(tmpCube));

  return cube;
}

int main() {

  const string JSON_FILE = "../json/input1.json";

  Screen screen(10, 10, 100, Vector4D(0, 0, 10, 1), Vector4D(0, 0, 5, 1));

  // Object creation
  auto tmp = std::make_unique<Halfspace>(Vector4D(0, 0, 0, 1),
                                         Vector4D(0, 0, 1, 0), RGB{1, 0, 1});

  auto tmp2 = std::make_unique<Halfspace>(Vector4D(1200, 0, 0, 1),
                                          Vector4D(-1, 0, 0, 0), RGB{0, 1, 0});
  // wall->add(std::move(tmp2));

  auto wall1 = std::make_unique<Object>(std::move(tmp));

  auto whiteLightSource =
      std::make_unique<LightSource>(Vector4D(-100, -100, 50, 1), RGB{1, 1, 1});
  auto redLightSource =
      std::make_unique<LightSource>(Vector4D(0, 0, 0, 1), RGB{1, 0, 0});

  auto cube = makeCube(Vector4D(10, 10, 0, 1), 2, 2, 1, RGB(1, 0.5, 0.5));
  auto cube2 = makeCube(Vector4D(5, 5, 0, 1), 2, 2, 1, RGB(0.5, 1, 0.5));

  Space space(RGB{0, 0, 0});
  space.add(std::move(whiteLightSource));
  // space.add(std::move(redLightSource));

  space.add(std::move(wall1));
  space.add(std::move(cube));
  // space.add(std::move(cube2));

  screen.calculate(space);

  Render render(1000, 1000);
  render.render(screen);
  render.show();
  return 0;
}
