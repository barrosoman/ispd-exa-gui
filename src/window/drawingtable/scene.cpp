#include "window/drawingtable/scene.h"
#include "window/drawingtable/drawingtable.h"
#include "components/link.h"
#include "icon/linkicon.h"
#include "icon/machineicon.h"
#include "icon/schemaicon.h"
#include "qgraphicsitem.h"
#include "components/schema.h"
#include "window/machineconfiguration.h"
#include "window/users.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <iostream>
#include <string>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>

///
/// Create the scene following the QGraphicsScene constructor
///
Scene::Scene(DrawingTable *parent) : QGraphicsScene{parent}
{
    this->table  = parent;
    this->schema = this->table->schema;
    this->lBegin = nullptr;
    this->lEnd   = nullptr;
    this->pickOp = NONE;

    setSceneRect(0, 0, 2000, 2000);
    drawBackgroundLines();
}

///
/// @brief Add an Icon to the position specified.
///
/// @params icon an icon (Machine, Schema...)
/// @params pos  the position to set the icon
///
void Scene::addIcon(Icon *icon, QPointF pos)
{
    icon->setPos(pos);
    icon->setOutputLabel(machineDescriptionLabel);
    this->addItem(icon);
}

///
/// @brief Add an Link to the position specified.
///
/// @params link an Link
/// @params a    the Item that the Link comes from
/// @params b    the Item that the Link goes to
///
void Scene::addLink(Link *link, Connection *a, Connection *b)
{
    link->addLine(a, b);

    this->addItem(link->icon);
}

///
/// @brief Handles a keyboard keypress.
///
/// @params event a keyboard event
///
void Scene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        this->deleteItems();
    }

    QGraphicsScene::keyPressEvent(event);
}

void Scene::deleteItems()
{
    std::map<unsigned, Machine *> machinesToRemove;
    std::map<unsigned, Link *>    linksToRemove;

    for (auto iter : *this->schema->machines) {
        Machine     *m     = iter.second;
        MachineIcon *mIcon = m->icon;

        if (mIcon->isSelected) {
            machinesToRemove.insert(std::pair(mIcon->id, m));

            for (auto linkIter : *m->connected_links) {
                Link *link = linkIter.second;

                linksToRemove.insert(std::pair(link->id, link));

                /* auto *otherIcon = (link->icon->begin == mIcon) */
                /*                       ? link->icon->end */
                /*                       : link->icon->begin; */
                /* otherIcon->links->erase(link); */
            }
            mIcon->links->clear();
        }
    }

    /* for (auto icon : iconsToRemove) { */
    /*     if (dynamic_cast<MachineIcon *>(icon.second) != nullptr) { */
    /*         this->removeMachine((MachineIcon *)icon.second); */
    /*     } */
    /* } */
    /*  */
    /* for (auto link : linksToRemove) { */
    /*     this->removeLink(link.second); */
    /* } */
}

