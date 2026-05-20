#include "components/machineset.h"

void to_json(json& j, const MachineSetConf& ms)
{
    j = json{
        {"id",        ms.id},
        {"name",      ms.name},
        {"bandwidth", ms.bandwidth},
        {"load",      ms.loadFactor},
        {"latency",   ms.latency},
    };
}
