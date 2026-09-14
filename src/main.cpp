#include "render.hpp"
#include "screen.hpp"
#include "space.hpp"
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

using Eigen::MatrixXd;

int main() {
  Space space;

  Screen screen(2, 10, 100, Vector4D(0, 0, 100, 0), Vector4D(0, 0, 0, 0));
  screen.calculate(space);

  Render render(1000, 200);
  render.render(screen);
  render.show();
  return 0;
}
