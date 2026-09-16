#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <screen.hpp>
#include <json_io.hpp>

using std::string, std::cout, std::endl;
using json = nlohmann::json;

Screen makeScreen(const json& j) {
    const auto& s = j.at("screen"); 
    auto dims = s.at("dimensions");
    return Screen(
                dims.at(0).get<int>(), 
                dims.at(1).get<int>(), 
                s.at("dpi").get<int>(),
                s.at("observer").get<Vector4D>(),
                s.at("position").get<Vector4D>()
                );
}

// Expects json type of a light source
std::unique_ptr<LightSource> makeLight(const json& j) {
    return std::make_unique<LightSource>(
        j.at("position").get<Vector4D>(),
        j.at("intensity").get<RGB>()
    ); 
}

// Expects json type of an element of object[]
std::unique_ptr<PrimitiveObject> makeObject(const json& j) {
    if (j.contains("halfSpace")) {
        return std::make_unique<Halfspace>(
            j.at("halfspace").at("position").get<Vector4D>(),
            j.at("halfspace").at("normal").get<Vector4D>().normalized());
    }
    // if (j.contains("sphere")) {
    //     return std::make_unique<Sphere>(
    //         j.at("sphere").at("position").get<Vector4D>(),
    //         j.at("sphere").at("radius").get<double>());
    // }
    throw std::runtime_error("Unknown or unsupported object entry: " + j.dump());
}



// makeScreen, makeLight, makeObject, etc.