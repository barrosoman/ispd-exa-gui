#include "components/switch.h"

void to_json(json& j, const SwitchConf& s)
{
    j = json{
        {"id",        s.id},
        {"name",      s.name},
        {"bandwidth", s.bandwidth},
        {"load",      s.loadFactor},
        {"latency",   s.latency},
    };
}
