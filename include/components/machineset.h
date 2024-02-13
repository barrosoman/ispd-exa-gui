#include "components/connectable.h"
#include "window/machinesetconfigurationwindow.h"
#include <memory>

class MachineSetConfiguration;
class Schema;

class MachineSet : public Connectable
{
public:
    MachineSet(Schema *schema, MachineSetConfiguration *conf);
    ~MachineSet() override;
    std::vector<std::shared_ptr<Link>> *getConnectedLinks() override;
    void                                setConnectedLinks(
                                       std::vector<std::shared_ptr<Link>> *linksVector) override;
    void        removeConnectedLink(Link *link) override;
    void        addConnectedLink(std::shared_ptr<Link> link) override;
    PixmapIcon *getIcon() override;
    unsigned    getId() const override;
    void        setId(unsigned newId) override;
    std::unique_ptr<std::vector<std::string>> print() override;
    std::unique_ptr<ConnectableCloner> cloner(SchemaCloner *parent) override;
    void                               showConfiguration() override;
    ItemConfiguration                 *getConf() override;

private:
    Schema                                      *schema;
    unsigned                                     id;
    std::unique_ptr<MachineSetConfigurationWindow>      window;
    std::unique_ptr<MachineSetConfiguration> conf;
    std::unique_ptr<PixmapIcon>                  icon;
    std::vector<std::shared_ptr<Link>>           connectedLinks;
};
