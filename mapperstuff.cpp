#include <mapperstuff.h>

MapperStuff::MapperStuff() :
    monitor(SUBSCRIBE_ALL),
    db(monitor.db()),
    device("/monitor")
{
    deviceFlags = 0;
    signalFlags = 0;
    connectionFlags = 0;

    db.add_device_callback(deviceHandler, &deviceFlags);
    db.add_signal_callback(signalHandler, &signalFlags);
    db.add_connection_callback(connectionHandler, &connectionFlags);
}

MapperStuff::~MapperStuff()
{
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
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void signalHandler(mapper_db_signal sig, mapper_db_action_t action, void *user)
{
    printf("QtMonitor got update from signal '%s'\n", sig->name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void connectionHandler(mapper_db_connection con, mapper_db_action_t action, void *user)
{
    printf("QtMonitor got update from connection ");
    if (con->num_sources > 1)
        printf("[");
    for (int i = 0; i < con->num_sources; i++)
        printf("%s, ", con->sources[i].signal->name);
    printf("\b\b%s -> %s", con->num_sources > 1 ? "[" : "", con->destination.signal->name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

//void signalHandler(mapper_signal sig, mapper_db_signal props, int instance_id,
//                   void *value, int count, mapper_timetag_t *timetag)
//{
//    ;
//}

