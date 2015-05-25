#include "linkview.h"
#include "ui_linkview.h"
#include "iostream"

LinkView::LinkView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkView)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this->rect());
    ui->links->setScene(scene);
    ui->links->setDragMode(QGraphicsView::NoDrag);

//    ui->links->setSceneRect(ui->links->viewport()->rect());
//    QRectF viewRect = this->rect();
    std::cout << "settingscenerect" << scene->width() << ","<< scene->height() << std::endl;
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
    printf("adding link from (%f,%f)->(%f,%f)\n", src.x(), src.y(), dst.x(), dst.y());
    QPainterPath *path = new QPainterPath();
//    float xL = ui->links->viewport()->rect().left();
//    float xR = ui->links->viewport()->rect().right();
    float width = this->width();
    float halfwidth = width * 0.5;

    path->moveTo(src.x() * width, src.y());
    path->cubicTo(halfwidth, src.y(),
                  halfwidth, dst.y(),
                  dst.x() * width, dst.y());

//    scene->addRect(ui->links->viewport()->rect());
//    QPen *pen = new QPen(QBrush()0)
    scene->addPath(*path, QPen(Qt::black, 2));
    QPointF endPoint, arrowP1, arrowP2;
    endPoint = QPointF(dst.x() * width, dst.y());
    arrowP1 = endPoint + QPointF(dst.x() * 8.66025403784439, 5);
    arrowP2 = endPoint + QPointF(dst.x() * 8.66025403784439, -5);

    scene->addPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2,
                      QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap,
                           Qt::RoundJoin), QBrush(Qt::black));

    scene->update();

//    painter.setPen(QColor(255, 255, 255));
//    painter.strokePath(*path, painter.pen());
}
