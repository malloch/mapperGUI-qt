#include "listtab.h"
#include "ui_listtab.h"
#include <math.h>
#include <QSplitter>

ListTab::ListTab(QTabWidget *parent, MapperStuff *_data) :
    Tab(parent, _data),
    ui(new Ui::ListTab)
{
    ui->setupUi(this);
}

ListTab::~ListTab()
{
    ;
}

void ListTab::deviceEvent()
{
    printf("DEVICE EVENT (list)\n");
    ListView *view = ui->listview;
    printf("data = %p\n", data);
    printf("data->db = %p\n", &data->db);
    printf("data->monitor = %p\n", &data->monitor);
//    printf("data->monitor.db() = %p\n", &data->monitor.db());
    for (auto const &device : data->monitor.db().devices()) {
        if (device.num_outputs() > 0) {
            printf("should be adding source device %s\n", device.name().c_str());
            view->addDevice(0, QString::fromStdString(device.name()), DI_OUTGOING);
//            mapper::Db::Signal::Iterator sig = data->monitor.db().outputs(device.name()).begin();
//            for (auto const &signal : data->monitor.db().outputs(device.name())) {
//                view->addSignal(0, QString::fromStdString(signal.name()),
//                                QChar(signal.type()), signal.length(), DI_OUTGOING);
//            }
        }
        if (device.num_inputs() > 0) {
            printf("should be adding destination device %s\n", device.name().c_str());
            view->addDevice(0, QString::fromStdString(device.name()), DI_INCOMING);
//            mapper::Db::Signal::Iterator sig = data->monitor.db().inputs(device.name()).begin();
//            for (auto const &signal : data->monitor.db().inputs(device.name())) {
//                view->addSignal(0, QString::fromStdString(signal.name()),
//                                QChar(signal.type()), signal.length(), DI_INCOMING);
//            }
        }
    }
}

void ListTab::signalEvent()
{
    ;
}

void ListTab::connectionEvent()
{
    ;
}

void ListTab::update()
{
    ;
}
