#include "listtab.h"
#include "ui_listtab.h"
#include <math.h>
#include <QSplitter>

ListTab::ListTab(QTabWidget *parent, MapperStuff *_data) :
    Tab(parent, _data),
    ui(new Ui::ListTab)
{
    ui->setupUi(this);
//    QGridLayout *layout = new QGridLayout(this);
//    layout->setContentsMargins(0, 0, 0, 0);
//    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
//    QVBoxLayout *splitter = new QVBoxLayout(this);
//    splitter->setContentsMargins(0, 0, 0, 0);

//    parent->insertTab(0, this, QString("List View"));

//    props = new ConnectionProps;
//    splitter->insertWidget(0, props);

//    QWidget *lists = new QWidget();
//    splitter->insertWidget(1, lists);
//    splitter->setStretch(0, 1);
//    splitter->setStretch(1, 3);
//    QHBoxLayout *listLayout = new QHBoxLayout;
//    listLayout->setContentsMargins(0, 0, 0, 0);
//    listLayout->setSpacing(0);
//    lists->setLayout(listLayout);

//    sources = new SignalList(this, " Sources", 1);
//    listLayout->insertWidget(0, sources);
//    links = new LinkView;
//    listLayout->insertWidget(1, links);
//    destinations = new SignalList(this, " Destinations", 0);
//    listLayout->insertWidget(2, destinations);
//    listLayout->setStretch(0, 2);
//    listLayout->setStretch(1, 1);
//    listLayout->setStretch(2, 2);
}

ListTab::~ListTab()
{
    ;
}

void ListTab::deviceEvent()
{
//    printf("DEVICE EVENT (list)\n");
//    sources->clear();
//    destinations->clear();

//    for (auto const &device : data->db.devices()) {
//        printf("should be working with dev '%s'\n", device.name().c_str());
//        if (device.num_outputs() > 0) {
//            sources->addDevice(0, QString::fromStdString(device.name()));
//            mapper::Db::Signal::Iterator sig = data->db.outputs(device.name()).begin();
//            for (auto const &signal : data->db.outputs(device.name())) {
//                sources->addSignal(0, QString::fromStdString(signal.name()),
//                                   QChar(signal.type()), signal.length());
//            }
//        }
//        if (device.num_inputs() > 0) {
//            destinations->addDevice(0, QString::fromStdString(device.name()));
//            mapper::Db::Signal::Iterator sig = data->monitor.db().outputs(device.name()).begin();
//            for (auto const &signal : data->db.inputs(device.name())) {
//                destinations->addSignal(0, QString::fromStdString(signal.name()),
//                                       QChar(signal.type()), signal.length());
//            }
//        }
//    }
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
