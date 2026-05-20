#include "window/switchconfigurationwindow.h"
#include "ui_switchconfigurationwindow.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

SwitchConfigurationWindow::SwitchConfigurationWindow(SwitchConf* conf, QWidget* parent)
    : QDialog(parent), ui(new Ui::SwitchConfigurationWindow), conf(conf)
{
    this->ui->setupUi(this);
    this->setupConfAndWindow();
}

void SwitchConfigurationWindow::setupConfAndWindow()
{
    this->ui->nameEditLine->setText(conf->name.c_str());
    this->ui->loadFactorSpinBox->setValue(conf->loadFactor);
    this->ui->latencySpinBox->setValue(conf->latency);
    this->ui->bandwidthSpinBox->setValue(conf->bandwidth);

    connect(this->ui->nameEditLine,      &QLineEdit::textChanged,
            this, &SwitchConfigurationWindow::setName);
    connect(this->ui->bandwidthSpinBox,  qOverload<int>(&QSpinBox::valueChanged),
            this, &SwitchConfigurationWindow::setBandwidth);
    connect(this->ui->latencySpinBox,    qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &SwitchConfigurationWindow::setLatency);
    connect(this->ui->loadFactorSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &SwitchConfigurationWindow::setLoadFactor);
}

void SwitchConfigurationWindow::setName(const QString& newName) { conf->name      = newName.toStdString(); }
void SwitchConfigurationWindow::setBandwidth(int v)             { conf->bandwidth  = v; }
void SwitchConfigurationWindow::setLoadFactor(double v)         { conf->loadFactor = v; }
void SwitchConfigurationWindow::setLatency(double v)            { conf->latency    = v; }
