#ifndef SIGNALLIST_H
#define SIGNALLIST_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class SignalList;
}

class SignalList : public QWidget
{
    Q_OBJECT

public:
    explicit SignalList(QWidget *parent);
    ~SignalList();

    void clear();
    void setRole(bool _is_src);
    int addDevice(qulonglong id, const QString & name);
    void removeDevice(qulonglong id);
    void addSignal(qulonglong dev_id, qulonglong qig_id, const QString &signame,
                   QChar type, qreal length);
    void removeSignal(qulonglong id);
    QPointF signalPosition(qulonglong id);

public Q_SLOTS:
    void selectionChanged();
    void filterPatternChanged(const QString& pattern);
    void filterPrefixChanged(const QString& prefix);

Q_SIGNALS:
    void updated();
    void selectedSigs(QList<qulonglong> ids, QList<QPointF> positions, bool is_src);
    void dropped(qulonglong id);
    void selectDrag(QPointF pos, bool is_src);
    void selectDrop(bool is_src);

protected:
    bool eventFilter(QObject *object, QEvent *event);
//    void mousePressEvent(QMouseEvent *event);

private:
    Ui::SignalList *ui;
    bool is_src;
    bool dragging;
//    MapperStuff *data;
};

#endif // SignalList_H
