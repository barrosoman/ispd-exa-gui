#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QPointF>
#include <map>

class Schema;
class DrawingTable;
class PropertiesPanel;

class SchemaWindow : public QMainWindow
{
    Q_OBJECT

public:
    Schema *schema;
    explicit SchemaWindow(Schema *schema,
                          std::map<unsigned, QPointF> layout = {},
                          QWidget *parent = nullptr);
    DrawingTable   *drawingTable;
    PropertiesPanel *propertiesPanel;
};
