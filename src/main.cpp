#include "helper.hpp"
#include "json_io.hpp"
#include "render.hpp"
#include "screen.hpp"
#include "space.hpp"

#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

using Eigen::MatrixXd;



int main() {

  string json_file = "json/cur.json";
  SceneLoader loader(json_file);
  json scene = loader.load();

  Screen screen = makeScreen(scene);
  Space space = makeSpace(scene);
  SzeneContext ctx = makeMaterials(scene); 

  makeLights(scene, space); // reads json parameters & add them to the space
  makeObjects(scene, space, ctx);

  screen.calculate(space);

  Render render(screen.width * screen.dpi, screen.height * screen.dpi);
  render.render(screen);
  // render.show();
  render.write("img/result.png");
  return 0;
}
