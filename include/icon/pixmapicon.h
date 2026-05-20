#pragma once
#include "utils/iconSize.h"
#include <QGraphicsPixmapItem>
#include <QTimer>

#define CLICK_DURATION 200

class Scene;

class PixmapPair
{
public:
    PixmapPair(const char* normalPath, const char* selectedPath)
    {
        normal   = QPixmap(normalPath).scaled(iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        selected = QPixmap(selectedPath).scaled(iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    QPixmap getNormal()   const { return normal; }
    QPixmap getSelected() const { return selected; }
private:
    QPixmap normal;
    QPixmap selected;
};

class PixmapIcon : public QGraphicsPixmapItem
{
public:
    PixmapIcon(unsigned node_id, PixmapPair pixmapPair);

    unsigned node_id;

    void toggleChoosen();
    bool isChosen();
    void toggleChosenIfInside(QRectF area);
    void updatePosition();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QTimer     clickTimer;
    bool       chose = false;
    PixmapPair pixmapPair;
};
