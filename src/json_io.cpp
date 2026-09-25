#include "helper.hpp"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <screen.hpp>
#include <json_io.hpp>

using std::string, std::cout, std::endl;
using json = nlohmann::json;

std::unique_ptr<Intersection> makeCube(Vector4D start, double x, double y,
                                       double z, const Material &mat) {


  auto tmp = std::make_unique<Halfspace>(start, Vector4D(-1, 0, 0, 0), mat);
  auto tmp2 = std::make_unique<Halfspace>(start, Vector4D(0, -1, 0, 0), mat);
  auto tmp3 = std::make_unique<Halfspace>(start, Vector4D(0, 0, -1, 0), mat);

  auto tmp4 = std::make_unique<Halfspace>(start + Vector4D(x, 0, 0, 0),
                                          Vector4D(1, 0, 0, 0), mat);
  auto tmp5 = std::make_unique<Halfspace>(start + Vector4D(0, y, 0, 0),
                                          Vector4D(0, 1, 0, 0), mat);
  auto tmp6 = std::make_unique<Halfspace>(start + Vector4D(0, 0, z, 0),
                                          Vector4D(0, 0, 1, 0), mat);
 
  auto tmpCube = std::make_unique<Intersection>();
  tmpCube->add(std::move(tmp));
  tmpCube->add(std::move(tmp2));
  tmpCube->add(std::move(tmp3));
  tmpCube->add(std::move(tmp4));
  tmpCube->add(std::move(tmp5));
  tmpCube->add(std::move(tmp6));

  return tmpCube;
}

std::unique_ptr<Tree> modify(std::unique_ptr<Tree> toMod, Vector4D translation,
                             Vector4D scaling, Vector4D rotation) {
  auto scal = std::make_unique<Scaling>(std::move(toMod), scaling);
  auto rot = std::make_unique<Rotation>(std::move(scal), rotation);
  auto transl = std::make_unique<Translation>(std::move(rot), translation);

  return transl;
}

Vector4D parsePoint(const json& j) {
    auto a = j.get<std::array<double, 3>>(); 
    return Vector4D(a[0], a[1], a[2], 1);
}
Vector4D parseDirection(const json& j) {
    auto a = j.get<std::array<double, 3>>();
    return Vector4D(a[0], a[1], a[2], 0);
}
RGB parseColor (const json& j) {
    auto a = j.get<std::array<double, 3>>();
    return RGB(a[0], a[1], a[2]);
}

Material parseMaterial(const json& j) {
    Material mat;
    mat.ambient   = parseColor(j.at("ambient"));
    mat.diffuse   = parseColor(j.at("diffuse"));
    mat.specular  = parseColor(j.at("specular"));
    mat.reflected = parseColor(j.at("reflected"));
    mat.refracted = parseColor(j.at("refracted"));
    mat.shininess = j.at("shininess").get<double>();
    return mat;
};

Screen makeScreen(const json& j) {

    auto dim = j.at("screen").at("dimensions").get<std::array<int,2>>();
    int height = dim[0]; 
    int width = dim[1];
    double dpi = j.at("screen").at("dpi").get<double>();
    auto screenPos = parsePoint(j.at("screen").at("position")); 
    auto observerPos = parsePoint(j.at("screen").at("observer"));

    std::cout << "Window settings: \n" << "h: " << height << ", w: " << width << "\ndpi: " << dpi << std::endl;
    return Screen(height, width, dpi, observerPos, screenPos);
}

Space makeSpace(const json& j) {
    RGB ambient = parseColor(j.at("medium").at("ambient"));
    int index = j.at("medium").at("index").get<double>();
    int recursion = j.at("medium").at("recursion").get<int>();
    return Space(ambient, index, recursion);
}

void makeLights(const json& j, Space& space) {
    auto sources = j.at("sources");
    for (auto s : sources) {
        Vector4D pos  = parsePoint(s.at("position"));
        RGB intensity = parseColor(s.at("intensity")); 
        space.add(std::make_unique<LightSource>(pos, intensity));
    }
    std::cout << sources.size() << " light sources added." << std::endl; 
}


