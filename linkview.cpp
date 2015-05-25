#include "linkview.h"
#include "ui_linkview.h"
#include "iostream"
#include <QPainter>

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
    printf("LinkView::addLink\n");

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
    else
        scene->addItem(new Edge(hash, src, dst));
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
}

QRectF LinkView::Edge::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(src, QSizeF(dst.x() - src.x(), dst.y() - src.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void LinkView::Edge::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    printf("LinkView::Edge::paint\n");
    QPainterPath *path = new QPainterPath();

    float width = widget->width()-2;
    float halfwidth = width * 0.5;

    path->moveTo(src.x() * width, src.y());
    path->cubicTo(halfwidth, src.y(),
                  halfwidth, dst.y(),
                  dst.x() * width, dst.y());

    if (isSelected())
        painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    else
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(*path);

    // Draw the arrow
    QPointF endPoint, arrowP1, arrowP2;
    endPoint = QPointF(dst.x() * width, dst.y());
    arrowP1 = endPoint + QPointF(dst.x() * -10, 4);
    arrowP2 = endPoint + QPointF(dst.x() * -10, -4);

    if (isSelected())
        painter->setBrush(Qt::red);
    else
        painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2);
}
