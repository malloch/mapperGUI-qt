#ifndef LINKVIEW_H
#define LINKVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>

#define arrowSize 8

namespace Ui {
class LinkView;
}

class LinkView : public QWidget
{
    Q_OBJECT

public:
    explicit LinkView(QWidget *parent = 0);
    ~LinkView();

    void clear();
    void addLink(uint32_t hash, QPointF src, QPointF dst, bool muted);
    void removeLink(uint32_t hash);
    void resize();

    class Edge : public QGraphicsItem
    {
    public:
        Edge(uint32_t _hash, QPointF _src, QPointF _dst, bool _muted)
            : hash(_hash), src(_src), dst(_dst), muted(_muted), selected(false) {};
        ~Edge() {};

        void resize(QPointF &_src, QPointF &_dst)
        {
            prepareGeometryChange();
            src = _src;
            dst = _dst;
        }

        uint32_t hash;
        QPointF src;
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

signals:
    void selectedMaps(QList<uint32_t> hashes);

private:
    Ui::LinkView *ui;
    QGraphicsScene *scene;
    bool needsUpdate;
    QPointF lastPos;
    void clearSelected();
    void updateSelected(const QPointF &newPos);
    QList<uint32_t> selectedMapHashes;
};

#endif // LINKVIEW_H
