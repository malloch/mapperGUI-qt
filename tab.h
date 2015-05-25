#ifndef TAB_H
#define TAB_H

#include <mapper/mapper_cpp.h>
#include <QTabWidget>

namespace Ui {
class Tab;
}

class MapperStuff;
class Tab : public QWidget
{
public:
    explicit Tab(QTabWidget *_parent, MapperStuff *data);
    ~Tab();

    virtual void update() = 0;
    virtual void deviceEvent(mapper_db_device dev, mapper_db_action_t action) = 0;
    virtual void signalEvent(mapper_db_signal sig, mapper_db_action_t action) = 0;
    virtual void mapEvent(mapper_db_map map, mapper_db_action_t action) = 0;

protected:
    MapperStuff *data;

private:
    Ui::Tab *ui;
};

#endif // TAB_H
