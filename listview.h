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
    void addDevice(int index, const QString & name, int direction);
    void removeDevice(const QString & name);
    void addSignal(const QString &devname, const QString &signame,
                   QChar type, qreal length, int direction);
    void removeSignal(const QString &devname, const QString &signame);

private:
    Ui::ListView *ui;
};

#endif // LISTVIEW_H
