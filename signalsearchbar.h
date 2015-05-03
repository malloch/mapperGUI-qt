#ifndef SIGNALSEARCHBAR_H
#define SIGNALSEARCHBAR_H

#include <QWidget>

namespace Ui {
class SignalSearchBar;
}

class SignalSearchBar : public QWidget
{
    Q_OBJECT

public:
    explicit SignalSearchBar(QWidget *parent = 0);
    ~SignalSearchBar();

private:
    Ui::SignalSearchBar *ui;
};

#endif // SIGNALSEARCHBAR_H
