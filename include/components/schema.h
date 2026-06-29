#pragma once
#include "components/link.h"
#include "components/machine.h"
#include "components/machineset.h"
#include "components/switch.h"
#include "nlohmann/json.hpp"
#include <map>
#include <string>
#include <vector>

struct Schema {
    unsigned    id        = 0;
    std::string name;
    unsigned    next_id   = 1;
    unsigned    output_id = 0;

    unsigned bandwidth  = 0;
    double   loadFactor = 0.0;
    double   latency    = 0.0;

    std::vector<MachineConf>    machines;
    std::vector<SwitchConf>     switches;
    std::vector<Schema>         schemas;
    std::vector<MachineSetConf> machineSets;
    std::vector<LinkConf>       links;

    unsigned addMachine(std::string_view nodeName = "");
    unsigned addSwitch(std::string_view nodeName = "");
    unsigned addMachineSet(std::string_view nodeName = "");
    unsigned addSchema(std::string_view nodeName = "");
    unsigned addLink(unsigned from_id, unsigned to_id, std::string_view linkName = "");

    MachineConf*    findMachine(unsigned id);
    SwitchConf*     findSwitch(unsigned id);
    MachineSetConf* findMachineSet(unsigned id);
    Schema*         findSchema(unsigned id);
    LinkConf*       findLink(unsigned id);

    void removeNode(unsigned id);
    void removeLink(unsigned id);
};

Schema clone_schema(const Schema& src, unsigned& next_id);

using json = nlohmann::json;
void to_json(json& j, const Schema& s);
