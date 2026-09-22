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

  string json_file = "json/input1.json";
  SceneLoader loader(json_file);
  json scene = loader.load();

  Screen screen = makeScreen(scene);
  Space space = makeSpace(scene);
  makeLights(scene, space);
  // makeLights should read json parameters & add them to the space

  // auto whiteLightSource =
  //     std::make_unique<LightSource>(Vector4D(3, 3, 10, 1), RGB{1, 1, 1});
  // auto redLightSource =
  //     std::make_unique<LightSource>(Vector4D(1000, 0, 0, 1), RGB{1, 0, 0});

  // space.add(std::move(whiteLightSource));
  // space.add(std::move(redLightSource));

  // Object creation
  auto tmp = std::make_unique<Halfspace>(Vector4D(0, 0, 0, 1),
                                         Vector4D(0, 0, 1, 0), RGB{1, 1, 1});

  auto wall1 = std::make_unique<Object>(std::move(tmp));

  auto tmp2 = std::make_unique<Sphere>(Vector4D(2, 2, 2, 1), 1, RGB(0, 0, 1));
  auto transf =
      std::make_unique<Translation>(std::move(tmp2), Vector4D(0, 0, 0, 1));

  auto sphere1 = std::make_unique<Object>(std::move(transf));

  auto cube = makeCube(Vector4D(10, 10, 0, 1), 2, 2, 1, RGB(1, 0.5, 0.5));
  auto cube2 = makeCube(Vector4D(5, 5, 0, 1), 2, 2, 1, RGB(0.5, 1, 0.5));

  space.add(std::move(wall1));
  space.add(std::move(cube));
  // space.add(std::move(cube2));
  space.add(std::move(sphere1));

  screen.calculate(space);

  Render render(screen.width * screen.dpi, screen.height * screen.dpi);
  render.render(screen);
  render.show();
  return 0;
}
