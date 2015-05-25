#include "linkview.h"
#include "ui_linkview.h"
#include "iostream"

LinkView::LinkView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkView)
{
    ui->setupUi(this);
    QRect rect = this->rect();
    scene = new QGraphicsScene(rect.left(), rect.top(), rect.width()-2, rect.height()-50);
    ui->links->setScene(scene);
    ui->links->setDragMode(QGraphicsView::NoDrag);

//    ui->links->setSceneRect(ui->links->viewport()->rect());
//    QRectF viewRect = this->rect();
//    std::cout << "settingscenerect" << scene->width() << ","<< scene->height() << std::endl;
//    ui->links->setSceneRect(0, +20, ui->links->width(), viewRect.height());
}

LinkView::~LinkView()
{
    delete ui;
}

void LinkView::clear()
{
    scene->clear();
}

void LinkView::addLink(QPointF src, QPointF dst)
{
    QPainterPath *path = new QPainterPath();

    float width = this->width()-2;
    float halfwidth = width * 0.5;

    path->moveTo(src.x() * width, src.y());
    path->cubicTo(halfwidth, src.y(),
                  halfwidth, dst.y(),
                  dst.x() * width, dst.y());

//    QPen *pen = new QPen(QBrush()0)
    scene->addPath(*path, QPen(Qt::black, 2));

    QPointF endPoint, arrowP1, arrowP2;
    endPoint = QPointF(dst.x() * width, dst.y());
    arrowP1 = endPoint + QPointF(dst.x() * -8.66025403784439, 5);
    arrowP2 = endPoint + QPointF(dst.x() * -8.66025403784439, -5);

    scene->addPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2,
                      QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap,
                           Qt::RoundJoin), QBrush(Qt::black));

    scene->update();

//    painter.setPen(QColor(255, 255, 255));
//    painter.strokePath(*path, painter.pen());
}

void LinkView::resize()
{
    QRect rect = this->rect();
    ui->links->setSceneRect(rect.left(), rect.top(), rect.width()-2, rect.height()-50);
}
