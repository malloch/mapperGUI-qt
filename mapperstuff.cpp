#include <mapperstuff.h>
#include <QDebug>

MapperStuff::MapperStuff() :
    monitor(),
    db(monitor.db()),
    device("/monitor")
{
    db.add_device_callback(deviceHandler, this);
    db.add_signal_callback(signalHandler, this);
    db.add_map_callback(mapHandler, this);
    ready = false;
}

MapperStuff::~MapperStuff()
{
    // disconnect any mapped signals
    monitor.unmap(db.maps(device));
    monitor.unsubscribe();
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

void MapperStuff::getSignalUpdates(Tab *tab, const QString& signame)
{
    const char *s = signame.toStdString().c_str();
    s = strchr(s, '/');
    if (!s)
        return;
    s++;
    mapper::Signal sig = device.add_input(s, 1, 'f', 0, 0, 0,
                                          signalUpdateHandler, this);
    if (!signalUpdateCallbacks.contains(tab))
        signalUpdateCallbacks << tab;
    // map the remote signal
    monitor.map(signame.toStdString().c_str(), sig);
}

void MapperStuff::cancelSignalUpdates(Tab *tab, const QString &signame)
{
    if (signame != 0) {
        mapper::Signal sig = device.inputs(signame.toStdString());
        if (sig)
            monitor.unmap(db.maps(sig));
    }
    else {
        // unmap all
        monitor.unmap(db.maps(device));
    }

    signalUpdateCallbacks.removeAll(tab);
}

int MapperStuff::poll()
{
    int count = monitor.poll(1);
    count += device.poll(1);
    if (!ready && device.ready()) {
        db.flush();
        monitor.subscribe(SUBSCRIBE_DEVICE);
    }
    return count;
}

void deviceHandler(mapper_db_device dev, mapper_db_action_t action, void *user)
{
    qDebug() << "QtMonitor got update from device" << dev->name;

    MapperStuff *data = (MapperStuff*) user;
    // filter out our own signals
    if (!data->ready)
        return;
    if (data->device.name().compare(dev->name)==0)
        return;
    if (action == MDB_NEW) {
        // autosubscribe to future updates
        data->monitor.subscribe(dev, SUBSCRIBE_ALL, -1);
    }
    foreach (Tab *tab, data->deviceCallbacks)
        tab->deviceEvent(dev, action);
}

void signalHandler(mapper_db_signal sig, mapper_db_action_t action, void *user)
{
    qDebug() << "QtMonitor got update from signal" << sig->name;

    MapperStuff *data = (MapperStuff*) user;
    // filter out our own signals
    if (!data->ready)
        return;
    if (data->device.name().compare(sig->device->name)==0)
        return;
    foreach (Tab *tab, data->signalCallbacks)
        tab->signalEvent(sig, action);
}

void mapHandler(mapper_db_map map, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from map ");
//    if (map->num_sources > 1)
//        printf("[");
//    for (int i = 0; i < map->num_sources; i++)
//        printf("%s, ", map->sources[i].signal->name);
//    printf("\b\b%s -> %s", map->num_sources > 1 ? "[" : "", map->destination.signal->name);

    MapperStuff *data = (MapperStuff*) user;
    // filter out our own signals
    if (!data->ready)
        return;
    std::string name = data->device.name();
    if (name.compare(map->destination.signal->device->name)==0)
        return;
    for (int i = 0; i < map->num_sources; i++) {
        if (name.compare(map->sources[i].signal->device->name)==0)
            return;
    }
    foreach (Tab *tab, data->mapCallbacks)
        tab->mapEvent(map, action);
}

void signalUpdateHandler(mapper_signal sig, mapper_db_signal props,
                         int instance_id, void *value, int count,
                         mapper_timetag_t *timetag)
{
//    qDebug() << "signalUpdateHandler" << props->name;

    MapperStuff *data = (MapperStuff*) props->user_data;
    foreach (Tab *tab, data->signalUpdateCallbacks)
        tab->signalUpdateEvent(sig, props, instance_id, value, count, timetag);
}
