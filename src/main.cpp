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

std::unique_ptr<Tree> modify(std::unique_ptr<Tree> toMod, Vector4D translation,
                             Vector4D scaling, Vector4D rotation) {
  auto scal = std::make_unique<Scaling>(std::move(toMod), scaling);
  auto rot = std::make_unique<Rotation>(std::move(scal), rotation);
  auto transl = std::make_unique<Translation>(std::move(rot), translation);

  return transl;
}

std::unique_ptr<Intersection> makeCube(Vector4D start, double x, double y,
                                       double z, RGB color) {

  // added specific parameters for glass material
  RGB amb = color * 0.5;
  RGB diff = color * 0.6;
  RGB spec = RGB(1.0, 0.8, 0.8);
  RGB refl = RGB(0.0, 0.0, 0.0);
  RGB refr = color * 0.8;
  double shininess = 2.0;

  auto tmp = std::make_unique<Halfspace>(start, Vector4D(-1, 0, 0, 0), amb,
                                         diff, spec, refl, refr, shininess);
  auto tmp2 = std::make_unique<Halfspace>(start, Vector4D(0, -1, 0, 0), amb,
                                          diff, spec, refl, refr, shininess);
  auto tmp3 = std::make_unique<Halfspace>(start, Vector4D(0, 0, -1, 0), amb,
                                          diff, spec, refl, refr, shininess);

  auto tmp4 = std::make_unique<Halfspace>(start + Vector4D(x, 0, 0, 0),
                                          Vector4D(1, 0, 0, 0), amb, diff, spec,
                                          refl, refr, shininess);
  auto tmp5 = std::make_unique<Halfspace>(start + Vector4D(0, y, 0, 0),
                                          Vector4D(0, 1, 0, 0), amb, diff, spec,
                                          refl, refr, shininess);
  auto tmp6 = std::make_unique<Halfspace>(start + Vector4D(0, 0, z, 0),
                                          Vector4D(0, 0, 1, 0), amb, diff, spec,
                                          refl, refr, shininess);

  auto tmpCube = std::make_unique<Intersection>();
  tmpCube->add(std::move(tmp));
  tmpCube->add(std::move(tmp2));
  tmpCube->add(std::move(tmp3));
  tmpCube->add(std::move(tmp4));
  tmpCube->add(std::move(tmp5));
  tmpCube->add(std::move(tmp6));

  return tmpCube;
}

int main() {

  string json_file = "json/input1.json";
  SceneLoader loader(json_file);
  json scene = loader.load();

  Screen screen = makeScreen(scene);
  Space space = makeSpace(scene);
  makeLights(scene, space); // reads json parameters & add them to the space

  // Object creation
  // wall creation parameters
  RGB wAmb(1.0, 1.0, 1.0);
  RGB wDiff(1.0, 1.0, 1.0);
  RGB wSpec(0, 0, 0);
  RGB wRefl(0, 0, 0);
  RGB wRefr(0, 0, 0);
  auto tmp =
      std::make_unique<Halfspace>(Vector4D(0, 0, 0, 1), Vector4D(0, 0, 1, 0),
                                  wAmb, wDiff, wSpec, wRefl, wRefr, 1.0);

  auto wall1 = std::make_unique<Object>(std::move(tmp));

  // added specific parameters for metal material
  RGB gAmb(0.2, 0.2, 0.2);
  RGB gDiff(0.4, 0.4, 0.4);
  RGB gSpec(0.95, 0.95, 0.95);
  RGB gRefl(0.6, 0.6, 0.6);
  RGB gRefr(0.0, 0.0, 0.0);


  Material metalMat(gAmb, gDiff, gSpec, gRefl, gRefr, 50.0);
  auto tmp2 = std::make_unique<Sphere>(Vector4D(2, 2, 2, 1), 1, metalMat);

  auto hold = modify(std::move(tmp2), Vector4D(2, 5, 1, 1),
                     Vector4D(2, 3, 1, 1), Vector4D(1, 1, 2, 1));

  auto sphere1 = std::make_unique<Object>(std::move(hold));

  auto cube = makeCube(Vector4D(0, 0, 0, 1), 2, 2, 1, RGB(1.0, 0.5, 0.5));
  hold = modify(std::move(cube), Vector4D(3.4, 2, 1, 1), Vector4D(1, 3, 1, 1),
                Vector4D(1, 0, 0, 1));

  auto cubeObj = std::make_unique<Object>(std::move(hold));

  space.add(std::move(wall1));
  // space.add(std::move(cubeObj));
  space.add(std::move(sphere1));

  screen.calculate(space);

  Render render(screen.width * screen.dpi, screen.height * screen.dpi);
  render.render(screen);
  render.show();
  return 0;
}
