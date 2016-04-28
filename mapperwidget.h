#ifndef MAPPERWIDGET_H
#define MAPPERWIDGET_H

#include <QWidget>
#include <mapper/mapper_cpp.h>

// function prototypes
void deviceHandler(mapper_database db, mapper_device dev,
                   mapper_record_action a, const void *user);
void signalHandler(mapper_database db, mapper_signal sig,
                   mapper_record_action a, const void *user);
void mapHandler(mapper_database db, mapper_map map,
                mapper_record_action a, const void *user);

class MapperWidget;
class MapperStuff
{
public:
    mapper::Database db;
    explicit MapperStuff() : db() {
        printf("new mapperstuff!\n");
        ready = false;
        db.subscribe(MAPPER_OBJ_ALL);
    }

    ~MapperStuff() {
        db.unsubscribe();
    }

    int poll() {
        return db.poll();
    }

//protected:
    QList <MapperWidget *> deviceCallbacks;
    QList <MapperWidget *> signalCallbacks;
    QList <MapperWidget *> mapCallbacks;
    int deviceFlags;
    int signalFlags;
    int mapFlags;
    bool ready;
    double now;
};

class MapperWidget
{
public:
    explicit MapperWidget(QWidget *_parent, MapperStuff *_data);
    explicit MapperWidget(MapperWidget *_parent);
    ~MapperWidget();

    void addDeviceCallback();
    void addSignalCallback();
    void addMapCallback();

//    virtual void update() = 0;
    virtual void deviceEvent(const mapper::Device& dev, mapper_record_action action) = 0;
    virtual void signalEvent(const mapper::Signal& sig, mapper_record_action action) = 0;
    virtual void mapEvent(const mapper::Map& map, mapper_record_action action) = 0;
    virtual void signalUpdateEvent(const mapper::Signal& sig, mapper_id instance,
                                   const void *value, int count,
                                   mapper_timetag_t *timetag) = 0;


protected:
    MapperStuff *mapper_data;
    bool own_data;
};

#endif // MAPPERWIDGET_H
