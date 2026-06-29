#include "window/drawingtable/scene.h"
#include "icon/linkicon.h"
#include "utils/iconPath.h"
#include "window/drawingtable/drawingtable.h"
#include "window/linkconfigurationwindow.h"
#include "window/machineconfigurationwindow.h"
#include "window/machinesetconfigurationwindow.h"
#include "window/schemaconfigurationwindow.h"
#include "window/schemawindow.h"
#include "window/switchconfigurationwindow.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPen>
#include <cstdio>

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
    layout[icon->node_id] = pos;
}

void Scene::addLink(LinkIcon* licon)
{
    licon->draw();
    addItem(licon);
    linkIcons[licon->link_id] = licon;
}

void Scene::onNodeClicked(unsigned id)
{
    emit nodeSelected(id);
}

void Scene::onNodeMoved(unsigned id)
{
    auto it = nodeIcons.find(id);
    if (it != nodeIcons.end())
        layout[id] = it->second->pos();

    for (auto& [lid, licon] : linkIcons) {
        auto* lconf = schema->findLink(lid);
        if (lconf && (lconf->from_id == id || lconf->to_id == id))
            licon->updatePosition();
    }
}

void Scene::onNodeDoubleClicked(unsigned id)
{
    if (!configWindows.count(id)) {
        if (id == schema->output_id)
            configWindows[id] = new SchemaConfigurationWindow(schema);
        else if (schema->findMachine(id))
            configWindows[id] = new MachineConfigurationWindow(schema, id);
        else if (schema->findSwitch(id))
            configWindows[id] = new SwitchConfigurationWindow(schema, id);
        else if (schema->findMachineSet(id))
            configWindows[id] = new MachineSetConfigurationWindow(schema, id);
        else if (auto* sub = schema->findSchema(id))
            configWindows[id] = new SchemaWindow(sub);
    }
    if (configWindows.count(id))
        configWindows[id]->show();
}

void Scene::onLinkDoubleClicked(unsigned id)
{
    if (!linkConfigWindows.count(id)) {
        if (schema->findLink(id))
            linkConfigWindows[id] = new LinkConfigurationWindow(schema, id);
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
            auto lcwIt = linkConfigWindows.find(lid);
            if (lcwIt != linkConfigWindows.end()) {
                delete lcwIt->second;
                linkConfigWindows.erase(lcwIt);
            }
        }
        removeItem(nodeIcons[id]);
        delete nodeIcons[id];
        nodeIcons.erase(id);
        auto cwIt = configWindows.find(id);
        if (cwIt != configWindows.end()) {
            delete cwIt->second;
            configWindows.erase(cwIt);
        }
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
        auto lcwIt = linkConfigWindows.find(id);
        if (lcwIt != linkConfigWindows.end()) {
            delete lcwIt->second;
            linkConfigWindows.erase(lcwIt);
        }
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
        printf("Node Id: %d\n", clipboard_id);
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
        else if (auto *sm = schema->findSchema(clipboard_id)) {
            Schema cloned = clone_schema(*sm, schema->next_id);
            new_id = cloned.id;

            std::map<unsigned, QPointF> remappedLayout;
            if (configWindows.count(clipboard_id)) {
                auto *srcWin =
                    dynamic_cast<SchemaWindow *>(configWindows[clipboard_id]);
                if (srcWin) {
                    auto &srcLayout = srcWin->drawingTable->getScene()->layout;
                    // reconstruct id_map by zipping original and cloned vectors
                    // (same order)
                    std::map<unsigned, unsigned> id_map;
                    for (size_t i = 0; i < sm->machines.size(); i++)
                        id_map[sm->machines[i].id] = cloned.machines[i].id;
                    for (size_t i = 0; i < sm->switches.size(); i++)
                        id_map[sm->switches[i].id] = cloned.switches[i].id;
                    for (size_t i = 0; i < sm->machineSets.size(); i++)
                        id_map[sm->machineSets[i].id] =
                            cloned.machineSets[i].id;
                    for (size_t i = 0; i < sm->schemas.size(); i++)
                        id_map[sm->schemas[i].id] = cloned.schemas[i].id;
                    for (auto &[old_id, p] : srcLayout)
                        if (id_map.count(old_id))
                            remappedLayout[id_map[old_id]] = p;
                }
            }

            schema->schemas.push_back(std::move(cloned));
            auto* sub = schema->findSchema(new_id);
            configWindows[new_id] = new SchemaWindow(sub, std::move(remappedLayout));
            path = schemaPath; pathSel = schemaPathSelected;
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
    case SCHEMA: {
        auto *icon = table->addSchema();
        addIcon(icon, event->scenePos());
        break;
    }
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
                auto resolveEndpoint = [&](unsigned icon_id) -> unsigned {
                    auto* sub = schema->findSchema(icon_id);
                    return (sub && sub->output_id) ? sub->output_id : icon_id;
                };
                unsigned link_id = table->addLink(resolveEndpoint(lBegin_id),
                                                  resolveEndpoint(id));
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

void Scene::populate()
{
    int col = 0, row = 0;
    auto pos = [&](unsigned id) -> QPointF {
        auto it = layout.find(id);
        if (it != layout.end()) return it->second;
        if (id == schema->output_id) return QPointF(960, 540);
        QPointF p(80 + col * 120, 80 + row * 120);
        if (++col >= 5) { col = 0; ++row; }
        return p;
    };

    for (auto& m  : schema->machines)
        addIcon(new PixmapIcon(m.id,  PixmapPair(machinePath,    machinePathSelected)),  pos(m.id));
    for (auto& s  : schema->switches)
        addIcon(new PixmapIcon(s.id,  PixmapPair(switchPath,     switchPathSelected)),   pos(s.id));
    for (auto& ms : schema->machineSets)
        addIcon(new PixmapIcon(ms.id, PixmapPair(setPath,        setPathSelected)),      pos(ms.id));
    for (auto& sub : schema->schemas)
        addIcon(new PixmapIcon(sub.id, PixmapPair(schemaPath,    schemaPathSelected)),   pos(sub.id));
    for (auto& l  : schema->links) {
        auto fromIt = nodeIcons.find(l.from_id);
        auto toIt   = nodeIcons.find(l.to_id);
        if (fromIt != nodeIcons.end() && toIt != nodeIcons.end())
            addLink(new LinkIcon(l.id, fromIt->second, toIt->second));
    }
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
