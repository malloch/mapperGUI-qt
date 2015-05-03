#ifndef TAB_H
#define TAB_H

#include <mapperstuff.h>
#include <QTabWidget>

namespace Ui {
class Tab;
}

class Tab : public QWidget
{
public:
    explicit Tab(QTabWidget *_parent, MapperStuff *data);
    ~Tab();

    virtual void update() = 0;
    virtual void deviceEvent() = 0;
    virtual void signalEvent() = 0;
    virtual void connectionEvent() = 0;

protected:
    MapperStuff *data;

private:
    Ui::Tab *ui;
};

#endif // TAB_H
