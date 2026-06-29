#pragma once
#include "components/schema.h"
#include "icon/pixmapicon.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QWidget>
#include <map>

class LinkIcon;
class DrawingTable;

enum PICK_OP { NONE, PC, SCHEMA, LINK, SWITCH, SET };

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(DrawingTable* parent);

    PICK_OP pickOp = NONE;

    std::map<unsigned, PixmapIcon*> nodeIcons;
    std::map<unsigned, LinkIcon*>   linkIcons;
    std::map<unsigned, QPointF>     layout;

    void addIcon(PixmapIcon* icon, QPointF pos = QPointF(0, 0));
    void addLink(LinkIcon* licon);
    void drawBackgroundLines();
    void populate();

    void onNodeMoved(unsigned id);
    void onNodeClicked(unsigned id);
    void onNodeDoubleClicked(unsigned id);
    void onLinkDoubleClicked(unsigned id);

    unsigned whichNode(QPointF pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void clicked(QPointF position);
    void nodeSelected(unsigned id);

private:
    QPointF      getScenePosition();
    void         deleteItems();
    void         selectionArea(QGraphicsSceneMouseEvent* event);

    DrawingTable*    table;
    Schema*          schema;

    unsigned         lBegin_id    = 0;
    unsigned         clipboard_id = 0;

    std::map<unsigned, QWidget*> configWindows;
    std::map<unsigned, QWidget*> linkConfigWindows;

    QPointF            startSelection;
    QGraphicsRectItem* selectionRect = nullptr;
};
