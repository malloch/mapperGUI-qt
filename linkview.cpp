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

void LinkView::addLink(uint32_t hash, QPointF src, QPointF dst)
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
                break;
            }
        }
    }
    if (link) {
        link->resize(src, dst);
    }
    else {
        scene->addItem(new Edge(hash, src, dst));
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
    // adjust for bezier shape
    float yratio = cosf(xratio * M_PI + M_PI) * 0.5 + 0.5;
    float yratioInv = 1 - yratio;
    float yratioSquashed = cosf(xratio * 0.78 * M_PI + M_PI) * 0.5 + 0.5;
    float yratioSquashedInv = cosf((1 - xratio) * 0.78 * M_PI + M_PI) * 0.5 + 0.5;
    float yratioSame = cosf(xratio * M_PI * 1.28);
    float yratioSameInv = 1 - yratioSame;
//    float coeffP0 = pow((1 - xratio), 3);
//    float coeffP1 = 3 * pow((1 - xratio), 2) * xratio;
//    float coeffP2 = 3 * (1 - xratio) * pow(xratio,2);
//    float coeffP3 = pow(xratio,3);
    qDebug() << "newPos:" << newPos << "lastpos:" << lastPos << "xratio:" << xratio << "yratioSameInv: " << yratioSameInv << "manhattan:"<< length;

    // need to check for intersections between map bezier curves and linesegment
    /* Since we have a consistent structure to our map curves we can simplify
     * this task somewhat instead of finding arbitrary bezier intersections.*/
    // case 1: left -> right
    // case 2: right -> left
    // case 3: left -> left
    // case 4: right -> right

    foreach (QGraphicsItem *item, scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->src.x() == edge->dst.x()) {
                // two possible points of intersection for each x
                float midpoint = (edge->dst.y() + edge->src.y()) * 0.5;
                if (edge->src.x() > 0) {
                    // right->right
                    if (xratio < 0.61)
                        continue;
                    float y = edge->src.y() + (edge->dst.y() - edge->src.y()) * yratioSameInv;
                    float diff = fabs(midpoint - y);
                    float ydiff = fabs(midpoint - lastPos.y());
                    qDebug() << "midpoint:" << midpoint << "y:" << y << "diff:" << diff << "ydiff" << ydiff << "distance" << fabs(diff-ydiff);
                    if (fabs(diff - ydiff) > length)
                        continue;
                }
                else {
                    // left->left
                    if (xratio > 0.39)
                        continue;
                    float y = edge->src.y() + (edge->dst.y() - edge->src.y()) * yratioSame;
                    float diff = fabs(midpoint - y);
                    float ydiff = fabs(midpoint - lastPos.y());
                    if (fabs(diff - ydiff) > length)
                        continue;
                }
            }
            else if (edge->dst.x() > 0) {
                // left->right
                float y = edge->src.y() + (edge->dst.y() - edge->src.y()) * yratio;
                if (fabs(y-lastPos.y()) > length)
                    continue;
            }
            else {
                // right->left
                float y = edge->src.y() + (edge->dst.y() - edge->src.y()) * yratioInv;
                if (fabs(y-lastPos.y()) > length)
                    continue;
            }
            if (!edge->selected) {
                updated++;
                edge->selected = true;
            }
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
//        qDebug() << "QEvent::GraphicsSceneMousePress" << cast->buttonDownScenePos(cast->button());
        clearSelected();
        lastPos = cast->scenePos();
        break;
    }
    case QEvent::GraphicsSceneMouseMove:
//        qDebug() << "QEvent::GraphicsSceneMouseMove" << cast->scenePos();
        updateSelected(cast->scenePos());
        break;
    case QEvent::GraphicsSceneMouseRelease:
//        qDebug() << "QEvent::GraphicsSceneMouseRelease" << cast->scenePos();
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

    float halfwidth = (widget->width() - 2) * 0.5;

    path->moveTo(src.x(), src.y());
    path->cubicTo(halfwidth, src.y(),
                  halfwidth, dst.y(),
                  dst.x(), dst.y());

    if (selected)
        painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    else
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(*path);

    // Draw the arrow
    QPointF endPoint, arrowP1, arrowP2;
    endPoint = QPointF(dst.x(), dst.y());
    arrowP1 = endPoint + QPointF(dst.x() ? -10 : 10, 4);
    arrowP2 = endPoint + QPointF(dst.x() ? -10 : 10, -4);

    if (selected)
        painter->setBrush(Qt::red);
    else
        painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2);
}
