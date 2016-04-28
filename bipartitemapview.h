#ifndef BIPARTITEMAPVIEW_H
#define BIPARTITEMAPVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>

#define arrowSize 8

namespace Ui {
class BipartiteMapView;
}

class BipartiteMapView : public QWidget
{
    Q_OBJECT

public:
    explicit BipartiteMapView(QWidget *parent = 0);
    ~BipartiteMapView();

    void clear();
    void addMap(qulonglong id, QList<QPointF> srcs, QPointF dst, bool muted);
    void removeMap(qulonglong id);
    void resize();
    void drawDrag(QList<QPointF>, QPointF);

    class Edge : public QGraphicsItem
    {
    public:
        Edge(qulonglong _id, QList<QPointF> _srcs, QPointF _dst, bool _muted)
            : id(_id), srcs(_srcs), dst(_dst), muted(_muted), selected(false) {};
        ~Edge() {};

        void resize(QList<QPointF> &_srcs, QPointF &_dst)
        {
            prepareGeometryChange();
            srcs = _srcs;
            dst = _dst;
        }

        qulonglong id;
        QList<QPointF> srcs;
        QPointF dst;
        bool selected;
        bool muted;

    protected:
        QRectF boundingRect() const Q_DECL_OVERRIDE;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget) Q_DECL_OVERRIDE;
    };

protected:
    bool eventFilter(QObject *object, QEvent *event);

Q_SIGNALS:
    void selectedMaps(QList<qulonglong> ids);
    void releaseSelectedMaps();
    void toggleSelectedMapsMuting();

private:
    Ui::BipartiteMapView *ui;
    QGraphicsScene *scene;
    bool needsUpdate;
    QPointF lastPos;
    void clearSelected();
    void updateSelected(const QPointF &newPos);
    QList<qulonglong> selectedMapIds;
};

#endif // BIPARTITEMAPVIEW_H
