#include "components/link.h"

void to_json(json& j, const LinkConf& l)
{
    j = json{
        {"id",        l.id},
        {"name",      l.name},
        {"from",      l.from_id},
        {"to",        l.to_id},
        {"bandwidth", l.bandwidth},
        {"load",      l.loadFactor},
        {"latency",   l.latency},
    };
}

void from_json(const json& j, LinkConf& l)
{
    j.at("id").get_to(l.id);
    j.at("name").get_to(l.name);
    j.at("from").get_to(l.from_id);
    j.at("to").get_to(l.to_id);
}
