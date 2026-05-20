#pragma once
#include "components/schema.h"
#include "context/context.h"
#include "icon/pixmapicon.h"
#include "window/drawingtable/view.h"
#include "window/simulation.h"
#include "window/users.h"
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class Scene;

class DrawingTable : public QWidget
{
    Q_OBJECT

public:
    DrawingTable(QFrame* parent = nullptr);
    DrawingTable(Schema* schema, QWidget* parent = nullptr);

    Schema* schema;

    PixmapIcon* addMachine();
    PixmapIcon* addSwitch();
    PixmapIcon* addSet();
    unsigned    addLink(unsigned from_id, unsigned to_id);

    Scene* getScene();

    void openUserWindowClicked();
    void openSimulationWindowClicked();
    void openWorkloadWindow();

    void pcButtonClicked();
    void setButtonClicked();
    void switchButtonClicked();
    void schemaButtonClicked();
    void linkButtonClicked();
    void noneButtonClicked();

    Context::MainContext mainContext;

private:
    Scene* scene;
    View*  view;

    QWidget*     buttonsRow;
    QHBoxLayout* buttonsLayout;
    QRadioButton* pcButton;
    QRadioButton* setButton;
    QRadioButton* schemaButton;
    QRadioButton* linkButton;
    QRadioButton* noneButton;
    QPushButton*  openUserWindow;
    QPushButton*  openSimulationWindow;

    void setupPcButton();
    void setupSwitchButton();
    void setupSetButton();
    void setupNoneButton();
    void setupSchemaButton();
    void setupLinkButton();
};
