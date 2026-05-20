#pragma once
#include "nlohmann/json.hpp"
#include <string>

struct SwitchConf {
    unsigned    id         = 0;
    std::string name;
    unsigned    bandwidth  = 0;
    double      loadFactor = 0.0;
    double      latency    = 0.0;
};

using json = nlohmann::json;
void to_json(json& j, const SwitchConf& s);
