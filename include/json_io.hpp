#pragma once

#include <helper.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace nlohmann {
    template <>
    struct adl_serializer<Eigen::Vector<double, 4>> {
        static void from_json(const json& j, Eigen::Vector<double, 4>& v) {
            v = Eigen::Vector<double, 4>(
                j.at(0).get<double>(), 
                j.at(1).get<double>(),
                j.at(2).get<double>(), 
                1.0
            );
        }
    };
}

inline void from_json(const json& j, RGB& c){
    c = RGB(j.at(0).get<double>(), j.at(1).get<double>(), j.at(2).get<double>());
}

