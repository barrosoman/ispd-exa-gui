#include "window/linkconfigurationwindow.h"
#include "ui_linkconfigurationwindow.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

LinkConfigurationWindow::LinkConfigurationWindow(LinkConf* conf, QWidget* parent)
    : QDialog(parent), ui(new Ui::LinkConfigurationWindow), conf(conf)
{
    this->ui->setupUi(this);
    this->setupConfAndWindow();
}

void LinkConfigurationWindow::setupConfAndWindow()
{
    this->ui->nameEditLine->setText(conf->name.c_str());
    this->ui->bandwidthSpinBox->setValue(conf->bandwidth);
    this->ui->latencySpinBox->setValue(conf->latency);
    this->ui->loadFactorSpinBox->setValue(conf->loadFactor);

    connect(this->ui->nameEditLine,      &QLineEdit::textChanged,
            this, &LinkConfigurationWindow::setName);
    connect(this->ui->bandwidthSpinBox,  qOverload<int>(&QSpinBox::valueChanged),
            this, &LinkConfigurationWindow::setBandwidth);
    connect(this->ui->latencySpinBox,    qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &LinkConfigurationWindow::setLatency);
    connect(this->ui->loadFactorSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &LinkConfigurationWindow::setLoadFactor);
}

void LinkConfigurationWindow::setName(const QString& newName) { conf->name      = newName.toStdString(); }
void LinkConfigurationWindow::setBandwidth(int v)             { conf->bandwidth  = v; }
void LinkConfigurationWindow::setLoadFactor(double v)         { conf->loadFactor = v; }
void LinkConfigurationWindow::setLatency(double v)            { conf->latency    = v; }
