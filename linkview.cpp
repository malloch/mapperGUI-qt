#include "linkview.h"
#include "ui_linkview.h"
#include "iostream"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneEvent>
#include "qmath.h"

LinkView::LinkView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkView)
{
    ui->setupUi(this);
    QRect rect = this->rect();
    scene = new QGraphicsScene(rect.left(), rect.top(), rect.width()-2, rect.height()-50);
    ui->links->setScene(scene);
    ui->links->setDragMode(QGraphicsView::NoDrag);
    ui->links->setRenderHint(QPainter::Antialiasing);
    ui->links->setRenderHint(QPainter::HighQualityAntialiasing);

    ui->links->scene()->installEventFilter(this);
}

LinkView::~LinkView()
{
    delete ui;
}

void LinkView::clear()
{
    scene->clear();
}

void LinkView::addLink(uint32_t hash, QPointF src, QPointF dst, bool muted)
{
    float width = this->rect().width() - 2;
    src.setX(src.x() * width);
    dst.setX(dst.x() * width);

    // check if link exists
    Edge *link = 0;
    foreach (QGraphicsItem *item, scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->hash == hash) {
                link = edge;
                edge->muted = muted;
                break;
            }
        }
    }
    if (link) {
        link->resize(src, dst);
    }
    else {
        scene->addItem(new Edge(hash, src, dst, muted));
    }
    scene->update();
}

void LinkView::removeLink(uint32_t hash)
{
    printf("LinkView::removeLink\n");
    foreach (QGraphicsItem *item, scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->hash == hash) {
                scene->removeItem(edge);
                break;
            }
        }
    }
    scene->update();
}

void LinkView::resize()
{
    QRect rect = this->rect();
    ui->links->setSceneRect(rect.left(), rect.top(), rect.width()-2, rect.height()-50);
    foreach (QGraphicsItem *item, scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->src.x())
                edge->src.setX(rect.width());
            if (edge->dst.x())
                edge->dst.setX(rect.width());
        }
    }
}

void LinkView::clearSelected()
{
    int updated = 0;
    foreach (QGraphicsItem *item, scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->selected) {
                updated++;
                edge->selected = 0;
            }
        }
    }
    if (updated)
        scene->update();
    selectedMapHashes.clear();
    selectedMaps(selectedMapHashes);
}

void LinkView::updateSelected(const QPointF &newPos)
{
    int updated = 0;

    float length = QPointF(newPos-lastPos).manhattanLength();
    if (length < 2) {
        // skip for efficiency
        return;
    }
    length *= 0.75;

    lastPos = (lastPos+newPos) * 0.5;

    float width = this->rect().width() - 2;
    float xratio = (lastPos.x() / width);

    // need to check for intersections between map bezier curves and linesegment
    /* Since we have a consistent structure to our map curves we can simplify
     * this task somewhat instead of finding arbitrary bezier intersections.*/

    // we will approximate shapes with cosine since it is easy to precompute
    float yratioSrcLeft = cosf(xratio * M_PI + M_PI) * 0.5 + 0.5;
    float yratioSrcRight = 1 - yratioSrcLeft;

    // for left->left and right->right links we will approximate with a circle
    float yratioBothLeft = sqrt(1 - pow(xratio * 2.56410256410256, 2)) * 0.5;
    float yratioBothRight = sqrt(1 -  pow((1-xratio) * 2.56410256410256, 2)) * 0.5;

    foreach (QGraphicsItem *item, scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->selected)
                continue;
            if (edge->src.x() == edge->dst.x()) {
                // two possible points of intersection for each x
                if (edge->src.x() > 0) {
                    // right->right
                    if (xratio < 0.61)
                        continue;
                    float midpoint = (edge->dst.y() + edge->src.y()) * 0.5;
                    float ydist = fabs(edge->dst.y() - edge->src.y()) * yratioBothRight;
                    float cursordist = fabs(midpoint - lastPos.y());
                    if (fabs(cursordist - ydist) > length)
                        continue;
                }
                else {
                    // left->left
                    if (xratio > 0.39)
                        continue;
                    float midpoint = (edge->dst.y() + edge->src.y()) * 0.5;
                    float ydist = fabs(edge->dst.y() - edge->src.y()) * yratioBothLeft;
                    float cursordist = fabs(midpoint - lastPos.y());
                    if (fabs(cursordist - ydist) > length)
                        continue;
                }
            }
            else if (edge->dst.x() > 0) {
                // left->right
                float y = edge->src.y() + (edge->dst.y() - edge->src.y()) * yratioSrcLeft;
                if (fabs(y-lastPos.y()) > length)
                    continue;
            }
            else {
                // right->left
                float y = edge->src.y() + (edge->dst.y() - edge->src.y()) * yratioSrcRight;
                if (fabs(y-lastPos.y()) > length)
                    continue;
            }
            updated++;
            edge->selected = true;
            selectedMapHashes << edge->hash;
            // send qt SIGNAL to connection props display
            selectedMaps(selectedMapHashes);
        }
    }
    if (updated)
        scene->update();
    lastPos = newPos;
}

bool LinkView::eventFilter(QObject *object, QEvent *event)
{
    if (object != ui->links->scene())
        return QObject::eventFilter(object, event);

    QGraphicsSceneMouseEvent *cast = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if (!cast)
        return QObject::eventFilter(object, event);

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress: {
        if (!(cast->modifiers() & Qt::ShiftModifier)) {
            clearSelected();
            // send qt SIGNAL to connection props display
            selectedMaps(selectedMapHashes);
        }
        lastPos = cast->scenePos();
        break;
    }
    case QEvent::GraphicsSceneMouseMove:
        updateSelected(cast->scenePos());
        break;
    case QEvent::GraphicsSceneMouseRelease:
        if (!selectedMapHashes.isEmpty()) {
            ;
        }
        break;
    default:
        return QObject::eventFilter(object, event);
    }
    event->setAccepted(true);
    return true;
}

QRectF LinkView::Edge::boundingRect() const
{
    // TODO: move back to normalized bounding rect to handle src.x() == dst.x()
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(src, QSizeF(dst.x() - src.x(), dst.y() - src.y()))
//        .normalized()
        .adjusted(0, -extra, 0, extra);
}

void LinkView::Edge::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    QPainterPath *path = new QPainterPath();
    QColor color(selected * 255, 0, 0, 255);
    QPen pen(color, 2, muted ? Qt::DashLine : Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    float halfwidth = (widget->width() - 2) * 0.5;

    path->moveTo(src.x(), src.y());
    path->cubicTo(halfwidth, src.y(),
                  halfwidth, dst.y(),
                  dst.x(), dst.y());

    painter->setPen(pen);
    painter->drawPath(*path);

    // Draw the arrow
    QPointF endPoint, arrowP1, arrowP2;
    endPoint = QPointF(dst.x(), dst.y());
    arrowP1 = endPoint + QPointF(dst.x() ? -10 : 10, 4);
    arrowP2 = endPoint + QPointF(dst.x() ? -10 : 10, -4);

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->setBrush(color);
    painter->drawPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2);
}
