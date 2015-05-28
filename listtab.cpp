#include "listtab.h"
#include "ui_listtab.h"
#include <math.h>
#include <QSplitter>
#include <QDebug>

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
    connect(ui->listview, SIGNAL(selectedMaps(QList<uint32_t>)),
            this, SLOT(selectedMaps(QList<uint32_t>)));
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
            ui->listview->addMap(map->hash,
                                 QString::fromStdString(src->device->name),
                                 QString::fromStdString(src->name),
                                 QString::fromStdString(dst->device->name),
                                 QString::fromStdString(dst->name),
                                 map->muted);
        }}
        break;
    case MDB_MODIFY:
        break;
    case MDB_REMOVE:
        ui->listview->removeMap(map->hash);
        break;
    default:
        break;
    }
}

void ListTab::updateMaps()
{
    for (auto const &map : data->db.maps()) {
        mapEvent(map, MDB_NEW);
    }
}

void ListTab::selectedMaps(QList<uint32_t> hashes)
{
    if (hashes.isEmpty()) {
        ui->connectionProps->clearProps();
        return;
    }
    qDebug() << "selected maps:" << hashes;
    if (hashes.count() != 1) {
        qDebug() << "Only handling single map selections for now.";
        return;
    }
    mapper::Db::Map map = data->db.map(hashes.at(0));
    if (!map) {
        qDebug() << "Couldn't find map in database.";
        return;
    }
    qDebug() << "muted: " << (int)map.get("muted");
    ui->connectionProps->displayProps(map.mode(), map.get("muted"), map.get("calibrating"),
                                      QString::fromStdString(map.expression()));
}

void ListTab::update()
{
//    ui->listview->update();
}

void ListTab::resizeEvent(QResizeEvent *event)
{
    ui->listview->resize();
}
