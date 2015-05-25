#ifndef LINKVIEW_H
#define LINKVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>

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
//    void update();
    void addLink(uint32_t hash, QPointF src, QPointF dst);
    void removeLink(uint32_t hash);
    void resize();

    class Edge : public QGraphicsItem
    {
    public:
        Edge(uint32_t _hash, QPointF _src, QPointF _dst)
            : hash(_hash), src(_src), dst(_dst), selected(false)
        {
            setFlags(ItemIsSelectable);
            setAcceptHoverEvents(true);
            setAcceptedMouseButtons(Qt::LeftButton);
//            update();
        };
        ~Edge() {};

        void resize(QPointF &_src, QPointF &_dst)
        {
            printf("edge::resize\n");
            prepareGeometryChange();
            src = _src;
            dst = _dst;
//            update();
        }

        uint32_t hash;
        QPointF src;
        QPointF dst;
        bool selected;

    protected:
        QRectF boundingRect() const Q_DECL_OVERRIDE;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE
        {
            printf("mouse press event\n");
            selected = (selected == false);
            update();
        };
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE
        {
//            printf("hover event\n");
//            setSelected(isSelected()==false);
        };

    };

private:
    Ui::LinkView *ui;
    QGraphicsScene *scene;
    bool needsUpdate;
};

#endif // LINKVIEW_H
