#ifndef CONNECTIONPROPS2_H
#define CONNECTIONPROPS2_H

#include <QWidget>

namespace Ui {
class ConnectionProps2;
}

class ConnectionProps2 : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionProps2(QWidget *parent = 0);
    ~ConnectionProps2();

private:
    Ui::ConnectionProps2 *ui;
};

#endif // CONNECTIONPROPS2_H
