#include "window/machinesetconfigurationwindow.h"
#include "ui_machinesetconfigurationwindow.h"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

MachineSetConfigurationWindow::MachineSetConfigurationWindow(Schema* schema, unsigned id, QWidget* parent)
    : QDialog(parent), ui(new Ui::MachineSetConfigurationWindow), schema(schema), id(id)
{
    this->ui->setupUi(this);
    auto* conf = schema->findMachineSet(id);
    if (!conf) return;
    this->ui->nameEditLine->setText(conf->name.c_str());

    connect(this->ui->nameEditLine, &QLineEdit::textChanged,
            this, &MachineSetConfigurationWindow::setName);
}

void MachineSetConfigurationWindow::setName(const QString& v) { auto* c = schema->findMachineSet(id); if (c) c->name      = v.toStdString(); }
void MachineSetConfigurationWindow::setBandwidth(int v)        { auto* c = schema->findMachineSet(id); if (c) c->bandwidth  = v; }
void MachineSetConfigurationWindow::setLoadFactor(double v)    { auto* c = schema->findMachineSet(id); if (c) c->loadFactor = v; }
void MachineSetConfigurationWindow::setLatency(double v)       { auto* c = schema->findMachineSet(id); if (c) c->latency    = v; }
