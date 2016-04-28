#include "listview.h"
#include "ui_listview.h"
#include <QDebug>

ListView::ListView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListView)
{
    ui->setupUi(this);
    ui->source->setRole(true);
    ui->destination->setRole(false);

    connect(ui->source, SIGNAL(updated()), this, SIGNAL(updateMaps()));
    connect(ui->destination, SIGNAL(updated()), this, SIGNAL(updateMaps()));

    connect(ui->maps, SIGNAL(selectedMaps(QList<qulonglong>)),
            this, SIGNAL(selectedMaps(QList<qulonglong>)));
    connect(ui->maps, SIGNAL(releaseSelectedMaps()),
            this, SIGNAL(releaseSelectedMaps()));
    connect(ui->maps, SIGNAL(toggleSelectedMapsMuting()),
            this, SIGNAL(toggleSelectedMapsMuting()));

    //    connect(ui->source, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)),
    //            this, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)));
    //    connect(ui->destination, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)),
    //            this, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)));
    connect(ui->source, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)),
            this, SLOT(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)));
    connect(ui->destination, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)),
            this, SLOT(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)));

    connect(ui->source, SIGNAL(dropped(qulonglong)), this, SLOT(dropped(qulonglong)));
    connect(ui->destination, SIGNAL(dropped(qulonglong)), this, SLOT(dropped(qulonglong)));
}

ListView::~ListView()
{
    delete ui;
}

void ListView::clear()
{
    ui->source->clear();
    ui->destination->clear();
    // clear map display
    // clear map props display
}

void ListView::addDevice(qulonglong id, const QString & name,
                         int num_outputs, int num_inputs)
{
    printf("listView::addDevice '%s'\n", name.toStdString().c_str());
    if (num_outputs)
        ui->source->addDevice(id, name);
    else
        ui->source->removeDevice(id);

    if (num_inputs)
        ui->destination->addDevice(id, name);
    else
        ui->destination->removeDevice(id);
}

void ListView::removeDevice(qulonglong id)
{
    ui->source->removeDevice(id);
    ui->destination->removeDevice(id);
}

void ListView::addSignal(qulonglong dev_id, qulonglong sig_id,
                         const QString &signame, QChar type, qreal length,
                         bool is_output)
{
    qDebug() << "listView::addSignal" << signame << is_output;
    if (is_output) {
        ui->source->addSignal(dev_id, sig_id, signame, type, length);
        ui->destination->removeSignal(sig_id);

    }
    else {
        ui->destination->addSignal(dev_id, sig_id, signame, type, length);
        ui->source->removeSignal(sig_id);
    }
}

void ListView::removeSignal(qulonglong id)
{
    ui->source->removeSignal(id);
    ui->destination->removeSignal(id);
}

QPointF ListView::signalPosition(qulonglong id)
{
    QPointF p = ui->source->signalPosition(id);
    if (!p.isNull())
        return p;
    return ui->destination->signalPosition(id);
}

void ListView::addMap(qulonglong id, QList<qulonglong> srcs, qulonglong dst,
                      bool muted)
{
    QList<QPointF> srcpos;
    QPointF p;
    int tempi = 0;
    for (auto const& src : srcs) {
        p = signalPosition(src);
        if (p.isNull())
            return;
        srcpos << p;
    }
    QPointF dstpos = signalPosition(dst);
    if (dstpos.isNull())
        return;
    ui->maps->addMap(id, srcpos, dstpos, muted);
}

void ListView::removeMap(qulonglong id)
{
    ui->maps->removeMap(id);
}

void ListView::resize()
{
    ui->maps->resize();
}

void ListView::update()
{
    ui->maps->update();
}

void ListView::selectedSigs(QList<qulonglong> ids, QList<QPointF> locations,
                            bool is_src)
{
    selected = locations;
    selectedIds = ids;
}

void ListView::mouseMoveEvent(QMouseEvent *event)
{
    // get selections from signallists
    if (selected.length() == 0)
        return;
    ui->maps->drawDrag(selected, event->pos());
}

void ListView::dropped(qulonglong id)
{
    printf("dropped! should be connecting %llu -> %llu\n", selectedIds[0], id);
    if (selectedIds.length() == 0)
        return;
    Q_EMIT dragAndDrop(selectedIds, id);
}


