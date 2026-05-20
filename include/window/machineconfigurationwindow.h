#pragma once
#include "components/machine.h"
#include <QDialog>

namespace Ui {
class MachineConfigurationWindow;
}

class MachineConfigurationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MachineConfigurationWindow(MachineConf* conf, QWidget* parent = nullptr);

public slots:
    void setName(const QString& newName);
    void setCoreCount(int coreCount);
    void setComputationPower(int computationPower);
    void setLoadFactor(double loadFactor);
    void setGpuCoreCount(int coreCount);
    void setGpuPower(int gpuPower);
    void setGpuBandwidth(int gpuBandwidth);
    void setRam(int ram);
    void setDisk(int disk);
    void setWattageIdle(int wattageIdle);
    void setWattageMax(int wattageMax);
    void checkMaster(int checked);
    void setScheduler(const QString& scheduler);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MachineConfigurationWindow* ui;
    MachineConf* conf;
    void setupConfAndWindow();
};
