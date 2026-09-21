#pragma once

#include <helper.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Vector4D parsePoint(const json& j);
Vector4D parseDirection(const json& j);
RGB parseColor (const json& j);
