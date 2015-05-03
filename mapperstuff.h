#ifndef MAPPERSTUFF
#define MAPPERSTUFF

#include <mapper/mapper_cpp.h>

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

//protected:
    int deviceFlags;
    int signalFlags;
    int connectionFlags;
};

#endif // MAPPERSTUFF

