#pragma once

#include <helper.hpp>
#include <screen.hpp>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SceneLoader {
private:
    std::string filename_;
    std::ifstream openWithFallback(const std::string& filename) const {
        std::ifstream file(filename);
        if (file.is_open()) return file;

        file.clear();
        file.open("../" + filename);
        if (file.is_open()) return file;

        throw std::runtime_error("Could not open scene json file \"" + filename + "\"");
    }
public:
    SceneLoader(std::string filename) : filename_(filename) {}

    json load() {
        std::ifstream file = openWithFallback(filename_);
        return json::parse(file);
    };
};


Vector4D parsePoint(const json& j);
Vector4D parseDirection(const json& j);
RGB parseColor (const json& j);
Material parseMaterial(const json& j);

Screen makeScreen(const json& j);
Space makeSpace(const json& j);
void makeLights(const json& j, Space& space);


std::unique_ptr<Object> parseNode(const json& j);
std::unique_ptr<Object> parseSphere(const json& j);
std::unique_ptr<Object> parseHalfspace(const json& j);

std::unique_ptr<Object> parseTranslation(const json& j);
std::unique_ptr<Object> parseRotation(const json& j);
std::unique_ptr<Object> parseScaling(const json& j);

std::unique_ptr<Object> parseUnion(const json& j);
std::unique_ptr<Object> parseIntersection(const json& j);
std::unique_ptr<Object> parseExclusion(const json& j);

