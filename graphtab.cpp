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

GraphTab::GraphTab(QTabWidget *parent, mapperGUIData _data)
    : Tab(parent, _data)
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
    data = _data;

    // add current devices
    mapper_db_device *pdev = mapper_db_get_all_devices(data->db);
    while (pdev) {
        scene->addItem(new GraphNode(this, (*pdev)->name));
        pdev = mapper_db_device_next(pdev);
    }

    needsUpdate = true;
}

GraphTab::~GraphTab()
{
    delete scene;
    delete view;
}

void GraphTab::deviceEvent()
{
    printf("DEVICE EVENT (graph)\n");
    if (data->deviceFlags & (1 << MDB_NEW)) {
        // need to add new nodes
        mapper_db_device *pdev = mapper_db_get_all_devices(data->db);
        while (pdev) {
            // check if we already know this node
            bool found = false;
            foreach (QGraphicsItem *item, scene->items()) {
                if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                    if (strcmp((*pdev)->name, node->name) == 0) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found)
                scene->addItem(new GraphNode(this, (*pdev)->name));
            pdev = mapper_db_device_next(pdev);
        }
    }
    if (data->deviceFlags & (1 << MDB_REMOVE)) {
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                // check if device exists in db
                mapper_db_device dev = mapper_db_get_device_by_name(data->db, node->name);
                if (!dev) {
                    scene->removeItem(node);
                    delete node;
                }
            }
        }
    }
}

void GraphTab::linkEvent()
{
    if (data->linkFlags & (1 << MDB_NEW)) {
        // need to add new nodes
        mapper_db_link *plnk = mapper_db_get_all_links(data->db);
        while (plnk) {
            bool exists = false;
            // check if link already exists
            foreach (QGraphicsItem *item, scene->items()) {
                if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
                    if ((strcmp((*plnk)->src_name, edge->sourceNode()->name) == 0)
                            && (strcmp((*plnk)->dest_name, edge->destNode()->name) == 0)) {
                        // link already exists
                        exists = true;
                        break;
                    }
                }
                if (exists)
                    break;
            }
            if (!exists) {
                GraphNode *src = 0, *dest = 0;
                // find source node
                foreach (QGraphicsItem *item, scene->items()) {
                    if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                        if (strcmp((*plnk)->src_name, node->name) == 0) {
                            src = node;
                            break;
                        }
                    }
                }
                if (!src) {
                    plnk = mapper_db_link_next(plnk);
                    continue;
                }
                // find destination node
                foreach (QGraphicsItem *item, scene->items()) {
                    if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) {
                        if (strcmp((*plnk)->dest_name, node->name) == 0) {
                            dest = node;
                            break;
                        }
                    }
                }
                if (dest)
                    scene->addItem(new GraphEdge(this, src, dest));
            }
            plnk = mapper_db_link_next(plnk);
        }
    }
    if (data->linkFlags & (1 << MDB_REMOVE)) {
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
                // check if link exists in db
                mapper_db_link lnk = mapper_db_get_link_by_src_dest_names(data->db,
                                                                          edge->sourceNode()->name, edge->destNode()->name);
                if (!lnk) {
                    edge->sourceNode()->removeEdge(edge);
                    edge->destNode()->removeEdge(edge);
                    scene->removeItem(edge);
                }
            }
        }
    }
}

void GraphTab::itemMoved()
{
    needsUpdate = true;
}

void GraphTab::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_L:
    case Qt::Key_U:
    {
        QList<GraphNode *> nodes;
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item))
                nodes << node;
        }
        foreach (GraphNode *node1, nodes) {
            if (node1->selected) {
                foreach (GraphNode *node2, nodes) {
                    if (node2 != node1 && node2->selected) {
                        if (event->key() == Qt::Key_L)
                            mapper_monitor_link(data->monitor, node1->name, node2->name, 0, 0);
                        else
                            mapper_monitor_unlink(data->monitor, node1->name, node2->name);
                    }
                }
            }
        }
    }
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
    {
        foreach (QGraphicsItem *item, scene->items()) {
            if (GraphEdge *edge = qgraphicsitem_cast<GraphEdge *>(item)) {
                if (edge->selected)
                    mapper_monitor_unlink(data->monitor, edge->sourceNode()->name, edge->destNode()->name);
            }
        }
    }
        break;
    default:
        ;
    }
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
