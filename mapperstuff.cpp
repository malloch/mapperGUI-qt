#include <mapperstuff.h>

MapperStuff::MapperStuff() :
    monitor(SUBSCRIBE_ALL),
    db(monitor.db()),
    device("/monitor")
{
    db.add_device_callback(deviceHandler, this);
    db.add_signal_callback(signalHandler, this);
    db.add_map_callback(mapHandler, this);
}

MapperStuff::~MapperStuff()
{
}

void MapperStuff::addDeviceCallback(Tab *tab)
{
    deviceCallbacks << tab;
}

void MapperStuff::addSignalCallback(Tab *tab)
{
    signalCallbacks << tab;
}

void MapperStuff::addMapCallback(Tab *tab)
{
    mapCallbacks << tab;
}

int MapperStuff::poll()
{
    int count = monitor.poll();
//    count += device.poll();
    return count;
}

void deviceHandler(mapper_db_device dev, mapper_db_action_t action, void *user)
{
    printf("QtMonitor got update from device '%s'\n", dev->name);

    MapperStuff *data = (MapperStuff*) user;
    foreach (Tab *tab, data->deviceCallbacks)
        tab->deviceEvent(dev, action);
}

void signalHandler(mapper_db_signal sig, mapper_db_action_t action, void *user)
{
    printf("QtMonitor got update from signal '%s'\n", sig->name);

    MapperStuff *data = (MapperStuff*) user;
    foreach (Tab *tab, data->signalCallbacks)
        tab->signalEvent(sig, action);
}

void mapHandler(mapper_db_map map, mapper_db_action_t action, void *user)
{
    printf("QtMonitor got update from map ");
    if (map->num_sources > 1)
        printf("[");
    for (int i = 0; i < map->num_sources; i++)
        printf("%s, ", map->sources[i].signal->name);
    printf("\b\b%s -> %s", map->num_sources > 1 ? "[" : "", map->destination.signal->name);

    MapperStuff *data = (MapperStuff*) user;
    foreach (Tab *tab, data->mapCallbacks)
        tab->mapEvent(map, action);
}
