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

#include <math.h>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

static const double Pi = 3.14159265358979323846264338327950288419717;
static const double TwoPi = 2.0 * Pi;

GraphEdge::GraphEdge(GraphTab *graphWidget, QList<GraphNode *> sourceNodes,
                     GraphNode *destNode, edge_type _kind)
    : graph(graphWidget), arrowSize(10), kind(_kind)
{
    setFlags(ItemIsSelectable);
    if (kind == EDGE_TYPE_CONNECTION)
        setAcceptHoverEvents(true);
//        setAcceptedMouseButtons(Qt::LeftButton);
    sources = sourceNodes;
    dest = destNode;
    foreach (GraphNode *s, sources) {
        printf("adding source to edge\n");
        s->addEdge(this);
        sourcePoints.append(QPointF(0, 0));
    }

    dest->addEdge(this);
    adjust();
    selected = false;
}

GraphEdge::~GraphEdge()
{
    // need to remove self from nodes
//    sourceNode()->removeEdge(this);
//    destNode()->removeEdge(this);
}

QList<GraphNode*> GraphEdge::sourceNodes() const
{
    return sources;
}

GraphNode* GraphEdge::destNode() const
{
    return dest;
}

void GraphEdge::adjust()
{
    printf("WE habe %d sourcePoints\n", sourcePoints.count());
    if (!sources.count() || !sourcePoints.count() || !dest)
        return;

    for (int i = 0; i < sources.count(); i++) {
        printf("surce index %d\n", i);
        GraphNode* s = sources[i];
        QLineF line(mapFromItem(s, 0, 0), mapFromItem(dest, 0, 0));
        qreal length = line.length();

        prepareGeometryChange();

        if (length > qreal(20.)) {
            const QPointF p = line.p1() + QPointF(line.dx() * s->radius / length,
                                                  line.dy() * s->radius / length);
            sourcePoints[i] = p;
            destPoint = line.p2() - QPointF(line.dx() * dest->radius / length,
                                            line.dy() * dest->radius / length);
        } else {
            sourcePoints[i] = destPoint = line.p1();
        }
    }
}

QRectF GraphEdge::boundingRect() const
{
    if (!sources.count() || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoints[0], QSizeF(destPoint.x() - sourcePoints[0].x(),
                                          destPoint.y() - sourcePoints[0].y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!sources.count() || !dest)
        return;

    QLineF line(QPointF(), destPoint);
    foreach (QPointF p, sourcePoints) {
        line.setP1(p);
        if (qFuzzyCompare(line.length(), qreal(0.)))
            continue;

        // Draw the line itself
        if (selected)
            painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        else
            painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(line);
    }

    if (kind != EDGE_TYPE_CONNECTION)
        return;

    // Draw the arrow
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    if (selected)
        painter->setBrush(Qt::red);
    else
        painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

//void GraphEdge::mousePressEvent(QGraphicsSceneMouseEvent *event)
void GraphEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (selected)
        selected = false;
    else {
        graph->unselectNodes();
        if (event->modifiers() ^ Qt::ShiftModifier) {
            graph->unselectEdges();
        }
        selected = true;
    }
    update();
//    QGraphicsItem::mousePressEvent(event);
}

