#include "components/machine.h"

void to_json(json& j, const MachineConf& m)
{
    if (m.master) {
        j = json{
            {"id",        m.id},
            {"name",      m.name},
            {"scheduler", m.scheduler},
            {"slaves",    m.slaves},
        };
    } else {
        j = json{
            {"id",                             m.id},
            {"name",                           m.name},
            {"power",                          m.computationalPower},
            {"load",                           m.loadFactor},
            {"core_count",                     m.coreCount},
            {"gpu_power",                      m.gpuPower},
            {"gpu_core_count",                 m.gpuCoreCount},
            {"gpu_interconnection_bandwidth",  m.gpuInterconnectionBandwidth},
            {"wattage_idle",                   m.wattageIdle},
            {"wattage_max",                    m.wattageMax},
        };
    }
}

void from_json(const json& j, MachineConf& m)
{
    j.at("id").get_to(m.id);
    j.at("name").get_to(m.name);
}
