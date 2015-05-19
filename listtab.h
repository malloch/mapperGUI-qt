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
    void deviceEvent(mapper_db_device dev, mapper_db_action_t action);
    void signalEvent(mapper_db_signal sig, mapper_db_action_t action);
    void connectionEvent(mapper_db_connection con, mapper_db_action_t action);

private:
    Ui::ListTab *ui;
};

#endif // LISTTAB_H
