#ifndef LIVESIGNALVIEW_H
#define LIVESIGNALVIEW_H

#include <QWidget>

namespace Ui {
class LiveSignalView;
}

class LiveSignalView : public QWidget
{
    Q_OBJECT

public:
    explicit LiveSignalView(QWidget *parent = 0);
    ~LiveSignalView();

private:
    Ui::LiveSignalView *ui;
};

#endif // LIVESIGNALVIEW_H
