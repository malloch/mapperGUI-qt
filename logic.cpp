#include "logic.h"
//#include <stdio.h>

void deviceHandler(mapper_db_device dev, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from device '%s'\n", dev->name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void linkHandler(mapper_db_link lnk, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from link '%s'->'%s'\n", lnk->src_name, lnk->dest_name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void signalHandler(mapper_db_signal sig, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from signal '%s'\n", sig->name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void connectionHandler(mapper_db_connection con, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from connection '%s'->'%s'\n", con->src_name, con->dest_name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void signalHandler(mapper_signal sig, mapper_db_signal props, int instance_id,
                   void *value, int count, mapper_timetag_t *timetag)
{
    displayBuffer *b = (displayBuffer*) props->user_data;
}
