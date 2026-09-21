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


// Screen makeScreen(const json& j) {
//     const auto& s = j.at("screen"); 
//     auto dims = s.at("dimensions");
//     return Screen(
//                 dims.at(0).get<int>(), 
//                 dims.at(1).get<int>(), 
//                 s.at("dpi").get<int>(),
//                 parsePoint(s.at("observer")),
//                 parsePoint(s.at("position")
//                 );
// }

// // Expects json type of a light source
// std::unique_ptr<LightSource> makeLight(const json& j) {
//     if (j.)

//     return std::make_unique<LightSource>(
//         parsePosition(j.at("position")),
//         parseColor(j.at("intensity"));
//     ); 
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



// makeScreen, makeLight, makeObject, etc.