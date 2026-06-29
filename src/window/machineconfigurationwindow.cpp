#include "window/machineconfigurationwindow.h"
#include "ui_machineconfigurationwindow.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

MachineConfigurationWindow::MachineConfigurationWindow(Schema* schema, unsigned id, QWidget* parent)
    : QDialog(parent), ui(new Ui::MachineConfigurationWindow), schema(schema), id(id)
{
    this->ui->setupUi(this);
    this->setupConfAndWindow();
}

void MachineConfigurationWindow::setupConfAndWindow()
{
    auto* conf = schema->findMachine(id);
    if (!conf) return;

    this->setFixedHeight(700);
    this->ui->nameEditLine->setText(conf->name.c_str());
    this->ui->coresSpinBox->setValue(conf->coreCount);
    this->ui->computationPowerSpinBox->setValue(conf->computationalPower);
    this->ui->loadFactorSpinBox->setValue(conf->loadFactor);
    this->ui->gpuCoresSpinBox->setValue(conf->gpuCoreCount);
    this->ui->gpuPower->setValue(conf->gpuPower);
    this->ui->gpuBandwidth->setValue(conf->gpuInterconnectionBandwidth);
    this->ui->ramSpinBox->setValue(conf->ram);
    this->ui->diskSpinBox->setValue(conf->hardDisk);
    this->ui->energyMaxSpinBox->setValue(conf->wattageMax);
    this->ui->energyIdleSpinBox->setValue(conf->wattageIdle);
    this->ui->masterCheckBox->setChecked(conf->master);
    this->ui->schedulersComboBox->setCurrentText(conf->scheduler.c_str());

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
    connect(this->ui->masterCheckBox,          &QCheckBox::checkStateChanged,
            this, &MachineConfigurationWindow::checkMaster);
    connect(this->ui->energyIdleSpinBox,       qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setWattageIdle);
    connect(this->ui->energyMaxSpinBox,        qOverload<int>(&QSpinBox::valueChanged),
            this, &MachineConfigurationWindow::setWattageMax);
    connect(this->ui->schedulersComboBox,      &QComboBox::currentTextChanged,
            this, &MachineConfigurationWindow::setScheduler);
}

void MachineConfigurationWindow::setName(const QString& v)    { auto* c = schema->findMachine(id); if (c) c->name = v.toStdString(); }
void MachineConfigurationWindow::setCoreCount(int v)          { auto* c = schema->findMachine(id); if (c) c->coreCount = v; }
void MachineConfigurationWindow::setComputationPower(int v)   { auto* c = schema->findMachine(id); if (c) c->computationalPower = v; }
void MachineConfigurationWindow::setLoadFactor(double v)      { auto* c = schema->findMachine(id); if (c) c->loadFactor = v; }
void MachineConfigurationWindow::setGpuCoreCount(int v)       { auto* c = schema->findMachine(id); if (c) c->gpuCoreCount = v; }
void MachineConfigurationWindow::setGpuPower(int v)           { auto* c = schema->findMachine(id); if (c) c->gpuPower = v; }
void MachineConfigurationWindow::setGpuBandwidth(int v)       { auto* c = schema->findMachine(id); if (c) c->gpuInterconnectionBandwidth = v; }
void MachineConfigurationWindow::setRam(int v)                { auto* c = schema->findMachine(id); if (c) c->ram = v; }
void MachineConfigurationWindow::setDisk(int v)               { auto* c = schema->findMachine(id); if (c) c->hardDisk = v; }
void MachineConfigurationWindow::setWattageIdle(int v)        { auto* c = schema->findMachine(id); if (c) c->wattageIdle = v; }
void MachineConfigurationWindow::setWattageMax(int v)         { auto* c = schema->findMachine(id); if (c) c->wattageMax = v; }
void MachineConfigurationWindow::setScheduler(const QString& v){ auto* c = schema->findMachine(id); if (c) c->scheduler = v.toStdString(); }

void MachineConfigurationWindow::checkMaster(Qt::CheckState checked)
{
    auto* c = schema->findMachine(id);
    if (c) c->master = (checked == Qt::Checked);
}

void MachineConfigurationWindow::on_pushButton_clicked()
{
    auto* c = schema->findMachine(id);
    if (c) {
        QString     slaves    = ui->idPlainText->toPlainText();
        QStringList slavelist = slaves.split(" ");
        for (const QString& slave : slavelist)
            c->slaves.push_back(slave.toInt());
    }
    this->close();
}