///
/// @brief Handles the mouse press, adapting the behavior based on the actual
///        state.
///
/// @params event a mouse press event
///
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (this->pickOp) {
    case NONE: {
        selectionArea(event);
        QGraphicsScene::mousePressEvent(event);
        break;
    }
    case PC: {
        auto newMachine = this->table->addMachine();

        this->addIcon(newMachine, event->scenePos());

        break;
    }
    case SCHEMA: {
        auto newSchema = this->table->addSchema();
        this->addIcon(newSchema, event->scenePos());
        if (newSchema->owner) {
            qDebug() << "Owner of schema exists";
            break;
        }
        qDebug() << "Owner of schema exists";
        break;
    }
    case LINK: {
        auto *connection = whichConnection(event->scenePos());

        if (!connection) {
            return;
        }

        if (this->lBegin == nullptr) {
            qDebug() << "Primeira máquina\n";
            this->lBegin = connection;
        }
        else if (this->lEnd == nullptr) {

            if (whichConnection(event->scenePos()) == this->lBegin) {
                break;
            }
            this->lEnd = connection;

            Link *newLink = this->table->addLink();
            qDebug() << "Antes de enfia link na scene.";
            this->addLink(newLink, this->lBegin, this->lEnd);

            this->lBegin = nullptr;
            this->lEnd   = nullptr;
        }
        break;
    }
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->startSelection != QPointF()) {
        QRectF selectionAreaRect = QRectF(this->startSelection, event->scenePos()).normalized();
        this->selectionRect->setRect(selectionAreaRect);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && this->startSelection != QPointF()) {
        // Calculate the selection area rectangle
        QRectF selectionAreaRect = QRectF(this->startSelection, event->scenePos()).normalized();

        // Deselect all icons outside the selection area
            for (auto item : this->items()) {
                if (Icon *icon = dynamic_cast<Icon *>(item)) {
                    if (selectionAreaRect.contains(icon->sceneBoundingRect())) {
                        icon->selection(true);
                    } else {
                        if (event->modifiers() & Qt::ShiftModifier) {
                        } else {
                        icon->selection(false);
                        }
                    }
                }
            }
        // Reset the initial position for area selection
        this->startSelection = QPointF();
        this->removeItem(this->selectionRect); // Remove the selection rectangle from the scene
        delete this->selectionRect;
        this->selectionRect = nullptr;
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::selectionArea(QGraphicsSceneMouseEvent *event) {
    /* Icon *clickedIcon = whichMachine(event->scenePos()); */
    /* if (!clickedIcon && !dynamic_cast<MachineIcon*>(clickedIcon)) { */
    /*     if (event->button() == Qt::LeftButton) { */
    /*         this->startSelection = event->scenePos(); */
    /*         this->selectionRect = new QGraphicsRectItem(); */
    /*         this->selectionRect->setPen(QPen(Qt::blue, 1, Qt::SolidLine)); // Change color and pen style */
    /*         this->selectionRect->setBrush(QBrush(QColor(100, 100, 255, 40))); */
    /*         this->selectionRect->setRect(QRectF(this->startSelection, event->scenePos()).normalized()); */
    /*         this->addItem(this->selectionRect); // Add the selection rectangle to the scene */
    /*     } */
    /* } */
}

///
/// @brief Draw lines at the background of the scene.
///
void Scene::drawBackgroundLines()
{
    auto rect = sceneRect();
    QPen pen; // creates a default pen

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(QColor(211, 211, 211, 255));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    for (qreal i = 40; i <= rect.height(); i += 40) {
        auto line = new QGraphicsLineItem(0, i, rect.width(), i);
        line->setPen(pen);
        line->setZValue(-2);
        addItem(line);
    }

    for (qreal i = 40; i <= rect.width(); i += 40) {
        auto line = new QGraphicsLineItem(i, 0, i, rect.height());
        line->setPen(pen);
        line->setZValue(-2);
        addItem(line);
    }
}

///
/// @brief  Finds the machine icon located at the specified position within
///         the scene.
///
/// @param  pos The position in the scene to check for a machine icon.
/// @return a pointer to the machine icon if found, or nullptr if not found.
///
Connection *Scene::whichConnection(QPointF pos)
{
    for (auto i = this->schema->machines->begin();
         i != this->schema->machines->end();
         i++) {
        if ((*i).second->icon->sceneBoundingRect().contains(pos)) {
            return (*i).second;
        }
    }

    for (auto i = this->schema->schemas->begin();
         i != this->schema->schemas->end();
         i++) {
        if ((*i).second->icon->sceneBoundingRect().contains(pos)) {
            return (*i).second;
        }
    }
    return nullptr;
}

void Scene::removeMachine(Machine *machine)
{
    unsigned id = machine->id;

    this->schema->deleteMachine(id);

    removeItemIcon(machine);

    delete (machine);
}

void Scene::removeItemIcon(Item *item)
{
    removeItem(item->getIcon());
}

void Scene::removeLink(Link *link)
{
    unsigned id = link->id;

    this->schema->deleteLink(id);

    removeItem(link->icon);
}