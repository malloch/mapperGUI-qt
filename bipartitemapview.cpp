#include "bipartitemapview.h"
#include "ui_bipartitemapview.h"
#include "iostream"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneEvent>
#include "qmath.h"

BipartiteMapView::BipartiteMapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BipartiteMapView)
{
    ui->setupUi(this);
    QRect rect = this->rect();
    scene = new QGraphicsScene(rect.left(), rect.top(), rect.width()-2, rect.height()-50);
    ui->maps->setScene(scene);
    ui->maps->setDragMode(QGraphicsView::NoDrag);
    ui->maps->setRenderHint(QPainter::Antialiasing);
    ui->maps->setRenderHint(QPainter::HighQualityAntialiasing);

    ui->maps->scene()->installEventFilter(this);

    mouseButtonPressed = false;
}

BipartiteMapView::~BipartiteMapView()
{
    delete ui;
}

void BipartiteMapView::clear()
{
    scene->clear();
}

void BipartiteMapView::addMap(qulonglong id, QList<QPointF> srcs, QPointF dst,
                              bool muted)
{
    qDebug() << "adding MAP" << id << "with" << srcs.length() << "sources";
    float width = this->rect().width() - 2;
    for (auto& src : srcs)
        src.setX(src.x() * width);
    dst.setX(dst.x() * width);

    // check if map exists
    Edge *map = 0;
    for (auto const& item : scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->id == id) {
                map = edge;
                edge->muted = muted;
                break;
            }
        }
    }
    if (map) {
        map->resize(srcs, dst);
    }
    else {
        scene->addItem(new Edge(id, srcs, dst, muted));
    }
    scene->update();
}

void BipartiteMapView::removeMap(qulonglong id)
{
    for (auto const& item : scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->id == id) {
                scene->removeItem(edge);
                break;
            }
        }
    }
    scene->update();
}

void BipartiteMapView::resize()
{
    QRect rect = this->rect();
    ui->maps->setSceneRect(rect.left(), rect.top(),
                           rect.width()-2, rect.height()-50);
    for (auto const& item : scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            for (auto& src : edge->srcs) {
                if (src.x())
                    src.setX(rect.width());
            }
            if (edge->dst.x())
                edge->dst.setX(rect.width());
        }
    }
}

void BipartiteMapView::clearSelected()
{
    selectedMapIds.clear();
    int updated = 0;
    for (auto const& item : scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->selected) {
                updated++;
                edge->selected = 0;
            }
        }
    }
    if (updated) {
        scene->update();
        selectedMaps(selectedMapIds);
    }
}

void BipartiteMapView::selectAllMaps()
{
    bool updated = false;
    for (auto const& item : scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (edge->id && !edge->selected) {
                updated = edge->selected = true;
                selectedMapIds << edge->id;
            }
        }
    }
    if (updated) {
        scene->update();
        selectedMaps(selectedMapIds);
    }
}

void BipartiteMapView::updateSelected(const QPointF &newPos)
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

    for (auto const& item : scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (!edge->id || edge->selected)
                continue;
            bool touched = true;
            for (auto const& src : edge->srcs) {
                if (src.x() == edge->dst.x()) {
                    // two possible points of intersection for each x
                    if (src.x() > 0) {
                        // right->right
                        if (xratio < 0.61)
                            touched = false;
                        float midpoint = (edge->dst.y() + src.y()) * 0.5;
                        float ydist = fabs(edge->dst.y() - src.y()) * yratioBothRight;
                        float cursordist = fabs(midpoint - lastPos.y());
                        if (fabs(cursordist - ydist) > length)
                            touched = false;
                    }
                    else {
                        // left->left
                        if (xratio > 0.39)
                            touched = false;
                        float midpoint = (edge->dst.y() + src.y()) * 0.5;
                        float ydist = fabs(edge->dst.y() - src.y()) * yratioBothLeft;
                        float cursordist = fabs(midpoint - lastPos.y());
                        if (fabs(cursordist - ydist) > length)
                            touched = false;
                    }
                }
                else if (edge->dst.x() > 0) {
                    // left->right
                    float y = src.y() + (edge->dst.y() - src.y()) * yratioSrcLeft;
                    if (fabs(y-lastPos.y()) > length)
                        touched = false;
                }
                else {
                    // right->left
                    float y = src.y() + (edge->dst.y() - src.y()) * yratioSrcRight;
                    if (fabs(y-lastPos.y()) > length)
                        touched = false;
                }
                if (touched)
                    continue;
            }
            if (!touched)
                continue;
            updated++;
            edge->selected = true;
            selectedMapIds << edge->id;
        }
    }
    if (updated) {
        // send qt SIGNAL to connection props display
        selectedMaps(selectedMapIds);
        scene->update();
    }
    lastPos = newPos;
}

