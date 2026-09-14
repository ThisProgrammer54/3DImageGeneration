#include "render.hpp"
#include "screen.hpp"
#include "space.hpp"
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

using Eigen::MatrixXd;

int main() {
  auto tmp = std::make_unique<Halfspace>(Vector4D(0, 0, 1000, 1),
                                         Vector4D(0, 0, -1, 0), RGB{1, 1, 1},
                                         RGB{1, 1, 1});
  auto wall1 = std::make_unique<Object>(std::move(tmp));

  tmp =
      std::make_unique<Halfspace>(Vector4D(600, 0, 0, 1), Vector4D(-1, 0, 0, 0),
                                  RGB{0, 1, 0}, RGB{0, 1, 0});
  auto wall2 = std::make_unique<Object>(std::move(tmp));

  auto whiteLightSource =
      std::make_unique<LightSource>(Vector4D(0, 0, -100, 1), RGB{1, 1, 1});
  auto redLightSource = std::make_unique<LightSource>(
      Vector4D(1000, 1000, -100, 1), RGB{1, 0, 0});

  Space space;
  space.add(std::move(whiteLightSource));
  space.add(std::move(redLightSource));

  space.add(std::move(wall1));
  space.add(std::move(wall2));

  Screen screen(10, 10, 100, Vector4D(0, 0, 0, 1), Vector4D(0, 0, 2, 1));
  screen.calculate(space);

  Render render(1000, 1000);
  render.render(screen);
  render.show();
  return 0;
}
