#ifndef SIGNALLIST_H
#define SIGNALLIST_H

#include <QWidget>

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
    void addSignal(int parentindex, const QString &name, QChar type, qreal length);
    float signalOffset(const QString & name);

private:
    Ui::SignalList *ui;
    int is_src;
};

#endif // SignalList_H