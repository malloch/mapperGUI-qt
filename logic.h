#ifndef LOGIC_H
#define LOGIC_H

#include <mapper/mapper.h>

#define DEVICES_UPDATED     0x01
#define SIGNALS_UPDATED     0x02
#define LINKS_UPDATED       0x04
#define CONNECTIONS_UPDATED 0x08

enum t_tabType {
    TAB_TYPE_LIST,
    TAB_TYPE_GRAPH
};

typedef struct _tabList
{
    int numDevices;
    const char **deviceNames;
    int scrollAmount;
    const char *searchString;
} t_tabList;

typedef struct _listTab
{
    t_tabList sources;
    t_tabList destinations;
    bool hideUnmapped;
};

typedef struct _tab
{
    t_tabType tabType;
    void *tabData;
} t_tab, *tab;

// function prototypes
void deviceHandler(mapper_db_device dev, mapper_db_action_t a, void *user);
void linkHandler(mapper_db_link lnk, mapper_db_action_t a, void *user);
void signalHandler(mapper_db_signal sig, mapper_db_action_t a, void *user);
void connectionHandler(mapper_db_connection con, mapper_db_action_t a, void *user);

#endif // LOGIC_H
