#include "window/machineconfigurationwindow.h"
#include "ui_machineconfigurationwindow.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

MachineConfigurationWindow::MachineConfigurationWindow(MachineConf* conf, QWidget* parent)
    : QDialog(parent), ui(new Ui::MachineConfigurationWindow), conf(conf)
{
    this->ui->setupUi(this);
    this->setupConfAndWindow();
}

void MachineConfigurationWindow::setupConfAndWindow()
{
    this->setFixedHeight(700);
    this->ui->nameEditLine->setText(this->conf->name.c_str());
    this->ui->coresSpinBox->setValue(this->conf->coreCount);
    this->ui->computationPowerSpinBox->setValue(this->conf->computationalPower);
    this->ui->loadFactorSpinBox->setValue(this->conf->loadFactor);
    this->ui->gpuCoresSpinBox->setValue(this->conf->gpuCoreCount);
    this->ui->gpuPower->setValue(this->conf->gpuPower);
    this->ui->gpuBandwidth->setValue(this->conf->gpuInterconnectionBandwidth);
    this->ui->ramSpinBox->setValue(this->conf->ram);
    this->ui->diskSpinBox->setValue(this->conf->hardDisk);
    this->ui->energyMaxSpinBox->setValue(this->conf->wattageMax);
    this->ui->energyIdleSpinBox->setValue(this->conf->wattageIdle);
    this->ui->masterCheckBox->setChecked(this->conf->master);
    this->ui->schedulersComboBox->setCurrentText(this->conf->scheduler.c_str());

    connect(this->ui->nameEditLine,            &QLineEdit::textChanged,
            this, &MachineConfigurationWindow::setName);
    connect(this->ui->coresSpinBox,            qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setCoreCount);
    connect(this->ui->computationPowerSpinBox, qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setComputationPower);
    connect(this->ui->loadFactorSpinBox,       qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setLoadFactor);
    connect(this->ui->gpuCoresSpinBox,         qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setGpuCoreCount);
    connect(this->ui->gpuPower,                qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setGpuPower);
    connect(this->ui->gpuBandwidth,            qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setGpuBandwidth);
    connect(this->ui->ramSpinBox,              qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setRam);
    connect(this->ui->diskSpinBox,             qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setDisk);
    connect(this->ui->masterCheckBox,          &QCheckBox::stateChanged,
            this, &MachineConfigurationWindow::checkMaster);
    connect(this->ui->energyIdleSpinBox,       qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setWattageIdle);
    connect(this->ui->energyMaxSpinBox,        qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setWattageMax);
    connect(this->ui->schedulersComboBox,      &QComboBox::currentTextChanged,
            this, &MachineConfigurationWindow::setScheduler);
}

void MachineConfigurationWindow::setName(const QString& newName)     { conf->name = newName.toStdString(); }
void MachineConfigurationWindow::setCoreCount(int v)                  { conf->coreCount = v; }
void MachineConfigurationWindow::setComputationPower(int v)           { conf->computationalPower = v; }
void MachineConfigurationWindow::setLoadFactor(double v)              { conf->loadFactor = v; }
void MachineConfigurationWindow::setGpuCoreCount(int v)              { conf->gpuCoreCount = v; }
void MachineConfigurationWindow::setGpuPower(int v)                   { conf->gpuPower = v; }
void MachineConfigurationWindow::setGpuBandwidth(int v)               { conf->gpuInterconnectionBandwidth = v; }
void MachineConfigurationWindow::setRam(int v)                        { conf->ram = v; }
void MachineConfigurationWindow::setDisk(int v)                       { conf->hardDisk = v; }
void MachineConfigurationWindow::setWattageIdle(int v)                { conf->wattageIdle = v; }
void MachineConfigurationWindow::setWattageMax(int v)                 { conf->wattageMax = v; }
void MachineConfigurationWindow::setScheduler(const QString& v)       { conf->scheduler = v.toStdString(); }

void MachineConfigurationWindow::checkMaster(int checked)
{
    conf->master = (checked == Qt::Checked);
}

void MachineConfigurationWindow::on_pushButton_clicked()
{
    QString     slaves     = ui->idPlainText->toPlainText();
    QStringList slavelist  = slaves.split(" ");
    for (const QString& slave : slavelist)
        conf->slaves.push_back(slave.toInt());
    this->close();
}
