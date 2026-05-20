#pragma once
#include "nlohmann/json.hpp"
#include <string>

struct LinkConf {
    unsigned    id         = 0;
    std::string name;
    unsigned    from_id    = 0;
    unsigned    to_id      = 0;
    unsigned    bandwidth  = 0;
    double      loadFactor = 0.0;
    double      latency    = 0.0;
};

using json = nlohmann::json;
void to_json(json& j, const LinkConf& l);
void from_json(const json& j, LinkConf& l);
