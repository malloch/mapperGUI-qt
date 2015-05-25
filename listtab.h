#ifndef LISTTAB_H
#define LISTTAB_H

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
    void mapEvent(mapper_db_map map, mapper_db_action_t action);

public slots:
    void updateMaps();

private:
    Ui::ListTab *ui;
};

#endif // LISTTAB_H
