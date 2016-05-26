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

    connect(ui->maps, SIGNAL(mapSelectedSigs()), this, SLOT(mapSelectedSigs()));
    connect(ui->maps, SIGNAL(unmapSelectedSigs()), this, SLOT(unmapSelectedSigs()));

    //    connect(ui->source, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)),
    //            this, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)));
    //    connect(ui->destination, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)),
    //            this, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>)));
    connect(ui->source, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)),
            this, SLOT(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)));
    connect(ui->destination, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)),
            this, SLOT(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)));
    connect(ui->source, SIGNAL(selectDrag(QPointF, bool)),
            this, SLOT(dragSelectedSigs(QPointF, bool)));
    connect(ui->source, SIGNAL(selectDrop(QPointF, bool)),
            this, SLOT(dropSelectedSigs(QPointF, bool)));
    connect(ui->destination, SIGNAL(selectDrag(QPointF, bool)),
            this, SLOT(dragSelectedSigs(QPointF, bool)));
    connect(ui->destination, SIGNAL(selectDrop(QPointF, bool)),
            this, SLOT(dropSelectedSigs(QPointF, bool)));

//    connect(ui->source, SIGNAL(dropped(qulonglong)), this, SLOT(dropped(qulonglong)));
//    connect(ui->destination, SIGNAL(dropped(qulonglong)), this, SLOT(dropped(qulonglong)));

//    this->installEventFilter(this);
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
//    int tempi = 0;
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
    if (is_src) {
        selectedSrcIds = ids;
        selectedSrcPos = locations;
    }
    else {
        selectedDstIds = ids;
        selectedDstPos = locations;
    }
}

void ListView::dragSelectedSigs(QPointF pos, bool is_src)
{
    qDebug() << "ListView::dragSelectedSigs" << pos << is_src;
    // check if should snap to src or dst
    int snap = ui->maps->shouldSnap(pos.x());
    if (snap == 0) {
        // should snap to src
        pos.setX(snap);
        int y = ui->source->snap(pos.y());
        qDebug() << "src snap " << y;
        if (y >= 0)
            pos.setY(y);
    }
    else if (snap != -1) {
        // should snap to dst
        pos.setX(snap);
        int y = ui->destination->snap(pos.y());
        qDebug() << "dst snap " << y;
        if (y >= 0)
            pos.setY(y);
    }
    if (is_src) {
        ui->maps->drawDrag(selectedSrcPos, pos);
    }
    else {
        ui->maps->drawDrag(selectedDstPos, pos);
    }
}

void ListView::dropSelectedSigs(QPointF pos, bool is_src)
{
    qDebug() << "ListView::dropSelectedSigs" << is_src;
    int snap = ui->maps->shouldSnap(pos.x());
    qulonglong id = 0;
    if (snap == 0) {
        id = ui->source->itemAt(pos.y());
    }
    else if (snap > 0) {
        id = ui->destination->itemAt(pos.y());
    }
    if (id) {
        qDebug() << "dropped on id " << id;
        Q_EMIT mapSigs(is_src ? selectedSrcIds : selectedDstIds, id);
    }
    ui->maps->removeMap(0);
}

void ListView::mapSelectedSigs()
{
    if (!selectedSrcIds.isEmpty() && !selectedDstIds.isEmpty())
        Q_EMIT mapSigs(selectedSrcIds, selectedDstIds[0]);
}

void ListView::unmapSelectedSigs()
{
    if (!selectedSrcIds.isEmpty() && !selectedDstIds.isEmpty())
        Q_EMIT unmapSigs(selectedSrcIds, selectedDstIds[0]);
}

//void ListView::mouseMoveEvent(QMouseEvent *event)
//{
//    qDebug() << "ListView::mouseMoveEvent";
//    // get selections from signallists
////    if (selectedSrcIds.isEmpty())
//        return;
////    ui->maps->drawDrag(selected, event->pos());
//}

//void ListView::dropped(qulonglong id)
//{
//    printf("dropped! should be connecting %llu -> %llu\n", selectedIds[0], id);
//    if (selectedIds.length() == 0)
//        return;
//    Q_EMIT dragAndDrop(selectedIds, id);
//}

bool ListView::eventFilter(QObject *object, QEvent *event)
{
//    qDebug() << "ListView::eventFilter";
    switch (event->type()) {
//    case QEvent::MouseButtonPress: {
//        qDebug() << "QEvent::MouseButtonPress";
//        break;
//    }
    case QEvent::MouseMove:
//        qDebug() << "ListView::MouseMove";
        break;
    case QEvent::MouseButtonRelease:
//        qDebug() << "ListView::MouseButtonRelease";
        break;
    case QEvent::KeyPress:
//        qDebug() << "ListView::KeyPress";
        break;

    default:
        return QObject::eventFilter(object, event);
    }
    event->setAccepted(true);
    return true;
}

