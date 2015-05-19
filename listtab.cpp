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
    data->addConnectionCallback(this);
}

ListTab::~ListTab()
{
    ;
}

void ListTab::deviceEvent(mapper_db_device dev, mapper_db_action_t action)
{
    int direction = ((  dev->num_inputs ? DI_INCOMING : 0)
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
        ui->listview->addSignal(QString::fromStdString(sig->device->name),
                                QString::fromStdString(sig->name),
                                QChar(sig->type), sig->length, sig->direction);
        break;
    case MDB_REMOVE:
        ui->listview->removeSignal(QString::fromStdString(sig->device->name),
                                   QString::fromStdString(sig->name));
        break;
    default:
        break;
    }
}

void ListTab::connectionEvent(mapper_db_connection con, mapper_db_action_t action)
{
    ;
}

void ListTab::update()
{
    ;
}
