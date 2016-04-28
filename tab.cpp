#include "tab.h"

Tab::Tab(QTabWidget *parent, MapperStuff *mapper_stuff) :
    MapperWidget(parent, mapper_stuff)
{

}

Tab::~Tab()
{

}

void Tab::deviceEvent(const mapper::Device& dev, mapper_record_action action) {}

void Tab::signalEvent(const mapper::Signal& sig, mapper_record_action action) {}

void Tab::mapEvent(const mapper::Map& map, mapper_record_action action) {}

void Tab::signalUpdateEvent(const mapper::Signal& sig, mapper_id instance,
                            const void *value, int count,
                            mapper_timetag_t *timetag) {}
