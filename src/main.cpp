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

int main() {

  const string JSON_FILE = "../json/input1.json";

  Screen screen(10, 10, 100, Vector4D(0, 0, 0, 1), Vector4D(0, 0, 2, 1));

  // Object creation
  auto tmp = std::make_unique<Halfspace>(Vector4D(0, 0, 1000, 1),
                                         Vector4D(0, 0, -1, 0), RGB{1, 1, 1},
                                         RGB{1, 1, 1}, 30);
  auto tmp2 = std::make_unique<Halfspace>(Vector4D(10000, 0, 0, 1),
                                          Vector4D(-1, 0, 0, 0), RGB{1, 0, 0},
                                          RGB{1, 0, 0}, 30);

  auto overTmp = std::make_unique<Intersection>();
  // overTmp->add(std::move(tmp));
  // overTmp->add(std::move(tmp2));

  // auto wall1 = std::make_unique<Object>(std::move(tmp));
  auto wall2 = std::make_unique<Object>(std::move(tmp2));

  auto whiteLightSource =
      std::make_unique<LightSource>(Vector4D(100, 100, 0, 1), RGB{1, 1, 1});
  auto redLightSource = std::make_unique<LightSource>(
      Vector4D(1000, -1000, -100, 1), RGB{1, 0, 0});

  Space space(RGB{0, 0, 0});

  space.add(std::move(whiteLightSource));
  // space.add(std::move(redLightSource));

  // space.add(std::move(wall1));
  space.add(std::move(wall2));

  screen.calculate(space);

  Render render(1000, 1000);
  render.render(screen);
  render.show();
  return 0;
}
