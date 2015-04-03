#include "linkview.h"
#include "ui_linkview.h"
#include "iostream"

LinkView::LinkView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkView)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    ui->links->setScene(scene);
    ui->links->setDragMode(QGraphicsView::NoDrag);

//    ui->links->setSceneRect(ui->links->viewport()->rect());
    QRectF viewRect = this->rect();
    std::cout << "settingscenerect" << ui->links->width() << ","<< viewRect.height() << std::endl;
    ui->links->setSceneRect(0, +20, ui->links->width(), viewRect.height());
}

LinkView::~LinkView()
{
    delete ui;
}

void LinkView::clear()
{
//    scene->clear();
}

void LinkView::addLink(int yL, int yR, int direction)
{
    QPainterPath *path = new QPainterPath();
//    float xL = ui->links->viewport()->rect().left();
//    float xR = ui->links->viewport()->rect().right();
    float width = ui->links->width();
    float height = ui->links->height();
    yL -= height*0.5;
    yR -= height*0.5;
    path->moveTo(width*-0.5, yL);
    path->cubicTo(width*0.5, yL, width*0.5, yR, width*1.5, yR);

//    scene->addRect(ui->links->viewport()->rect());
//    QPen *pen = new QPen(QBrush()0)
    scene->addPath(*path, QPen(Qt::black, 2));
    QPointF endPoint, arrowP1, arrowP2;
    if (direction) {
        endPoint = QPointF(width * -0.5, yL);
        arrowP1 = endPoint + QPointF(8.66025403784439, 5);
        arrowP2 = endPoint + QPointF(8.66025403784439, -5);
    }
    else {
        endPoint = QPointF(width * 1.5, yR);
        arrowP1 = endPoint + QPointF(-8.66025403784439, 5);
        arrowP2 = endPoint + QPointF(-8.66025403784439, -5);
    }
    scene->addPolygon(QPolygonF() << endPoint << arrowP1 << arrowP2, QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), QBrush(Qt::black));

    scene->update();

//    painter.setPen(QColor(255, 255, 255));
//    painter.strokePath(*path, painter.pen());
}
