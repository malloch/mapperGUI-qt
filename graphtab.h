#ifndef GRAPHTAB_H
#define GRAPHTAB_H

//#include <QColor>
//#include <QVector>
#include <QTabWidget>
////#include <QGraphicsScene>
#include <QGraphicsView>

#include "tab.h"
#include "graphnode.h"
#include "graphedge.h"


class GraphTab : public Tab
{
    Q_OBJECT

public:
    GraphTab(QTabWidget *parent, mapperGUIData data);
    ~GraphTab();

    void update();
    void deviceEvent();
    void linkEvent();
    void itemMoved();
    void unselectNodes();
    void unselectEdges();

    int numObjects;
//    QList<GraphNode *> nodes;
//    QList<GraphEdge *> edges;
    QGraphicsScene *scene;

public slots:
//    void animate();
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
////    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void scaleView(qreal scaleFactor);

private:
    QGraphicsView *view;
    mapperGUIData data;
    bool needsUpdate;
    QGraphicsItem* itemAt(const QPoint &pos);
};

#endif // GRAPHTAB_H
