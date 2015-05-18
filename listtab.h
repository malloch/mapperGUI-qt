#ifndef LISTTAB_H
#define LISTTAB_H

//#include <QTabWidget>
//#include <QGraphicsView>
#include "mapperstuff.h"
#include "tab.h"

namespace Ui {
class ListTab;
}

class ListTab : public Tab
{
    Q_OBJECT

public:
    explicit ListTab(QTabWidget *parent = 0, MapperStuff *data = 0);
    ~ListTab();

    void update();
    void deviceEvent();
    void signalEvent();
    void connectionEvent();

private:
    Ui::ListTab *ui;
};

#endif // LISTTAB_H
