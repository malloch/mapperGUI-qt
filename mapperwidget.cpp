#include "mapperwidget.h"
#include <QDebug>

MapperWidget::MapperWidget(QWidget *parent, MapperStuff *_data)
{
    qDebug() << "making new mapperwidget\n" << _data;
    if (_data) {
        mapper_data = _data;
        own_data = false;
    }
    else {
        qDebug() << "making new mapperstuff\n";
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
                   mapper_record_event event, const void *user)
{
//    printf("deviceHandler: '%s'\n", mapper_device_name(dev));
    qDebug() << "QtMonitor got update from device" << mapper_device_name(dev);

    MapperStuff *data = (MapperStuff*) user;
    // filter out our own signals
    if (!data->ready)
        return;
    if (event == MAPPER_ADDED) {
        // autosubscribe to future updates
        data->db.subscribe(dev, MAPPER_OBJ_ALL, -1);
    }
    for (auto tab : data->deviceCallbacks)
        tab->deviceEvent(dev, event);
}

void signalHandler(mapper_database db, mapper_signal _sig,
                   mapper_record_event event, const void *user)
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
        tab->signalEvent(sig, event);
    }
}

void mapHandler(mapper_database db, mapper_map _map,
                mapper_record_event event, const void* user)
{
    mapper::Map map(_map);

    qDebug() << "QtMonitor got update from MAP ";
    for (int i = 0; i < map.num_sources(); i++)
        qDebug() << "  from: " << map.source(i).signal().name().c_str();
    qDebug() << "  to: " << map.destination(0).signal().name().c_str();

    MapperStuff *data = (MapperStuff*) user;
    if (!data->ready)
        return;
    for (auto tab : data->mapCallbacks)
        tab->mapEvent(map, event);
}

