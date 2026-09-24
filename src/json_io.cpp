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
    return Vector4D(a[0], a[1], a[2], 0);
}
Vector4D parseDirection(const json& j) {
    auto a = j.get<std::array<double, 3>>();
    return Vector4D(a[0], a[1], a[2], 1);
}
RGB parseColor (const json& j) {
    auto a = j.get<std::array<double, 3>>();
    return RGB(a[0], a[1], a[2]);
}

Screen makeScreen(const json& j) {

    auto dim = j.at("screen").at("dimensions").get<std::array<int,2>>();
    int height = dim[0]; 
    int width = dim[1];
    double dpi = j.at("screen").at("dpi").get<double>();
    auto screenPos = parseDirection(j.at("screen").at("position"));    // PLSFIX: Previous version of this uses w=1, i.e. a direction, instead of point 
    auto observerPos = parseDirection(j.at("screen").at("observer"));

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


// Predeclaration so Recursion works
std::unique_ptr<Object> parseNode(const json& j) {};

std::unique_ptr<Object> parseSphere(const json& j){
    Vector4D pos = parsePoint(j.at("position"));
    double radius = j.at("radius").get<double>();
    Material mat = parseMaterial(j.at("color"));
    int index = j.at("index").get<int>();   // Template Json File contains some "index", no idea for what...
    auto sphere = std::make_unique<Sphere>(
                                    pos, radius, mat);
    return std::make_unique<Object>(std::move(sphere));
};

std::unique_ptr<Object> parseHalfspace(const json& j){
    Vector4D pos = parsePoint(j.at("position"));
    Vector4D normal = parseDirection(j.at("normal"));
    Material mat = parseMaterial(j.at("color"));
    int index = j.at("index").get<int>();
    auto hs = std::make_unique<Halfspace>(pos, normal, mat);
    return std::make_unique<Object>(std::move(hs));
};

// std::unique_ptr<Object> parseTranslation(const json& j){
//     Vector4D factors = parseDirection(j.at("factors"));
//     auto subject = parseNode(j.at("subjects"));
//     return std::make_unique<Object>(std::make_unique<Translation>(std::move(subject), factors));
// };

// std::unique_ptr<Object> parseRotation(const json& j){
//     Vector4D factors = parseDirection(j.at("factors"));
//     auto subject = parseNode(j.at("subjects"));
//     return std::make_unique<Object>(std::make_unique<Rotation>(std::move(subject), factors));
// };

// std::unique_ptr<Object> parseScaling(const json& j){
//     Vector4D factors = parseDirection(j.at("factors"));
//     auto subject = parseNode(j.at("subjects"));
//     return std::make_unique<Object>(std::make_unique<Rotation>(std::move(subject), factors));
// };

// std::unique_ptr<Object> parseUnion(const json& j);

// std::unique_ptr<Object> parseIntersection(const json& j);

// std::unique_ptr<Object> parseExclusion(const json& j);

// std::unique_ptr<Object> parseNode(const json& j) {

// };


// void makeObjects(const json& j, Space& space) {
//     auto objects = j.at("objects");
//     for (auto obj : objects) {
//         space.add(parseNode(obj));
//     }
// }


// // Expects json type of an element of object[]
// std::unique_ptr<PrimitiveObject> makeObject(const json& j) {
//     if (j.contains("halfSpace")) {
//         return std::make_unique<Halfspace>(
//             j.at("halfspace").at("position").get<Vector4D>(),
//             j.at("halfspace").at("normal").get<Vector4D>().normalized());
//     }
//     // if (j.contains("sphere")) {
//     //     return std::make_unique<Sphere>(
//     //         j.at("sphere").at("position").get<Vector4D>(),
//     //         j.at("sphere").at("radius").get<double>());
//     // }
//     throw std::runtime_error("Unknown or unsupported object entry: " + j.dump());
// }
