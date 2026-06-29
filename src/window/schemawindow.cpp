#include "window/schemawindow.h"
#include "window/drawingtable/drawingtable.h"
#include "window/drawingtable/scene.h"
#include "window/propertiespanel.h"
#include <QDebug>
#include <QDockWidget>
#include <QWidget>

SchemaWindow::SchemaWindow(Schema *schema, std::map<unsigned, QPointF> layout, QWidget *parent)
    : QMainWindow{parent}
{
    this->schema = schema;
    drawingTable = new DrawingTable(schema, std::move(layout), this);
    this->setCentralWidget(drawingTable);

    propertiesPanel = new PropertiesPanel(this);
    auto* dock = new QDockWidget("Properties", this);
    dock->setWidget(propertiesPanel);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(drawingTable->getScene(), &Scene::nodeSelected, this, [this, schema](unsigned id){
        propertiesPanel->display(schema, id);
    });

    qDebug() << this->rect();
}
