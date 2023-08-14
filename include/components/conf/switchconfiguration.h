#pragma once
#include "components/conf/itemconfiguration.h"

class SwitchConfiguration : public ItemConfiguration
{
public:
    SwitchConfiguration(unsigned id, std::string name);

    unsigned    getId() override;
    std::string getName() override;

    void        setId(unsigned id) override;
    void        setName(std::string name) override;

private:
    unsigned id = 0;
    std::string name = "";
};
