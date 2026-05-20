#include "window/machinesetconfigurationwindow.h"
#include "ui_machinesetconfigurationwindow.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

MachineSetConfigurationWindow::MachineSetConfigurationWindow(MachineSetConf* conf, QWidget* parent)
    : QDialog(parent), ui(new Ui::MachineSetConfigurationWindow), conf(conf)
{
    this->ui->setupUi(this);
    this->ui->nameEditLine->setText(conf->name.c_str());
}

void MachineSetConfigurationWindow::setName(const QString& newName) { conf->name      = newName.toStdString(); }
void MachineSetConfigurationWindow::setBandwidth(int v)             { conf->bandwidth  = v; }
void MachineSetConfigurationWindow::setLoadFactor(double v)         { conf->loadFactor = v; }
void MachineSetConfigurationWindow::setLatency(double v)            { conf->latency    = v; }
