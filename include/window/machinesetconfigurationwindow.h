#pragma once
#include "components/machineset.h"
#include <QDialog>

namespace Ui {
class MachineSetConfigurationWindow;
}

class MachineSetConfigurationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MachineSetConfigurationWindow(MachineSetConf* conf, QWidget* parent = nullptr);

public slots:
    void setName(const QString& newName);
    void setBandwidth(int newBandwidth);
    void setLoadFactor(double newLoadFactor);
    void setLatency(double newLatency);

private:
    Ui::MachineSetConfigurationWindow* ui;
    MachineSetConf*                    conf;
};
