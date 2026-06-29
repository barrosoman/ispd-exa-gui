#include "icon/pixmapicon.h"
#include "window/drawingtable/scene.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

PixmapIcon::PixmapIcon(unsigned node_id, PixmapPair pixmapPair)
    : node_id(node_id), pixmapPair(pixmapPair)
{
    setFlags(QGraphicsItem::ItemIsMovable);
    setPixmap(this->pixmapPair.getNormal());
}

void PixmapIcon::toggleChoosen()
{
    setPixmap(chose ? pixmapPair.getNormal() : pixmapPair.getSelected());
    chose = !chose;
}

bool PixmapIcon::isChosen() { return chose; }

void PixmapIcon::toggleChosenIfInside(QRectF area)
{
    if (area.contains(sceneBoundingRect()) && !chose)
        toggleChoosen();
}

void PixmapIcon::updatePosition()
{
    if (auto* s = dynamic_cast<Scene*>(scene()))
        s->onNodeMoved(node_id);
}

void PixmapIcon::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "Node id:" << node_id;
    if (auto* s = dynamic_cast<Scene*>(scene()))
        s->onNodeClicked(node_id);
    QGraphicsPixmapItem::mousePressEvent(event);
}

void PixmapIcon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (auto* s = dynamic_cast<Scene*>(scene()))
        s->onNodeDoubleClicked(node_id);
    QGraphicsPixmapItem::mouseDoubleClickEvent(event);
}

void PixmapIcon::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    clickTimer.setInterval(CLICK_DURATION);
    QGraphicsPixmapItem::mouseMoveEvent(event);
    updatePosition();
}

void PixmapIcon::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    clickTimer.stop();
    if (clickTimer.interval() < CLICK_DURATION)
        toggleChoosen();
    clickTimer.setInterval(0);
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}
