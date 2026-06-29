#include "window/linkconfigurationwindow.h"
#include "ui_linkconfigurationwindow.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

LinkConfigurationWindow::LinkConfigurationWindow(Schema* schema, unsigned id, QWidget* parent)
    : QDialog(parent), ui(new Ui::LinkConfigurationWindow), schema(schema), id(id)
{
    this->ui->setupUi(this);
    this->setupConfAndWindow();
}

void LinkConfigurationWindow::setupConfAndWindow()
{
    auto* conf = schema->findLink(id);
    if (!conf) return;

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

void LinkConfigurationWindow::setName(const QString& v) { auto* c = schema->findLink(id); if (c) c->name      = v.toStdString(); }
void LinkConfigurationWindow::setBandwidth(int v)        { auto* c = schema->findLink(id); if (c) c->bandwidth  = v; }
void LinkConfigurationWindow::setLoadFactor(double v)    { auto* c = schema->findLink(id); if (c) c->loadFactor = v; }
void LinkConfigurationWindow::setLatency(double v)       { auto* c = schema->findLink(id); if (c) c->latency    = v; }
