#pragma once
#include "components/schema.h"
#include <QDialog>

namespace Ui {
class LinkConfigurationWindow;
}

class LinkConfigurationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LinkConfigurationWindow(Schema* schema, unsigned id, QWidget* parent = nullptr);

public slots:
    void setName(const QString& newName);
    void setBandwidth(int newBandwidth);
    void setLoadFactor(double newLoadFactor);
    void setLatency(double newLatency);

private:
    void setupConfAndWindow();
    Ui::LinkConfigurationWindow* ui;
    Schema*  schema;
    unsigned id;
};
