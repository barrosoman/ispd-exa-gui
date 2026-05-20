#pragma once
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

struct MachineConf {
    unsigned id                          = 0;
    std::string name;
    double computationalPower            = 1.0;
    double loadFactor                    = 0.1;
    double ram                           = 1.0;
    double hardDisk                      = 1.0;
    double energyConsumption             = 1.0;
    double costPerProcessing             = 1.0;
    double costPerMemory                 = 1.0;
    double costPerDisk                   = 1.0;
    int    coreCount                     = 1;
    bool   master                        = false;
    std::string owner;
    std::string schedulingAlgorithm;
    double      gpuPower                 = 1.0;
    int         gpuCoreCount             = 1;
    double      gpuInterconnectionBandwidth = 1.0;
    double      wattageIdle              = 1.0;
    double      wattageMax               = 1.0;
    std::string scheduler                = "RoundRobin";
    std::vector<unsigned> slaves;
};

using json = nlohmann::json;
void to_json(json& j, const MachineConf& m);
void from_json(const json& j, MachineConf& m);
