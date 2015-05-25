/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graphtab.h"

#include <math.h>

#include <QGridLayout>
#include <QKeyEvent>
#include <QPainter>

GraphTab::GraphTab(QTabWidget *parent, MapperStuff *_data) :
    Tab(parent, _data)
{
    view = new QGraphicsView();
    view->setDragMode(QGraphicsView::ScrollHandDrag);
//    view->ViewportAnchor(QGraphicsView::AnchorUnderMouse);
    scene = new QGraphicsScene();

//    scene->setSceneRect(view->width()*-0.5, view->height()*-0.5, view->width()*0.5, view->height()*0.5);
    view->setScene(scene);
    QRectF viewRect = view->viewport()->rect();
    QRectF sceneRect = QRectF(QPointF(viewRect.left()-30, viewRect.top()-30),
                              QPointF(viewRect.right()+30,viewRect.bottom()+30));
    view->setSceneRect(sceneRect);

//    float lpos = sceneRect.left() + 200;
//    scene->addLine(lpos, sceneRect.top(), lpos, sceneRect.bottom());

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::HighQualityAntialiasing);
//    view->setTransformationAnchor(AnchorUnderMouse);

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setLayout(layout);
    layout->addWidget(view, 0, 0);

    parent->insertTab(0, this, QString("Overview"));

    numObjects = 0;

    // add current devices
    for (auto const &dev : data->db.devices()) {
        GraphNode *gdev = new GraphNode(this, dev.name(), dev.hash(), 0);
        scene->addItem(gdev);
        mapper::Db::Signal::Iterator iter = data->db.inputs(dev.name()).begin();
        for (; iter != data->db.inputs(dev.name()).end(); iter++) {
            scene->addItem(new GraphNode(this, dev.name(), 0, gdev));
        }
        iter = data->db.outputs(dev.name()).begin();
        for (; iter != data->db.outputs(dev.name()).end(); iter++) {
            scene->addItem(new GraphNode(this, dev.name(), 0, gdev));
        }
    }

    data->addDeviceCallback(this);
    data->addSignalCallback(this);
    data->addMapCallback(this);

    needsUpdate = true;
}

GraphTab::~GraphTab()
{
    delete scene;
    delete view;
}

void GraphTab::deviceEvent(mapper_db_device dev, mapper_db_action_t action)
{
    switch (action) {
    case MDB_NEW:
        scene->addItem(new GraphNode(this, dev->name, dev->hash, 0));
        break;
    case MDB_REMOVE:
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                // check if device exists in db
                if (dev->hash == node->hash) {
                    scene->removeItem(node);
                    delete node;
                    return;
                }
            }
        }
        break;
    default:
        break;
    }
}

void GraphTab::signalEvent(mapper_db_signal sig, mapper_db_action_t action)
{
    if (action == MDB_MODIFY)
        return;

    switch (action) {
    case MDB_NEW: {
        // find parent node
        GraphNode *parent = 0;
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                if (sig->device->hash == node->hash) {
                    parent = node;
                    break;
                }
            }
        }
        if (!parent) {
            printf("error: signal '%s' has no parent node in scene\n", sig->name);
        }
        GraphNode *signode = new GraphNode(this, sig->name, 0, parent);
        scene->addItem(signode);
        scene->addItem(new GraphEdge(this, parent, signode, EDGE_TYPE_SIGNAL));
        break;
    }
    case MDB_REMOVE:
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                if (sig->device->hash == node->hash
                        && strcmp(sig->name, node->name.c_str()) == 0) {
                    scene->removeItem(node);
                    delete node;
                    return;
                }
            }
        }
        break;
    default:
        break;
    }
}

