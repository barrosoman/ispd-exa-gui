#include "window/switchconfigurationwindow.h"
#include "ui_switchconfigurationwindow.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

SwitchConfigurationWindow::SwitchConfigurationWindow(Schema* schema, unsigned id, QWidget* parent)
    : QDialog(parent), ui(new Ui::SwitchConfigurationWindow), schema(schema), id(id)
{
    this->ui->setupUi(this);
    this->setupConfAndWindow();
}

void SwitchConfigurationWindow::setupConfAndWindow()
{
    auto* conf = schema->findSwitch(id);
    if (!conf) return;

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

void SwitchConfigurationWindow::setName(const QString& v) { auto* c = schema->findSwitch(id); if (c) c->name      = v.toStdString(); }
void SwitchConfigurationWindow::setBandwidth(int v)        { auto* c = schema->findSwitch(id); if (c) c->bandwidth  = v; }
void SwitchConfigurationWindow::setLoadFactor(double v)    { auto* c = schema->findSwitch(id); if (c) c->loadFactor = v; }
void SwitchConfigurationWindow::setLatency(double v)       { auto* c = schema->findSwitch(id); if (c) c->latency    = v; }
