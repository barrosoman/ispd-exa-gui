#include "icon/linkicon.h"
#include "icon/pixmapicon.h"
#include "window/drawingtable/scene.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <cmath>

static QPointF midOf(PixmapIcon* a)
{
    return {a->pos().x() + a->pixmap().width()  / 2.0,
            a->pos().y() + a->pixmap().height() / 2.0};
}

LinkIcon::LinkIcon(unsigned link_id, PixmapIcon* begin, PixmapIcon* end)
    : link_id(link_id), begin(begin), end(end)
{}

void LinkIcon::draw()
{
    linkPen.setWidth(2);
    linkPen.setColor(QColor(9, 132, 227));
    linkPen.setCapStyle(Qt::RoundCap);
    linkPen.setJoinStyle(Qt::RoundJoin);
    linkPen.setCosmetic(true);

    auto* shadow = new QGraphicsDropShadowEffect();
    shadow->setColor(QColor(0, 0, 0, 100));
    shadow->setBlurRadius(4);
    shadow->setOffset(2);
    setGraphicsEffect(shadow);

    setPen(linkPen);
    setPolygon(QPolygonF() << midOf(begin) << midOf(end));
    setZValue(-1);
}

void LinkIcon::updatePosition()
{
    setPolygon(QPolygonF() << midOf(begin) << midOf(end));
}

void LinkIcon::toggleChoosen()
{
    QPen pen;
    pen.setColor(chose ? QColor(9, 132, 227) : QColor(245, 69, 55));
    setPen(pen);
    chose = !chose;
}

bool LinkIcon::isChosen() { return chose; }

void LinkIcon::toggleChosenIfInside(QRectF area)
{
    if (area.contains(sceneBoundingRect()) && !chose)
        toggleChoosen();
}

void LinkIcon::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "Link id:" << link_id;
    toggleChoosen();
    QGraphicsPolygonItem::mousePressEvent(event);
}

void LinkIcon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (auto* s = dynamic_cast<Scene*>(scene()))
        s->onLinkDoubleClicked(link_id);
    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
}

void LinkIcon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QGraphicsPolygonItem::paint(painter, option, widget);

    QLineF line(midOf(begin), midOf(end));
    QLineF norm  = line.normalVector().unitVector();
    qreal  size  = 5;
    qreal  angle = std::atan2(-norm.dy(), norm.dx());

    QPointF p1 = line.p2() - QPointF(std::sin(angle - M_PI / 3) * size,
                                      std::cos(angle - M_PI / 3) * size);
    QPointF p2 = line.p2() - QPointF(std::sin(angle + M_PI / 3) * size,
                                      std::cos(angle + M_PI / 3) * size);

    painter->setBrush(QColor(9, 132, 227));
    painter->drawPolygon(QPolygonF() << line.p2() << p1 << p2);
}
