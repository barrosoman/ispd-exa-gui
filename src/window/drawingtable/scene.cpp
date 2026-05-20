#include "window/drawingtable/scene.h"
#include "icon/linkicon.h"
#include "utils/iconPath.h"
#include "window/drawingtable/drawingtable.h"
#include "window/linkconfigurationwindow.h"
#include "window/machineconfigurationwindow.h"
#include "window/machinesetconfigurationwindow.h"
#include "window/switchconfigurationwindow.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPen>

Scene::Scene(DrawingTable* parent)
    : QGraphicsScene{parent}, table(parent)
{
    schema = table->schema;
    setSceneRect(0, 0, 2000, 2000);
    drawBackgroundLines();
}

QPointF Scene::getScenePosition()
{
    QGraphicsView* view = this->views().first();
    return view->mapToScene(view->mapFromGlobal(QCursor::pos()));
}

void Scene::addIcon(PixmapIcon* icon, QPointF pos)
{
    icon->setPos(pos);
    addItem(icon);
    nodeIcons[icon->node_id] = icon;
}

void Scene::addLink(LinkIcon* licon)
{
    licon->draw();
    addItem(licon);
    linkIcons[licon->link_id] = licon;
}

void Scene::onNodeMoved(unsigned id)
{
    for (auto& [lid, licon] : linkIcons) {
        auto* lconf = schema->findLink(lid);
        if (lconf && (lconf->from_id == id || lconf->to_id == id))
            licon->updatePosition();
    }
}

void Scene::onNodeDoubleClicked(unsigned id)
{
    if (!configWindows.count(id)) {
        if (auto* m = schema->findMachine(id))
            configWindows[id] = new MachineConfigurationWindow(m);
        else if (auto* s = schema->findSwitch(id))
            configWindows[id] = new SwitchConfigurationWindow(s);
        else if (auto* ms = schema->findMachineSet(id))
            configWindows[id] = new MachineSetConfigurationWindow(ms);
    }
    if (configWindows.count(id))
        configWindows[id]->show();
}

void Scene::onLinkDoubleClicked(unsigned id)
{
    if (!linkConfigWindows.count(id)) {
        if (auto* lconf = schema->findLink(id))
            linkConfigWindows[id] = new LinkConfigurationWindow(lconf);
    }
    if (linkConfigWindows.count(id))
        linkConfigWindows[id]->show();
}

unsigned Scene::whichNode(QPointF pos)
{
    for (auto& [id, icon] : nodeIcons)
        if (icon->sceneBoundingRect().contains(pos))
            return id;
    return 0;
}

void Scene::deleteItems()
{
    std::vector<unsigned> nodesToRemove;
    for (auto& [id, icon] : nodeIcons)
        if (icon->isChosen()) nodesToRemove.push_back(id);

    for (unsigned id : nodesToRemove) {
        std::vector<unsigned> linkedLinks;
        for (auto& [lid, licon] : linkIcons) {
            auto* lconf = schema->findLink(lid);
            if (lconf && (lconf->from_id == id || lconf->to_id == id))
                linkedLinks.push_back(lid);
        }
        for (unsigned lid : linkedLinks) {
            removeItem(linkIcons[lid]);
            delete linkIcons[lid];
            linkIcons.erase(lid);
            linkConfigWindows.erase(lid);
        }
        removeItem(nodeIcons[id]);
        delete nodeIcons[id];
        nodeIcons.erase(id);
        configWindows.erase(id);
        schema->removeNode(id);
    }

    std::vector<unsigned> linksToRemove;
    for (auto& [id, licon] : linkIcons)
        if (licon->isChosen()) linksToRemove.push_back(id);

    for (unsigned id : linksToRemove) {
        auto it = linkIcons.find(id);
        if (it == linkIcons.end()) continue;
        removeItem(it->second);
        delete it->second;
        linkIcons.erase(it);
        linkConfigWindows.erase(id);
        schema->removeLink(id);
    }

    if (!nodeIcons.count(lBegin_id))
        lBegin_id = 0;
}

