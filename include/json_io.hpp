#pragma once

#include <helper.hpp>
#include <memory>
#include <screen.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

using std::pmr::unordered_map, std::string;

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

class SzeneContext{
public:
  unordered_map<string, Material> materials;
  const Material& mat(string name){
    return this->materials[name];
  }
  void add(string name, Material mat){
    this->materials[name] = mat; 
  }
}; 

Vector4D parsePoint(const json& j);
Vector4D parseDirection(const json& j);
RGB parseColor (const json& j);
Material parseMaterial(const json& j);

Screen makeScreen(const json& j);
Space makeSpace(const json& j);
void makeLights(const json& j, Space& space);
void makeObjects(const json&j, Space& space);


std::unique_ptr<Tree> parseNode(const json& j);
std::unique_ptr<Tree> parseSphere(const json& j);
std::unique_ptr<Tree> parseHalfspace(const json& j);
std::unique_ptr<Tree> parseCube(const json& j);


std::unique_ptr<Tree> parseModify(const json& j);
std::unique_ptr<Tree> parseTranslation(const json& j);
std::unique_ptr<Tree> parseRotation(const json& j);
std::unique_ptr<Tree> parseScaling(const json& j);

std::unique_ptr<Tree> parseTransform(const json& j);
std::unique_ptr<Tree> parseLogical(const json& j);

