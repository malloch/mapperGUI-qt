#include "listtab.h"
#include "ui_listtab.h"
#include <math.h>
#include <QSplitter>

ListTab::ListTab(QTabWidget *parent, MapperStuff *_data) :
    Tab(parent, _data),
    ui(new Ui::ListTab)
{
    ui->setupUi(this);
    data->addDeviceCallback(this);
    data->addSignalCallback(this);
    data->addMapCallback(this);

    // add current devices and signals
    for (auto const &signal : data->db.inputs()) {
        signalEvent(signal, MDB_NEW);
    }
    for (auto const &signal : data->db.outputs()) {
        signalEvent(signal, MDB_NEW);
    }

    connect(ui->listview, SIGNAL(updateMaps()), this, SLOT(updateMaps()));
}

ListTab::~ListTab()
{
    ;
}

void ListTab::deviceEvent(mapper_db_device dev, mapper_db_action_t action)
{
    int direction = (  (dev->num_inputs ? DI_INCOMING : 0)
                     | (dev->num_outputs ? DI_OUTGOING : 0));

    switch (action) {
    case MDB_NEW:
        ui->listview->addDevice(0, QString::fromStdString(dev->name), direction);
        break;
    case MDB_REMOVE:
        ui->listview->removeDevice(QString::fromStdString(dev->name));
        break;
    default:
        break;
    }
}

void ListTab::signalEvent(mapper_db_signal sig, mapper_db_action_t action)
{
    switch (action) {
    case MDB_NEW:
    case MDB_MODIFY:
        ui->listview->addSignal(QString::fromStdString(sig->device->name),
                                QString::fromStdString(sig->name),
                                QChar(sig->type), sig->length, sig->direction);
        // redraw maps
        ui->listview->clearMaps();
        // add current devices
        for (auto const &map : data->db.maps()) {
            mapEvent(map, MDB_NEW);
        }
        break;
    case MDB_REMOVE:
        ui->listview->removeSignal(QString::fromStdString(sig->device->name),
                                   QString::fromStdString(sig->name));
        break;
    default:
        break;
    }
}

void ListTab::mapEvent(mapper_db_map map, mapper_db_action_t action)
{
    switch (action) {
    case MDB_NEW: {
        mapper_db_signal src, dst = map->destination.signal;
        for (int i = 0; i < map->num_sources; i++) {
            src = map->sources[i].signal;
            ui->listview->addMap(QString::fromStdString(src->device->name),
                                 QString::fromStdString(src->name),
                                 QString::fromStdString(dst->device->name),
                                 QString::fromStdString(dst->name));
        }}
        break;
    case MDB_MODIFY:
        break;
    case MDB_REMOVE:
        // redraw maps
        ui->listview->clearMaps();
        // add current devices
        for (auto const &map : data->db.maps()) {
            mapEvent(map, MDB_NEW);
        }
        break;
    default:
        break;
    }
}

void ListTab::updateMaps()
{
    ui->listview->clearMaps();
    for (auto const &map : data->db.maps()) {
        mapEvent(map, MDB_NEW);
    }
}

void ListTab::update()
{
    ;
}

void ListTab::resizeEvent(QResizeEvent *event)
{
    ui->listview->clearMaps();
    ui->listview->resize();
    for (auto const &map : data->db.maps()) {
        mapEvent(map, MDB_NEW);
    }
}