bool BipartiteMapView::eventFilter(QObject *object, QEvent *event)
{
    if (object != ui->maps->scene()) {
        return QObject::eventFilter(object, event);
    }

    QGraphicsSceneMouseEvent *cast = dynamic_cast<QGraphicsSceneMouseEvent*>(event);

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress: {
        if (!cast)
            break;
        if (!(cast->modifiers() & Qt::ShiftModifier)) {
            clearSelected();
            // send qt SIGNAL to connection props display
            Q_EMIT selectedMaps(selectedMapIds);
        }
        lastPos = cast->scenePos();
        mouseButtonPressed = true;
        break;
    }
    case QEvent::GraphicsSceneMouseMove:
        if (!cast)
            break;
        if (mouseButtonPressed)
            updateSelected(cast->scenePos());
        break;
    case QEvent::GraphicsSceneMouseRelease:
        if (!cast)
            break;
        if (!selectedMapIds.isEmpty()) {
            ;
        }
        mouseButtonPressed = false;
        break;
    case QEvent::KeyPress: {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        switch (key->key()) {
        case Qt::Key_Backspace:
        case Qt::Key_Delete:
            releaseSelectedMaps();
            break;
        case Qt::Key_Space:
            toggleSelectedMapsMuting();
            break;
        case Qt::Key_M:
            Q_EMIT mapSelectedSigs();
            break;
        case Qt::Key_U:
            Q_EMIT unmapSelectedSigs();
            break;
        case Qt::Key_A:
            if (key->modifiers() & (Qt::ControlModifier | Qt::MetaModifier))
                selectAllMaps();
        }
        break;
    }

    default:
        return QObject::eventFilter(object, event);
    }
    event->setAccepted(true);
    return true;
}

QRectF BipartiteMapView::Edge::boundingRect() const
{
    // TODO: move back to normalized bounding rect to handle src.x() == dst.x()
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    float height = 0, width = fabs(srcs[0].x() - dst.x());
    for (auto const& src : srcs) {
        height = fmax(height, fabs(src.y() - dst.y()));
    }
    return QRectF(dst, QSize(width, height))
            //        .normalized()
            .adjusted(0, -extra, 0, extra);
}

void BipartiteMapView::Edge::paint(QPainter *painter,
                                   const QStyleOptionGraphicsItem *option,
                                   QWidget *widget)
{
    QPainterPath *path = new QPainterPath();
    QColor color(selected * 255, 0, 0, 255);
    QPen pen(color, 2, muted ? Qt::DashLine : Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    float halfwidth = (widget->width() - 2) * 0.5;
    float srcX = dst.x();
    for (auto const& src : srcs) {
        srcX += src.x();
    }
    srcX /= (srcs.length() + 1);
//    halfwidth = (halfwidth + srcX) * 0.5;

    int i = 0;
    for (auto const& src : srcs) {
        qDebug() << "drawing source" << i++ << "of" << srcs.length();
        path->moveTo(src.x(), src.y());
        path->cubicTo(halfwidth, src.y(),
                      halfwidth, dst.y(),
                      dst.x(), dst.y());
    }
    bool dir = dst.x() > halfwidth;
    painter->setPen(pen);
    painter->drawPath(*path);

    // Draw the arrow
    QPointF endPoint, arrowP1, arrowP2;
    endPoint = QPointF(dst.x(), dst.y());
    arrowP1 = endPoint + QPointF(dir ? -10 : 10, 4);
    arrowP2 = endPoint + QPointF(dir ? -10 : 10, -4);

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->setBrush(color);
    painter->drawPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2);
}

void BipartiteMapView::drawDrag(QList<QPointF> start, QPointF end)
{
    float width = this->rect().width() - 2;

    // check if already dragging
    Edge *drag = 0;
    for (auto const& item : scene->items()) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            if (!edge->id) {
                drag = edge;
                break;
            }
        }
    }
    if (end.x() > width)
        end.setX(width);
    else if (end.x() < -width)
        end.setX(0);
    else if (end.x() < 0)
        end.setX(width + end.x());

    if (drag) {
        drag->dst.setX(end.x());
        drag->dst.setY(end.y());
    }
    else {
        for (auto& src : start)
            src.setX(src.x() * width);
        scene->addItem(new Edge(0, start, end, false));
    }
    scene->update();
}

int BipartiteMapView::shouldSnap(float x)
{
    float width = this->rect().width() - 2;
    if (x < 0)
        x += width;
    if (x < (width * 0.3333))
        return 0;
    else if (x > (width * 0.6667))
        return width;
    return -1;
}
