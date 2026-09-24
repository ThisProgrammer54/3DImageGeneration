#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <screen.hpp>
#include <json_io.hpp>

using std::string, std::cout, std::endl;
using json = nlohmann::json;

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
    int index = j.at("index").get<int>();   // Template Json File contains some "index", no idea for what...
    return std::make_unique<Sphere>(pos, radius, mat);
};

std::unique_ptr<Tree> parseHalfspace(const json& j){
    Vector4D pos = parsePoint(j.at("position"));
    Vector4D normal = parseDirection(j.at("normal"));
    Material mat = parseMaterial(j.at("color"));
    int index = j.at("index").get<int>();
    return std::make_unique<Halfspace>(pos, normal, mat);
};

template<typename TransformationTree>
std::unique_ptr<Tree> parseTransform(const json& j){
    Vector4D factors = parseDirection(j.at("factors"));
    auto subject = parseNode(j.at("subject"));
    return std::make_unique<TransformationTree>(std::move(subject), factors);
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
    if (j.contains("sphere"))           return parseSphere(j.at("sphere"));
    if (j.contains("halfSpace"))        return parseHalfspace(j.at("halfSpace"));
    if (j.contains("translation"))      return parseTransform<Translation>(j.at("translation"));
    if (j.contains("rotation"))         return parseTransform<Rotation>(j.at("rotation"));
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