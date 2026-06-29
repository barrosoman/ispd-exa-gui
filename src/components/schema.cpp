#include "components/schema.h"
#include <algorithm>

static std::string defaultName(std::string_view prefix, std::string_view given, unsigned id)
{
    return given.empty() ? std::string(prefix) + std::to_string(id) : std::string(given);
}

unsigned Schema::addMachine(std::string_view nodeName)
{
    unsigned new_id = next_id++;
    machines.push_back({.id = new_id, .name = defaultName("Machine", nodeName, new_id)});
    return new_id;
}

unsigned Schema::addSwitch(std::string_view nodeName)
{
    unsigned new_id = next_id++;
    switches.push_back({.id = new_id, .name = defaultName("Switch", nodeName, new_id)});
    return new_id;
}

unsigned Schema::addSchema(std::string_view nodeName)
{
    unsigned new_id = next_id++;
    Schema sub;
    sub.id   = new_id;
    sub.name = defaultName("Schema", nodeName, new_id);

    unsigned sw_id = sub.next_id++;
    sub.switches.push_back({.id = sw_id, .name = "Output"});
    sub.output_id = sw_id;

    schemas.push_back(std::move(sub));
    return new_id;
}

unsigned Schema::addMachineSet(std::string_view nodeName)
{
    unsigned new_id = next_id++;
    machineSets.push_back({.id = new_id, .name = defaultName("Set", nodeName, new_id)});
    return new_id;
}

unsigned Schema::addLink(unsigned from_id, unsigned to_id, std::string_view linkName)
{
    unsigned new_id = next_id++;
    links.push_back({
        .id      = new_id,
        .name    = defaultName("Link", linkName, new_id),
        .from_id = from_id,
        .to_id   = to_id,
    });
    return new_id;
}

MachineConf* Schema::findMachine(unsigned id)
{
    for (auto& m : machines)
        if (m.id == id) return &m;
    return nullptr;
}

SwitchConf* Schema::findSwitch(unsigned id)
{
    for (auto& s : switches)
        if (s.id == id) return &s;
    return nullptr;
}

MachineSetConf* Schema::findMachineSet(unsigned id)
{
    for (auto& ms : machineSets)
        if (ms.id == id) return &ms;
    return nullptr;
}

Schema* Schema::findSchema(unsigned id)
{
    for (auto& s : schemas)
        if (s.id == id) return &s;
    return nullptr;
}

LinkConf* Schema::findLink(unsigned id)
{
    for (auto& l : links)
        if (l.id == id) return &l;
    return nullptr;
}

void Schema::removeNode(unsigned id)
{
    unsigned output_id = 0;
    for (auto& s : schemas)
        if (s.id == id) { output_id = s.output_id; break; }

    std::erase_if(machines,    [id](auto& m)  { return m.id == id; });
    std::erase_if(switches,    [id](auto& s)  { return s.id == id; });
    std::erase_if(machineSets, [id](auto& ms) { return ms.id == id; });
    std::erase_if(schemas,     [id](auto& s)  { return s.id == id; });
    std::erase_if(links, [id, output_id](auto& l) {
        return l.from_id == id  || l.to_id == id ||
              (output_id && (l.from_id == output_id || l.to_id == output_id));
    });
}

void Schema::removeLink(unsigned id)
{
    std::erase_if(links, [id](auto& l) { return l.id == id; });
}

Schema clone_schema(const Schema& src, unsigned& next_id)
{
    Schema result;
    result.id   = next_id++;
    result.name = src.name;

    std::map<unsigned, unsigned> id_map;

    auto remap = [&](unsigned old_id) -> unsigned {
        unsigned new_id    = next_id++;
        id_map[old_id]     = new_id;
        return new_id;
    };

    for (auto m : src.machines) {
        m.id = remap(m.id);
        result.machines.push_back(m);
    }
    for (auto s : src.switches) {
        s.id = remap(s.id);
        result.switches.push_back(s);
    }
    for (auto ms : src.machineSets) {
        ms.id = remap(ms.id);
        result.machineSets.push_back(ms);
    }
    for (const auto& sub : src.schemas) {
        Schema cloned = clone_schema(sub, next_id);
        id_map[sub.id] = cloned.id;
        result.schemas.push_back(std::move(cloned));
    }
    for (auto l : src.links) {
        l.id      = next_id++;
        l.from_id = id_map[l.from_id];
        l.to_id   = id_map[l.to_id];
        result.links.push_back(l);
    }

    result.output_id = id_map.count(src.output_id) ? id_map[src.output_id] : 0;
    return result;
}

void to_json(json& j, const Schema& s)
{
    j["users"]     = json::array();
    j["workloads"] = json::array();
    j["services"]  = {
        {"machines", json::array()},
        {"masters",  json::array()},
        {"links",    json::array()},
        {"switches", json::array()},
    };

    for (const auto& m : s.machines) {
        if (m.master)
            j["services"]["masters"].push_back(m);
        else
            j["services"]["machines"].push_back(m);
    }
    for (const auto& sw : s.switches)
        j["services"]["switches"].push_back(sw);
    for (const auto& l : s.links)
        j["services"]["links"].push_back(l);
}
