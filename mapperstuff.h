#ifndef MAPPERSTUFF
#define MAPPERSTUFF

#include <mapper/mapper_cpp.h>

typedef struct _mapperGUIData
{
    mapper::Admin *admin;
    mapper::Monitor *monitor;
    mapper::Db db;
    mapper::Device *device;
    mapper::Timetag timetag;
//    mapper_admin admin;
//    mapper_monitor monitor;
//    mapper_db db;
//    mapper_device device;
//    mapper_timetag_t timetag;
    int deviceFlags;
    int linkFlags;
    int signalFlags;
    int connectionFlags;
} t_mapperGUIData, *mapperGUIData;

#endif // MAPPERSTUFF