// Predeclaration so Recursion works
std::unique_ptr<Tree> parseNode(const json& j);

std::unique_ptr<Tree> parseSphere(const json& j){
    Vector4D pos = parsePoint(j.at("position"));
    double radius = j.at("radius").get<double>();
    Material mat = parseMaterial(j.at("color"));
    int index = j.at("index").get<int>(); 
    (void) index;
    return std::make_unique<Sphere>(pos, radius, mat);
};

std::unique_ptr<Tree> parseHalfspace(const json& j){
    Vector4D pos = parsePoint(j.at("position"));
    Vector4D normal = parseDirection(j.at("normal"));
    Material mat = parseMaterial(j.at("color"));
    int index = j.at("index").get<int>();
    (void) index;
    return std::make_unique<Halfspace>(pos, normal, mat);
};

std::unique_ptr<Tree> parseCube(const json& j){
  Vector4D pos = parsePoint(j.at("position"));
  Vector4D dimensions = parsePoint(j.at("dimensions")); 
  Material mat = parseMaterial(j.at("color")); 
  int index = j.at("index").get<int>();
  (void) index; 
  
  return makeCube(pos, dimensions(0), dimensions(1), dimensions(2), mat);
}

template<typename TransformationTree>
std::unique_ptr<Tree> parseTransform(const json& j){
    Vector4D factors = parseDirection(j.at("factors"));
    auto subject = parseNode(j.at("subject"));
    return std::make_unique<TransformationTree>(std::move(subject), factors);
}
std::unique_ptr<Tree> parseRotation(const json &j){
    double angle = j.at("angle").get<double>(); 
    // umwandlung in bogenmass
    double pi = 2 * acos(0.0); 
    angle = angle * (pi / 180); 

    auto subject = parseNode(j.at("subject"));
    int direction = j.at("direction").get<int>(); 
    Vector4D rotationVec(0, 0, 0, 1);

    if(direction == 3) rotationVec(2) = angle; 
    else rotationVec(direction) = angle;    
    
    return std::make_unique<Rotation>(std::move(subject), rotationVec);
}

/**
* @brief parses basic modify for compacter transformatons 
*/
std::unique_ptr<Tree> parseModify(const json& j){
  Vector4D scal = parseDirection(j.at("scaling")); 
  Vector4D rot = parseDirection(j.at("rotation")); 
  
  // umwandlung in bogenmass
  double pi = 2 * acos(0.0);
  for(int i = 0; i<3; i++){
    rot(i) = rot(i) * (pi / 180);
  }


  Vector4D transl = parseDirection(j.at("translation")); 

  auto subject = parseNode(j.at("subject")); 
  return modify(std::move(subject), transl, scal, rot);
}

template<typename LogicalTree>
std::unique_ptr<Tree> parseLogical(const json& j){
    auto tree = std::make_unique<LogicalTree>();
    for (auto& node : j){
        tree->add(parseNode(node));
    }
    return tree;
}


std::unique_ptr<Tree> parseNode(const json& j) {
    if (j.contains("cube"))             return parseCube(j.at("cube"));
    if (j.contains("sphere"))           return parseSphere(j.at("sphere"));
    if (j.contains("halfSpace"))        return parseHalfspace(j.at("halfSpace"));
    if (j.contains("modify"))           return parseModify(j.at("modify"));
    if (j.contains("translation"))      return parseTransform<Translation>(j.at("translation"));
    if (j.contains("rotation"))         return parseRotation(j.at("rotation"));
    if (j.contains("scaling"))          return parseTransform<Scaling>(j.at("scaling"));
    if (j.contains("union"))            return parseLogical<Union>(j.at("union"));
    if (j.contains("intersection"))     return parseLogical<Intersection>(j.at("intersection"));
    if (j.contains("exclusion"))        return parseLogical<Exclusion>(j.at("exclusion"));
    
    throw std::runtime_error("Unknown object node in JSON scene: " + j.dump(4));
};


void makeObjects(const json& j, Space& space) {
    auto objects = j.at("objects");
    for (auto obj : objects) {
        space.add(std::make_unique<Object>(parseNode(obj)));
    }
}
