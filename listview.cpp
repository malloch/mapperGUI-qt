#include "listview.h"
#include "ui_listview.h"

ListView::ListView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListView)
{
    ui->setupUi(this);
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
    if (direction & DI_INCOMING)
        ui->destination->addDevice(index, name);
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
    if (direction & DI_INCOMING)
        ui->destination->addSignal(devname, signame, type, length);
}

void ListView::removeSignal(const QString &devname, const QString &signame)
{
    ui->source->removeSignal(devname, signame);
    ui->destination->removeSignal(devname, signame);
}
