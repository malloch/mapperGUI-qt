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

GraphNode::GraphNode(GraphTab *graphWidget, std::string _name,
                     uint32_t _hash, GraphNode *_parent)
    : graph(graphWidget)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    name = _name;
    hash = _hash;
    parent = _parent;
    label = new QGraphicsTextItem(name.c_str(), this);
    label->setPos(20, -30);
    int quadrant = rand() % 3;
    QPoint position;
    switch (quadrant) {
    case 0:
        position = QPoint(graph->width() + 30, rand() % graph->height());
        break;
    case 1:
        position = QPoint(rand() % graph->width(), -30);
        break;
    default:
        position = QPoint(rand() % graph->width(), graph->height() + 30);
        break;
    }
    setPos(position);
    velocity = QPoint(0, 0);
    selected = false;
    fixed = false;
    radius = parent ? 15 : 25;
}

GraphNode::~GraphNode()
{
    // need to remove connected edges
//    foreach (GraphEdge *edge, edgeList) {
//        if (edge->sourceNode() == this)
//            edge->destNode()->removeEdge(edge);
//        else
//            edge->sourceNode()->removeEdge(edge);
//    }
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
        velocity = QPointF(0, 0);
        return;
    }

    // constants
    qreal Kr = 90000;          // repulsion constant
    qreal Ks = 1;          // spring constant
    qreal L = 150;           // spring rest length
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
            fx += rand() % 2;
            fy += rand() % 2;
        }
    }

    // Attraction forces from edges
    foreach (GraphEdge *edge, edgeList) {
        qreal L2 = edge->kind == EDGE_TYPE_CONNECTION ? L : L * 0.5;
        QPointF vec = {0, 0};
        if (edge->destNode() == this) {
            foreach (GraphNode *s, edge->sourceNodes())
                vec += mapToItem(s, 0, 0);
        }
        else
            vec = mapToItem(edge->destNode(), 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        if (dx != 0 || dy != 0) {
            double distance = sqrt(dx * dx + dy * dy);
            double force = Ks * (distance - L2);
            fx -= force * dx / distance;
            fy -= force * dy / distance;
        }
    }

    // Attraction from centre or left window margin
    if (fixed == true) {
//        fx -= (vec.x() - scene()->sceneRect().left() + 100) * 0.01;
        // TODO: disallow drifting into staging area
        fx = fy = 0;
    }
    else {
        QPointF vec = mapToScene(pos());
        fx -= (vec.x() - scene()->width() * 0.5) * 0.01;
        fy -= (vec.y() - scene()->height() * 0.5) * 0.01;
    }

    double dx = fx * delta_t;
    double dy = fy * delta_t;
//    double displacementSquared = dx*dx + dy*dy;
//    if (displacementSquared > MAX_DISPLACEMENT_SQUARED) {
//        double s = sqrt(MAX_DISPLACEMENT_SQUARED / displacementSquared);
//        dx *= s;
//        dy *= s;
//    }

    if (qAbs(dx) < 0.1)
        dx = 0;
    if (qAbs(dy) < 0.1)
        dy = 0;

    velocity += QPointF(dx, dy);
}

bool GraphNode::tick()
{
    if (velocity.x() == 0 && velocity.y() == 0)
        return false;

    QPointF *newPos = new QPointF(pos() + velocity);
    QRectF sceneRect = scene()->sceneRect();
    newPos->setX(qMin(qMax(newPos->x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos->setY(qMin(qMax(newPos->y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
    setPos(pos() + velocity);
    velocity *= 0.8;
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
    path.addEllipse(QPointF(0, 0), radius, radius);
    return path;
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (selected)
        painter->setPen(QPen(Qt::red, 2));
    else
        painter->setPen(QPen(Qt::black, 2));
    if (fixed)
        painter->setBrush(Qt::lightGray);
    painter->drawEllipse(QPointF(0, 0), radius, radius);
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
    if (selected)
        selected = false;
    else {
        graph->unselectEdges();
        if (event->modifiers() ^ Qt::ShiftModifier) {
            graph->unselectNodes();
        }
        selected = true;
    }
    update();
//    QGraphicsItem::mousePressEvent(event);
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (pos().x()-scene()->sceneRect().left() < 50)
        fixed = true;
    else
        fixed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

