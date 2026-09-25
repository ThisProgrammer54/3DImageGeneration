#include "helper.hpp"
#include "json_io.hpp"
#include "render.hpp"
#include "screen.hpp"
#include "space.hpp"
#include "tree.hpp"
#include <fstream>

#include <Eigen/Dense>
#include <memory>
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

  // Object creation
  // wall creation parameters
  // RGB wAmb(1.0, 1.0, 1.0);
  // RGB wDiff(1.0, 1.0, 1.0);
  // RGB wSpec(0, 0, 0);
  // RGB wRefl(0, 0, 0);
  // RGB wRefr(0, 0, 0);
  // auto tmp =
  //     std::make_unique<Halfspace>(Vector4D(0, 0, 0, 1), Vector4D(0, 0, 1, 0),
  //                                 wAmb, wDiff, wSpec, wRefl, wRefr, 1.0);

  // auto wall1 = std::make_unique<Object>(std::move(tmp));

  // // added specific parameters for metal material
  // RGB gAmb(0.2, 0.2, 0.2);
  // RGB gDiff(0.4, 0.4, 0.4);
  // RGB gSpec(0.95, 0.95, 0.95);
  // RGB gRefl(0.6, 0.6, 0.6);
  // RGB gRefr(0.0, 0.0, 0.0);

  // Material metalMat(gAmb, gDiff, gSpec, gRefl, gRefr, 50.0);

  screen.calculate(space);

  Render render(screen.width * screen.dpi, screen.height * screen.dpi);
  render.render(screen);
  render.show();
  return 0;
}
