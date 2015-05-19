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

//    // add current devices
//    for (auto const &device : data->monitor.db().devices()) {
//        GraphNode *gdev = new GraphNode(this, device.name(), 0);
//        scene->addItem(gdev);
//        mapper::Db::Signal::Iterator iter = data->monitor.db().inputs(device.name()).begin();
//        for (; iter != data->monitor.db().inputs(device.name()).end(); iter++) {
//            scene->addItem(new GraphNode(this, device.name(), gdev));
//        }
//        iter = data->monitor.db().outputs(device.name()).begin();
//        for (; iter != data->monitor.db().outputs(device.name()).end(); iter++) {
//            scene->addItem(new GraphNode(this, device.name(), gdev));
//        }
//    }

    needsUpdate = true;
}

GraphTab::~GraphTab()
{
    delete scene;
    delete view;
}

void GraphTab::deviceEvent()
{
//    printf("DEVICE EVENT (graph)\n");
//    if (data->deviceFlags & (1 << MDB_NEW)) {
//        // need to add new nodes
//        for (auto const &device : data->monitor.db().devices()) {
//            // check if we already know this node
//            bool found = false;
//            foreach (QGraphicsItem *item, scene->items()) {
//                if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                    if (device.name().compare(node->name) == 0) {
//                        found = true;
//                        break;
//                    }
//                }
//            }
//            if (!found)
//                scene->addItem(new GraphNode(this, device.name(), 0));
//        }
//    }
//    if (data->deviceFlags & (1 << MDB_REMOVE)) {
//        foreach (QGraphicsItem *item, scene->items()) {
//            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                // check if device exists in db
//                mapper::Db::Device dev = data->monitor.db().device(node->name);
//                if (!dev) {
//                    scene->removeItem(node);
//                    delete node;
//                }
//            }
//        }
//    }
}

void GraphTab::signalEvent()
{
//    printf("SIGNAL EVENT (graph)\n");
//    if (data->signalFlags & (1 << MDB_NEW)) {
//        // need to add new nodes
//        for (auto const &signal : data->monitor.db().inputs()) {
//            // check if we already know this node
//            bool found = false;
//            // find parent device first
//            GraphNode *device;
//            foreach (QGraphicsItem *item, scene->items()) {
//                if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                    if (node->parent)
//                        continue;
//                    if (signal.device().name().compare(node->name) == 0) {
//                        found = true;
//                        device = node;
//                        break;
//                    }
//                }
//            }
//            if (!found) {
//                device = new GraphNode(this, signal.device().name(), 0);
//                scene->addItem(device);
//            }

//            found = false;
//            foreach (QGraphicsItem *item, scene->items()) {
//                if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                    if (node->parent)
//                        continue;
//                    if (signal.name().compare(node->name) == 0) {
//                        found = true;
//                        break;
//                    }
//                }
//            }
//            if (!found) {
//                GraphNode *signode = new GraphNode(this, signal.name(), device);
//                scene->addItem(signode);
//                scene->addItem(new GraphEdge(this, device, signode, EDGE_TYPE_SIGNAL));
//            }
//        }
//        for (auto const &signal : data->monitor.db().outputs()) {
//            // check if we already know this node
//            bool found = false;
//            // find parent device first
//            GraphNode *device;
//            foreach (QGraphicsItem *item, scene->items()) {
//                if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                    if (node->parent)
//                        continue;
//                    if (signal.device().name().compare(node->name) == 0) {
//                        found = true;
//                        device = node;
//                        break;
//                    }
//                }
//            }
//            if (!found) {
//                device = new GraphNode(this, signal.device().name(), 0);
//                scene->addItem(device);
//            }
//            found = false;
//            foreach (QGraphicsItem *item, scene->items()) {
//                if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                    if (node->parent)
//                        continue;
//                    if (signal.name().compare(node->name) == 0) {
//                        found = true;
//                        break;
//                    }
//                }
//            }
//            if (!found) {
//                GraphNode *signode = new GraphNode(this, signal.name(), device);
//                scene->addItem(signode);
//                scene->addItem(new GraphEdge(this, device, signode, EDGE_TYPE_SIGNAL));
//            }
//        }
//    }
////    if (data->signalFlags & (1 << MDB_REMOVE)) {
////        foreach (QGraphicsItem *item, scene->items()) {
////            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
////                // check if device exists in db
////                mapper_db_signal sig = mapper_db_get_input_by_name(data->db, node->name);
////                if (!dev) {
////                    scene->removeItem(node);
////                    delete node;
////                }
////            }
////        }
////    }
}

void GraphTab::connectionEvent()
{
//    if (data->connectionFlags & (1 << MDB_NEW)) {
//        // need to add new nodes
//        for (auto const &c : data->monitor.db().connections()) {
//            bool exists = false;
//            // check if connection already exists
//            foreach (QGraphicsItem *item, scene->items()) {
//                if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
//                    if (c.hash() == edge->hash) {
//                        // connection already exists
//                        exists = true;
//                        break;
//                    }
//                }
//                if (exists)
//                    break;
//            }
//            if (!exists) {
//                GraphNode *src = 0, *dest = 0;
//                // find destination node
//                foreach (QGraphicsItem *item, scene->items()) {
//                    if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                        if (c.destination().signal().name().compare(node->name) == 0) {
//                            dest = node;
//                            break;
//                        }
//                    }
//                }
//                if (!dest)
//                    continue;
//                // find source node(s)
//                foreach (QGraphicsItem *item, scene->items()) {
//                    if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
//                        for (int i = 0; i < c.num_sources(); i++) {
//                            if (c.source(i).signal().name().compare(node->name) == 0) {
//                                scene->addItem(new GraphEdge(this, node, dest,
//                                                             EDGE_TYPE_CONNECTION));
//                                break;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
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