void GraphTab::mapEvent(mapper_db_map map, mapper_db_action_t action)
{
    switch (action) {
    case MDB_NEW: {
        GraphNode *dest = 0;
        // find destination node
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                if (strcmp(map->destination.signal->name, node->name.c_str()) == 0) {
                    dest = node;
                    break;
                }
            }
        }
        if (!dest)
            return;
        // find source node(s)
        QList<GraphNode*> sources;
        for (int i = 0; i < map->num_sources; i++) {
            foreach (QGraphicsItem *item, scene->items()) {
                if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                    if (strcmp(map->sources[i].signal->name, node->name.c_str()) == 0) {
                        sources << node;
                        break;
                    }
                }
            }
        }
        if (sources.count() == map->num_sources)
            scene->addItem(new GraphEdge(this, sources, dest, EDGE_TYPE_CONNECTION));
        break;
    }
    case MDB_REMOVE:
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
                if (edge->type() != EDGE_TYPE_CONNECTION)
                    continue;
                if (map->num_sources != edge->numSources())
                    continue;
                if (strcmp(map->destination.signal->name, edge->destNode()->name.c_str()))
                    continue;
                for (int i = 0; i < map->num_sources; i++) {
                    if (strcmp(map->sources[i].signal->name, edge->destNode()->name.c_str()))
                        continue;
                }
                if (!map) {
                    foreach(GraphNode *s, edge->sourceNodes())
                        s->removeEdge(edge);
                    edge->destNode()->removeEdge(edge);
                    scene->removeItem(edge);
                }
            }
        }
        break;
    default:
        break;
    }


//    if (data->connectionFlags & (1 << MDB_REMOVE)) {
//        foreach (QGraphicsItem *item, scene->items()) {
//            if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
//                // check if link exists in db
//                mapper::Db::Connection con = data->monitor.db().connection(edge->hash);
//                if (!con) {
//                    edge->sourceNode()->removeEdge(edge);
//                    edge->destNode()->removeEdge(edge);
//                    scene->removeItem(edge);
//                }
//            }
//        }
//    }
}


void GraphTab::itemMoved()
{
    needsUpdate = true;
}

void GraphTab::keyPressEvent(QKeyEvent *event)
{
//    switch (event->key()) {
//    case Qt::Key_Plus:
//        zoomIn();
//        break;
//    case Qt::Key_Minus:
//        zoomOut();
//        break;
//    case Qt::Key_L:
//    case Qt::Key_U:
//    {
//        QList<GraphNode *> nodes;
//        foreach (QGraphicsItem *item, scene->items()) {
//            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item))
//                nodes << node;
//        }
//        foreach (GraphNode *node1, nodes) {
//            if (node1->selected) {
//                foreach (GraphNode *node2, nodes) {
//                    if (node2 != node1 && node2->selected) {
//                        if (event->key() == Qt::Key_L)
//                            data->monitor.connect(node1->name, node2->name);
//                        else
//                            data->monitor.disconnect(node1->name, node2->name);
//                    }
//                }
//            }
//        }
//    }
//        break;
//    case Qt::Key_Delete:
//    case Qt::Key_Backspace:
//    {
//        foreach (QGraphicsItem *item, scene->items()) {
//            if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
//                if (edge->selected)
//                    data->monitor.disconnect(edge->sourceNode()->name,
//                                              edge->destNode()->name);
//            }
//        }
//    }
//        break;
//    default:
//        ;
//    }
}

void GraphTab::update()
{
    QList<GraphNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item))
            nodes << node;
    }

    foreach (GraphNode *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (GraphNode *node, nodes) {
        if (node->tick())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        needsUpdate = false;
    }
}

#ifndef QT_NO_WHEELEVENT
void GraphTab::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphTab::scaleView(qreal scaleFactor)
{
    qreal factor = view->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    view->scale(scaleFactor, scaleFactor);
}

void GraphTab::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphTab::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void GraphTab::unselectNodes()
{
    foreach (QGraphicsItem *item, scene->items()) {
        if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
            node->selected = false;
            node->setSelected(false);
            node->update();
        }
    }
}

void GraphTab::unselectEdges()
{
    foreach (QGraphicsItem *item, scene->items()) {
        if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
            edge->selected = false;
            edge->setSelected(false);
            edge->update();
        }
    }
}

void GraphTab::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && (event->modifiers() ^ Qt::ShiftModifier)) {
        // clear selection
        unselectNodes();
        unselectEdges();
    }
}
