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

#include "graphedge.h"
#include "graphnode.h"
#include "graphtab.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <string.h>
#include <math.h>

#define MAX_DISPLACEMENT_SQUARED 100

GraphNode::GraphNode(GraphTab *graphWidget, const char *_name)
    : graph(graphWidget)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    name = strdup(_name);
    label = new QGraphicsTextItem(name, this);
    label->setPos(20, -30);
}

GraphNode::~GraphNode()
{
    // need to remove connected edges
    foreach (GraphEdge *edge, edgeList) {
        if (edge->sourceNode() == this)
            edge->destNode()->removeEdge(edge);
        else
            edge->sourceNode()->removeEdge(edge);
    }

    if (name)
        free((void*)name);
}

void GraphNode::addEdge(GraphEdge *edge)
{
    edgeList << edge;
    edge->adjust();
}

void GraphNode::removeEdge(GraphEdge *edge)
{
    edgeList.removeOne(edge);
}

QList<GraphEdge *> GraphNode::edges() const
{
    return edgeList;
}

void GraphNode::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    // constants
    qreal Kr = 90000;          // repulsion constant
    qreal Ks = 3;          // string constant
    qreal L = 200;           // spring rest length
    qreal delta_t = 0.050;  // 50 ms timestep

    // Repulsion forces
    qreal fx = 0;
    qreal fy = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        GraphNode *node = qgraphicsitem_cast<GraphNode *>(item);
        if (!node || node == this)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        if (dx != 0 || dy != 0) {
            double distanceSquared = dx*dx + dy*dy;
            double distance = sqrt(distanceSquared);
            double force = Kr / distanceSquared;
            fx += force * dx / distance;
            fy += force * dy / distance;
        }
        else {
            // add a small random force
            fx += rand() % 100;
            fy += rand() % 100;
        }
    }

    // Attaction forces from edges
    foreach (GraphEdge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        if (dx != 0 || dy != 0) {
            double distance = sqrt(dx * dx + dy * dy);
            double force = Ks * (distance - L);
            fx -= force * dx / distance;
            fy -= force * dy / distance;
        }
    }

    double dx = fx * delta_t;
    double dy = fy * delta_t;
//    double displacementSquared = dx*dx + dy*dy;
//    if (displacementSquared > MAX_DISPLACEMENT_SQUARED) {
//        double s = sqrt(MAX_DISPLACEMENT_SQUARED / displacementSquared);
//        dx *= s;
//        dy *= s;
//    }

    if (qAbs(dx) < 0.1 && qAbs(dy) < 0.1)
        dx = dy = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(dx, dy);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

bool GraphNode::tick()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF GraphNode::boundingRect() const
{
    double adjust = 2;
    return QRectF( -25 - adjust, -25 - adjust, 52 + adjust, 52 + adjust);
}

QPainterPath GraphNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-25, -25, 50, 50);
    return path;
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(QPen(Qt::black, 2));
//    painter->setBrush(Qt::red);
    painter->drawEllipse(-25, -25, 50, 50);
//    painter->drawText(QRect(pos().x), QString("foo"));
}

QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (GraphEdge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
