#ifndef TAB_H
#define TAB_H

#include <QTabWidget>
#include "mapperwidget.h"

namespace Ui {
class Tab;
}

class Tab : public QWidget, public MapperWidget
{
public:
    explicit Tab(QTabWidget *parent, MapperStuff *mapper_stuff);
    ~Tab();

//    void update() = 0;
    void deviceEvent(const mapper::Device& dev, mapper_record_action action);
    void signalEvent(const mapper::Signal& sig, mapper_record_action action);
    void mapEvent(const mapper::Map& map, mapper_record_action action);
    void signalUpdateEvent(const mapper::Signal& sig, mapper_id instance,
                           const void *value, int count,
                           mapper_timetag_t *timetag);

private:
    Ui::Tab *ui;
};

#endif // TAB_H
