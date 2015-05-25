#include "listview.h"
#include "ui_listview.h"

ListView::ListView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListView)
{
    ui->setupUi(this);
    connect(ui->source, SIGNAL(updateMaps()), this, SIGNAL(updateMaps()));
    connect(ui->destination, SIGNAL(updateMaps()), this, SIGNAL(updateMaps()));
}

ListView::~ListView()
{
    delete ui;
}

void ListView::clear()
{
    ui->source->clear();
    ui->destination->clear();
    // clear link display
    // clear connection props display
}

void ListView::addDevice(int index, const QString & name, int direction)
{
    if (direction & DI_OUTGOING)
        ui->source->addDevice(index, name);
    else
        ui->source->removeDevice(name);

    if (direction & DI_INCOMING)
        ui->destination->addDevice(index, name);
    else
        ui->destination->removeDevice(name);
}

void ListView::removeDevice(const QString & name)
{
    ui->source->removeDevice(name);
    ui->destination->removeDevice(name);
}

void ListView::addSignal(const QString &devname, const QString &signame,
                         QChar type, qreal length, int direction)
{
    if (direction & DI_OUTGOING)
        ui->source->addSignal(devname, signame, type, length);
    else
        ui->source->removeSignal(devname, signame);

    if (direction & DI_INCOMING)
        ui->destination->addSignal(devname, signame, type, length);
    else
        ui->destination->removeSignal(devname, signame);
}

void ListView::removeSignal(const QString &devname, const QString &signame)
{
    ui->source->removeSignal(devname, signame);
    ui->destination->removeSignal(devname, signame);
}

QPointF ListView::signalPosition(const QString &devname, const QString &signame)
{
    QPointF p = ui->source->signalPosition(devname, signame);
    if (!p.isNull())
        return p;
    return ui->destination->signalPosition(devname, signame);
}

void ListView::clearMaps()
{
    ui->links->clear();
}

void ListView::addMap(const QString &srcdevname, const QString &srcsigname,
                      const QString &dstdevname, const QString &dstsigname)
{
    QPointF srcpos = signalPosition(srcdevname, srcsigname);
    QPointF dstpos = signalPosition(dstdevname, dstsigname);
    ui->links->addLink(srcpos, dstpos);
}