void Scene::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Delete:
        deleteItems();
        break;
    case Qt::Key_C:
        clipboard_id = whichNode(getScenePosition());
        break;
    case Qt::Key_V: {
        if (clipboard_id == 0) break;
        QPointF pos = getScenePosition();
        unsigned new_id = 0;
        const char* path    = machinePath;
        const char* pathSel = machinePathSelected;

        if (auto* m = schema->findMachine(clipboard_id)) {
            MachineConf copy = *m;
            new_id = schema->addMachine(copy.name);
            auto* newM = schema->findMachine(new_id);
            *newM = copy;
            newM->id = new_id;
            path = machinePath; pathSel = machinePathSelected;
        } else if (auto* s = schema->findSwitch(clipboard_id)) {
            SwitchConf copy = *s;
            new_id = schema->addSwitch(copy.name);
            auto* newS = schema->findSwitch(new_id);
            *newS = copy;
            newS->id = new_id;
            path = switchPath; pathSel = switchPathSelected;
        } else if (auto* ms = schema->findMachineSet(clipboard_id)) {
            MachineSetConf copy = *ms;
            new_id = schema->addMachineSet(copy.name);
            auto* newMs = schema->findMachineSet(new_id);
            *newMs = copy;
            newMs->id = new_id;
            path = setPath; pathSel = setPathSelected;
        }

        if (new_id != 0)
            addIcon(new PixmapIcon(new_id, PixmapPair(path, pathSel)), pos);
        break;
    }
    default:
        break;
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    switch (pickOp) {
    case NONE: {
        QRectF allItems;
        for (auto& [id, icon] : nodeIcons)  allItems |= icon->sceneBoundingRect();
        for (auto& [id, licon] : linkIcons) allItems |= licon->sceneBoundingRect();
        if (allItems.contains(event->scenePos()))
            QGraphicsScene::mousePressEvent(event);
        else
            selectionArea(event);
        return;
    }
    case PC: {
        auto* icon = table->addMachine();
        addIcon(icon, event->scenePos());
        break;
    }
    case SET: {
        auto* icon = table->addSet();
        addIcon(icon, event->scenePos());
        break;
    }
    case SWITCH: {
        auto* icon = table->addSwitch();
        addIcon(icon, event->scenePos());
        break;
    }
    case SCHEMA:
        break;
    case LINK: {
        unsigned id = whichNode(event->scenePos());
        if (id == 0) return;
        if (lBegin_id == 0) {
            lBegin_id = id;
        } else {
            if (id == lBegin_id) break;
            auto beginIt = nodeIcons.find(lBegin_id);
            auto endIt   = nodeIcons.find(id);
            if (beginIt != nodeIcons.end() && endIt != nodeIcons.end()) {
                unsigned link_id = table->addLink(lBegin_id, id);
                addLink(new LinkIcon(link_id, beginIt->second, endIt->second));
            }
            lBegin_id = 0;
        }
        break;
    }
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (startSelection != QPointF() && selectionRect)
        selectionRect->setRect(QRectF(startSelection, event->scenePos()).normalized());
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && startSelection != QPointF()) {
        QRectF area = QRectF(startSelection, event->scenePos()).normalized();
        for (auto& [id, icon]  : nodeIcons)  icon->toggleChosenIfInside(area);
        for (auto& [id, licon] : linkIcons)  licon->toggleChosenIfInside(area);
        startSelection = QPointF();
        delete selectionRect;
        selectionRect = nullptr;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::selectionArea(QGraphicsSceneMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    startSelection = event->scenePos();
    selectionRect  = new QGraphicsRectItem();
    selectionRect->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    selectionRect->setBrush(QBrush(QColor(100, 100, 255, 40)));
    selectionRect->setRect(QRectF(startSelection, event->scenePos()).normalized());
    addItem(selectionRect);
}

void Scene::drawBackgroundLines()
{
    QRectF rect = sceneRect();
    QPen   pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(QColor(211, 211, 211, 255));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    for (qreal i = 40; i <= rect.height(); i += 40) {
        auto* l = new QGraphicsLineItem(0, i, rect.width(), i);
        l->setPen(pen); l->setZValue(-2); addItem(l);
    }
    for (qreal i = 40; i <= rect.width(); i += 40) {
        auto* l = new QGraphicsLineItem(i, 0, i, rect.height());
        l->setPen(pen); l->setZValue(-2); addItem(l);
    }
}
