#pragma once
#include "components/schema.h"
#include <QDialog>

namespace Ui {
class SwitchConfigurationWindow;
}

class SwitchConfigurationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SwitchConfigurationWindow(Schema* schema, unsigned id, QWidget* parent = nullptr);

public slots:
    void setName(const QString& newName);
    void setBandwidth(int newBandwidth);
    void setLoadFactor(double newLoadFactor);
    void setLatency(double newLatency);

private:
    void setupConfAndWindow();
    Ui::SwitchConfigurationWindow* ui;
    Schema*  schema;
    unsigned id;
};
