#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>

namespace Ui {
class ListView;
}

class ListView : public QWidget
{
    Q_OBJECT

public:
    explicit ListView(QWidget *parent = 0);
    ~ListView();

    void clear();
    void resize();
    void update();

    void addDevice(qulonglong id, const QString & name, int num_outputs,
                   int num_inputs);
    void removeDevice(qulonglong id);

    void addSignal(qulonglong dev_id, qulonglong sig_id, const QString &signame,
                   QChar type, qreal length, bool is_output);
    void removeSignal(qulonglong id);
    QPointF signalPosition(qulonglong id);

    void addMap(qulonglong id,
                QList<qulonglong> srcs, qulonglong dst, bool muted);
    void removeMap(qulonglong id);

public Q_SLOTS:
    void selectedSigs(QList<qulonglong> ids, QList<QPointF> positions, bool is_src);
    void dropped(qulonglong id);

Q_SIGNALS:
    void updateMaps();
    void selectedMaps(QList<qulonglong> ids);
    void releaseSelectedMaps();
    void toggleSelectedMapsMuting();
    void dragAndDrop(QList<qulonglong> srcs, qulonglong dst);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::ListView *ui;
    QList<qulonglong> selectedIds;
    QList<QPointF> selected;
};

#endif // LISTVIEW_H
