#include "logic.h"
#include <stdio.h>

void deviceHandler(mapper_db_device dev, mapper_db_action_t a, void *user)
{
    printf("QtMonitor got update from device '%s'\n", dev->name);
    int *flags = (int*)user;
    *flags |= DEVICES_UPDATED;
}

void linkHandler(mapper_db_link lnk, mapper_db_action_t a, void *user)
{
    printf("QtMonitor got update from link '%s'->'%s'\n", lnk->src_name, lnk->dest_name);
    int *flags = (int*)user;
    *flags |= LINKS_UPDATED;
}

void signalHandler(mapper_db_signal sig, mapper_db_action_t a, void *user)
{
    printf("QtMonitor got update from signal '%s'\n", sig->name);
    int *flags = (int*)user;
    *flags |= SIGNALS_UPDATED;
}

void connectionHandler(mapper_db_connection con, mapper_db_action_t a, void *user)
{
    printf("QtMonitor got update from connection '%s'->'%s'\n", con->src_name, con->dest_name);
    int *flags = (int*)user;
    *flags |= CONNECTIONS_UPDATED;
}
