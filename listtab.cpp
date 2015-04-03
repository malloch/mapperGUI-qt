#include "listtab.h"
#include <math.h>
#include <QSplitter>

ListTab::ListTab(QTabWidget *parent, mapperGUIData _data)
    : Tab(parent, _data)
{
    data = _data;
//    QGridLayout *layout = new QGridLayout(this);
//    layout->setContentsMargins(0, 0, 0, 0);
//    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    QVBoxLayout *splitter = new QVBoxLayout(this);
    splitter->setContentsMargins(0, 0, 0, 0);

    parent->insertTab(0, this, QString("List"));

    props = new ConnectionProps;
    splitter->insertWidget(0, props);

    QWidget *lists = new QWidget();
    splitter->insertWidget(1, lists);
    splitter->setStretch(0, 1);
    splitter->setStretch(1, 3);
    QHBoxLayout *listLayout = new QHBoxLayout;
    listLayout->setContentsMargins(0, 0, 0, 0);
    listLayout->setSpacing(0);
    lists->setLayout(listLayout);

    sources = new SignalList(this, " Sources", 1);
    listLayout->insertWidget(0, sources);
    links = new LinkView;
    listLayout->insertWidget(1, links);
    destinations = new SignalList(this, " Destinations", 0);
    listLayout->insertWidget(2, destinations);
    listLayout->setStretch(0, 2);
    listLayout->setStretch(1, 1);
    listLayout->setStretch(2, 2);
}

ListTab::~ListTab()
{
    ;
}

void ListTab::deviceEvent()
{
    printf("DEVICE EVENT (list)\n");
    sources->clear();
    destinations->clear();

    for (auto const &device : data->db.devices()) {
        if (device.num_outputs() > 0) {
            sources->addDevice(0, QString::fromStdString(device.name()));
            mapper::Db::Signal::Iterator sig = data->db.outputs(device.name()).begin();
            for (; sig != data->db.outputs(device.name()).end(); sig++) {
                sources->addSignal(0, QString::fromStdString((*sig).name()),
                                   QChar((*sig).type()), (*sig).length());
            }
        }
        if (device.num_inputs() > 0) {
            destinations->addDevice(0, QString::fromStdString(device.name()));
            mapper::Db::Signal::Iterator sig = data->db.outputs(device.name()).begin();
            for (; sig != data->db.outputs(device.name()).end(); sig++) {
                destinations->addSignal(0, QString::fromStdString((*sig).name()),
                                        QChar((*sig).type()), (*sig).length());
            }
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
