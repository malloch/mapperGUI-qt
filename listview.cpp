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
    SignalList *list = direction == DI_OUTGOING ? ui->source : ui->destination;
    list->addDevice(index, name);
}

void ListView::addSignal(int parentindex, const QString &name,
                         QChar type, qreal length, int direction)
{
    SignalList *list = direction == DI_OUTGOING ? ui->source : ui->destination;
    list->addSignal(parentindex, name, type, length);
}
