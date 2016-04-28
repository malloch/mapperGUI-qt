#include "mapperwidget.h"
#include <QDebug>

MapperWidget::MapperWidget(QWidget *parent, MapperStuff *_data)
{
//    qDebug() << "making new mapperwidget\n" << _data;
    if (_data) {
        mapper_data = _data;
        own_data = false;
    }
    else {
//        qDebug() << "making new mapperstuff\n";
        mapper_data = new MapperStuff;
        own_data = true;
        mapper_data->db.add_device_callback(deviceHandler, mapper_data);
        mapper_data->db.add_signal_callback(signalHandler, mapper_data);
        mapper_data->db.add_map_callback(mapHandler, mapper_data);
    }
}

MapperWidget::MapperWidget(MapperWidget *parent)
{
    mapper_data = parent->mapper_data;
}

MapperWidget::~MapperWidget()
{
    if (own_data)
        delete mapper_data;
}

void MapperWidget::addDeviceCallback()
{
    mapper_data->deviceCallbacks << this;
}

void MapperWidget::addSignalCallback()
{
    mapper_data->signalCallbacks << this;
//    qDebug() << "adding signal callback!" << mapper_data->signalCallbacks.count()
//             << "to object" << mapper_data;
}

void MapperWidget::addMapCallback()
{
    mapper_data->mapCallbacks << this;
}

void deviceHandler(mapper_database db, mapper_device dev,
                   mapper_record_action action, const void *user)
{
//    printf("deviceHandler: '%s'\n", mapper_device_name(dev));

    MapperStuff *data = (MapperStuff*) user;
    // filter out our own signals
    if (!data->ready)
        return;
    if (action == MAPPER_ADDED) {
        // autosubscribe to future updates
        data->db.subscribe(dev, MAPPER_OBJ_ALL, -1);
    }
    for (auto tab : data->deviceCallbacks)
        tab->deviceEvent(dev, action);
}

void signalHandler(mapper_database db, mapper_signal _sig,
                   mapper_record_action action, const void *user)
{
    mapper::Signal sig(_sig);
//    qDebug() << "QtMonitor got update from signal" << mapper_signal_name(_sig);

    MapperStuff *data = (MapperStuff*) user;
    // TODO: filter out our own signals
//    if (!data->ready)
//        return;
//    qDebug() << data << "has" << data->signalCallbacks.count() << "callbacks";
    for (auto tab : data->signalCallbacks) {
//        qDebug() << "calling signal event";
        tab->signalEvent(sig, action);
    }
}

void mapHandler(mapper_database db, mapper_map _map,
                mapper_record_action action, const void* user)
{
//    printf("QtMonitor got update from map ");
//    if (map->num_sources > 1)
//        printf("[");
//    for (int i = 0; i < map->num_sources; i++)
//        printf("%s, ", map->sources[i].signal->name);
//    printf("\b\b%s -> %s", map->num_sources > 1 ? "[" : "", map->destination.signal->name);

    mapper::Map map(_map);
    MapperStuff *data = (MapperStuff*) user;
    if (!data->ready)
        return;
    for (auto tab : data->mapCallbacks)
        tab->mapEvent(map, action);
}

