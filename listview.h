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
    void addSignal(int parentindex, const QString &name,
                   QChar type, qreal length, int direction);

private:
    Ui::ListView *ui;
};

#endif // LISTVIEW_H
