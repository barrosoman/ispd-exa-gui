#pragma once
#include <QGraphicsPolygonItem>
#include <QPen>

class PixmapIcon;
class Scene;

class LinkIcon : public QGraphicsPolygonItem
{
public:
    LinkIcon(unsigned link_id, PixmapIcon* begin, PixmapIcon* end);

    unsigned    link_id;
    PixmapIcon* begin;
    PixmapIcon* end;

    void draw();
    void updatePosition();
    void toggleChoosen();
    void toggleChosenIfInside(QRectF area);
    bool isChosen();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    QPen linkPen;
    bool chose = false;
};
