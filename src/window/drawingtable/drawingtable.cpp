#include "window/drawingtable/drawingtable.h"
#include "components/link.h"
#include "components/machine.h"
#include "components/schema.h"
#include "components/switch.h"
#include "context/user.h"
#include "icon/pixmapicon.h"
#include "utils/iconPath.h"
#include <fstream>

#include "utils/iconSize.h"
#include "window/drawingtable/scene.h"
#include "window/users.h"
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QPushButton>
#include <QRadioButton>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <memory>
#include <qt/QtWidgets/qsizepolicy.h>
#include <window/addworkloadwindow.h>
void printSchema(Schema *schema);

DrawingTable::DrawingTable(QFrame *parent) : DrawingTable(new Schema(), {}, parent)
{
    mainContext.mainSchema = std::shared_ptr<Schema>(this->schema);

    //-------------------------------------------------------------------------
    // TEMPORARY
    mainContext.users.push_back(Context::User{.name="John", .allowedUsage=0.9});

    //-------------------------------------------------------------------------

    /* QPixmap workloadIcon(":/icons/perfil.png"); */
    /* QPixmap  = image.scaled(buttonSize); */

    auto workloadButton = new QPushButton("Workload", this);
    workloadButton->setSizePolicy(
        QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    workloadButton->setFixedSize(70, 40);

    //-------------------------------------------------------------------------

    QPixmap image(":/icons/perfil.png");
    QPixmap resizedImage = image.scaled(buttonSize);

    openUserWindow = new QPushButton("Users", this);
    openUserWindow->setIcon(QIcon(resizedImage));
    openUserWindow->setSizePolicy(
        QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    openUserWindow->setFixedSize(70, 40);

    //-------------------------------------------------------------------------

    QPixmap image_2(":/icons/engine.png");
    QPixmap resizedImage_2 = image_2.scaled(buttonSize);

    openSimulationWindow = new QPushButton("Simulate", this);
    openSimulationWindow->setIcon(QIcon(resizedImage_2));
    openUserWindow->setSizePolicy(
        QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    openSimulationWindow->setFixedSize(100, 40);

    //-------------------------------------------------------------------------

    buttonsLayout->addWidget(workloadButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(openUserWindow, 0, Qt::AlignRight);
    buttonsLayout->addWidget(openSimulationWindow, 0, Qt::AlignRight);


    connect(openUserWindow,
            &QPushButton::clicked,
            this,
            &DrawingTable::openUserWindowClicked);
    connect(openSimulationWindow,
            &QPushButton::clicked,
            this,
            &DrawingTable::openSimulationWindowClicked);
    connect(workloadButton,
            &QPushButton::clicked,
            this,
            &DrawingTable::openWorkloadWindow);

}

DrawingTable::DrawingTable(Schema *schema, std::map<unsigned, QPointF> layout, QWidget *parent) : QWidget{parent}
{
    this->schema = schema;
    this->view   = new View(this);
    this->scene  = new Scene(this);
    this->view->setGScene(scene);
    this->buttonsRow    = new QWidget(this);
    this->buttonsLayout = new QHBoxLayout(buttonsRow);
    this->buttonsLayout->setAlignment(Qt::AlignLeft);

    this->setupNoneButton();
    this->setupPcButton();
    this->setupSchemaButton();
    this->setupSetButton();
    this->setupLinkButton();
    this->setupSwitchButton();

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(buttonsRow);
    mainLayout->addWidget(view);

    scene->layout = std::move(layout);
    scene->populate();
}

///
/// @brief Creates the PC option button and connects it to the scene
///
void DrawingTable::setupPcButton()
{
    this->pcButton = new QRadioButton(this->buttonsRow);
    this->pcButton->setIcon(QIcon(QPixmap::fromImage(QImage(":icons/pc.png"))));
    this->pcButton->setIconSize(buttonSize);
    this->buttonsLayout->addWidget(pcButton);
    QObject::connect(
        pcButton, &QRadioButton::clicked, this, &DrawingTable::pcButtonClicked);
}

void DrawingTable::setupSetButton()
{
    this->setButton = new QRadioButton(this->buttonsRow);
    this->setButton->setIcon(
        QIcon(QPixmap::fromImage(QImage(":icons/connectableset.png"))));
    this->setButton->setIconSize(buttonSize);
    this->buttonsLayout->addWidget(setButton);
    QObject::connect(setButton,
                     &QRadioButton::clicked,
                     this,
                     &DrawingTable::setButtonClicked);
}

///
/// @brief Creates the Cursor option button and connects it to the scene
///
void DrawingTable::setupNoneButton()
{
    noneButton = new QRadioButton(buttonsRow);
    noneButton->setIcon(QIcon(QPixmap::fromImage(QImage(":icons/cursor.png"))));
    noneButton->setIconSize(buttonSize);
    buttonsLayout->addWidget(noneButton);
    QObject::connect(noneButton,
                     &QRadioButton::clicked,
                     this,
                     &DrawingTable::noneButtonClicked);
}

///
/// @brief Creates the Switch option button and connects it to the scene
///
void DrawingTable::setupSwitchButton()
{
    noneButton = new QRadioButton(buttonsRow);
    noneButton->setIcon(QIcon(QPixmap::fromImage(QImage(":icons/switch.svg"))));
    noneButton->setIconSize(buttonSize);
    buttonsLayout->addWidget(noneButton);
    QObject::connect(noneButton,
                     &QRadioButton::clicked,
                     this,
                     &DrawingTable::switchButtonClicked);
}

///
/// @brief Creates the Schema option button and connects it to the scene
///
void DrawingTable::setupSchemaButton()
{

    schemaButton = new QRadioButton(buttonsRow);
    schemaButton->setIcon(
        QIcon(QPixmap::fromImage(QImage(":icons/cluster.png"))));
    schemaButton->setIconSize(buttonSize);
    buttonsLayout->addWidget(schemaButton);
    QObject::connect(schemaButton,
                     &QRadioButton::clicked,
                     this,
                     &DrawingTable::schemaButtonClicked);
}

///
/// @brief Creates the Link option button and connects it to the scene
///
void DrawingTable::setupLinkButton()
{
    const QIcon linkIcon(QPixmap::fromImage(QImage(":icons/connection.png")));

    linkButton = new QRadioButton(buttonsRow);
    linkButton->setIcon(linkIcon);
    linkButton->setIconSize(buttonSize);
    buttonsLayout->addWidget(linkButton);
    QObject::connect(linkButton,
                     &QRadioButton::clicked,
                     this,
                     &DrawingTable::linkButtonClicked);
}

///
/// @brief  Creates a Machine inside the DrawingTable's Schema
///
/// @return the machine's icon
///
PixmapIcon *DrawingTable::addMachine()
{
    const unsigned id = schema->addMachine();
    return new PixmapIcon(id, PixmapPair(machinePath, machinePathSelected));
}

///
/// @brief  Creates a Switch inside the DrawingTable's Schema
///
/// @return the switch's icon
///
PixmapIcon *DrawingTable::addSwitch()
{
    const unsigned id = schema->addSwitch();
    return new PixmapIcon(id, PixmapPair(switchPath, switchPathSelected));
}

PixmapIcon *DrawingTable::addSchema()
{
    const unsigned id = schema->addSchema();
    return new PixmapIcon(id, PixmapPair(schemaPath, schemaPathSelected));
}

PixmapIcon *DrawingTable::addSet()
{
    const unsigned id = schema->addMachineSet();
    return new PixmapIcon(id, PixmapPair(setPath, setPathSelected));
}

///
/// @brief  Creates a Link inside the DrawingTable's Schema
///
unsigned DrawingTable::addLink(unsigned from_id, unsigned to_id)
{
    return schema->addLink(from_id, to_id);
}

///
/// @brief  set the scene operator to the PC insert mode
///
void DrawingTable::pcButtonClicked()
{
    this->scene->pickOp = PC;
}

void DrawingTable::setButtonClicked()
{
    this->scene->pickOp = SET;
}

///
/// @brief  set the scene operator to the switch insert mode
///
void DrawingTable::switchButtonClicked()
{
    this->scene->pickOp = SWITCH;
}

///
/// @brief  set the scene operator to the click mode
///
void DrawingTable::noneButtonClicked()
{
    this->scene->pickOp = NONE;
}

///
/// @brief  set the scene operator to the link insert mode
///
void DrawingTable::linkButtonClicked()
{
    this->scene->pickOp = LINK;
}

///
/// @brief  set the scene operator to the schema insert mode
///
void DrawingTable::schemaButtonClicked()
{
    this->scene->pickOp = SCHEMA;
}

///
/// @brief  prints the DrawingTable's Schema and its components
///
void printSchema(Schema *schema)
{
    for (const auto& m : schema->machines)
        qDebug() << "Machine #" << m.id << ": " << m.name.c_str();
    for (const auto& s : schema->switches)
        qDebug() << "Switch #" << s.id << ": " << s.name.c_str();
    for (const auto& ms : schema->machineSets)
        qDebug() << "MachineSet #" << ms.id << ": " << ms.name.c_str();
    for (const auto& l : schema->links)
        qDebug() << "Link #" << l.id << ": " << l.name.c_str();
}
void DrawingTable::openUserWindowClicked()
{
    auto userWindow = new UserWindow(&this->mainContext);

    userWindow->show();
}

void DrawingTable::openSimulationWindowClicked()
{
    /// temporary must be removed when simulation allows more than one workload
    if (!this->mainContext.workloads.empty()) {
        for (const auto& m : this->schema->machines) {
            if (m.master) {
                this->mainContext.workloads.at(0).master_id = m.id;
                break;
            }
        }
    }


    json j     = *this->schema;
    j["users"] = this->mainContext.users;

    j["workloads"] = this->mainContext.workloads;



    std::string fileName = "output.json";
    std::ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        outputFile << j.dump(4);
        outputFile.close();
        QMessageBox::information(nullptr, "Sucesso", "Arquivo criado com sucesso.");

    } else {
        QMessageBox::critical(nullptr, "Erro", "Erro ao abrir o arquivo para escrita.");

    }

    QFile file("routes.route");

           /// @todo treat it
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        exit(0);
    }

    QTextStream outStream(&file);

    for (const auto& link : this->schema->links) {
        outStream << link.from_id << " ";
        outStream << link.id << " ";
        outStream << link.to_id << "\n";
    }

    file.close();
    /* this->simulationWindow = new Simulation(); */
    /* simulationWindow->show(); */
}


void DrawingTable::openWorkloadWindow()
{
    addWorkloadWindow *workloadwindow = new addWorkloadWindow(this, &this->mainContext);

    workloadwindow->show();
}


Scene *DrawingTable::getScene()
{
    return this->scene;
}
