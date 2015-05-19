#ifndef MAPPERSTUFF
#define MAPPERSTUFF

#include <mapper/mapper_cpp.h>
#include "tab.h"
#include <QList>

// function prototypes
void deviceHandler(mapper_db_device dev, mapper_db_action_t a, void *user);
void signalHandler(mapper_db_signal sig, mapper_db_action_t a, void *user);
void connectionHandler(mapper_db_connection con, mapper_db_action_t a, void *user);

class MapperStuff
{
public:
    explicit MapperStuff();
    ~MapperStuff();

    mapper::Monitor monitor;
    const mapper::Db db;
    mapper::Device device;

    int poll();
    void addDeviceCallback(Tab *tab);
    void addSignalCallback(Tab *tab);
    void addConnectionCallback(Tab *tab);

//protected:
    QList <Tab *> deviceCallbacks;
    QList <Tab *> signalCallbacks;
    QList <Tab *> connectionCallbacks;
    int deviceFlags;
    int signalFlags;
    int connectionFlags;
};

#endif // MAPPERSTUFF

