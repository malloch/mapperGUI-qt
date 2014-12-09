#ifndef LOGIC_H
#define LOGIC_H

#include <mapper/mapper.h>

enum t_tabType {
    TAB_TYPE_LIST,
    TAB_TYPE_GRAPH
};

//typedef struct _tabList
//{
//    int numDevices;
//    const char **deviceNames;
//    int scrollAmount;
//    const char *searchString;
//} t_tabList;

//typedef struct _tab
//{
//    t_tabType tabType;
//    void *tabData;
//} t_tab, *tab;

typedef struct _mapperGUIData
{
    mapper_admin admin;
    mapper_monitor monitor;
    mapper_db db;
    mapper_device device;
    mapper_timetag_t timetag;
    int deviceFlags;
    int linkFlags;
    int signalFlags;
    int connectionFlags;
} t_mapperGUIData, *mapperGUIData;

typedef struct _displayBuffer
{
    float data[100];
    int index;
    int length;
} t_displayBuffer, *displayBuffer;

// function prototypes
void deviceHandler(mapper_db_device dev, mapper_db_action_t a, void *user);
void linkHandler(mapper_db_link lnk, mapper_db_action_t a, void *user);
void signalHandler(mapper_db_signal sig, mapper_db_action_t a, void *user);
void connectionHandler(mapper_db_connection con, mapper_db_action_t a, void *user);

#endif // LOGIC_H
