#ifndef SIGNALLIST_H
#define SIGNALLIST_H

#include <QWidget>
#include "mapperstuff.h"

namespace Ui {
class SignalList;
}

class SignalList : public QWidget
{
    Q_OBJECT

public:
    explicit SignalList(QWidget *parent, const char *_label, int _is_src);
    ~SignalList();

    void clear();
    int addDevice(int index, const QString & name);
    void removeDevice(const QString &name);
    void addSignal(const QString &devname, const QString &signame,
                   QChar type, qreal length);
    void removeSignal(const QString &devname, const QString &signame);
    QPointF signalPosition(const QString &devname, const QString &signame);

private:
    Ui::SignalList *ui;
    int is_src;
    MapperStuff *data;
};

#endif // SignalList_H
