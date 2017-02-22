#include "listtab.h"
#include "ui_listtab.h"
#include <math.h>
#include <QSplitter>
#include <QDebug>

ListTab::ListTab(QTabWidget *parent, MapperStuff *_mapper_data) :
    Tab(parent, _mapper_data),
    ui(new Ui::ListTab)
{
    ui->setupUi(this);
    addDeviceCallback();
    addSignalCallback();
    addMapCallback();

    // add current devices and signals
    for (auto const &signal : mapper_data->db.signals()) {
        signalEvent(signal, MAPPER_ADDED);
    }

    connect(ui->listview, SIGNAL(updateMaps()), this, SLOT(updateMaps()));
    connect(ui->listview, SIGNAL(selectedMaps(QList<qulonglong>)),
            this, SLOT(setSelectedMaps(QList<qulonglong>)));
    connect(ui->listview, SIGNAL(releaseSelectedMaps()),
            this, SLOT(releaseSelectedMaps()));
    connect(ui->listview, SIGNAL(toggleSelectedMapsMuting()),
            this, SLOT(toggleSelectedMapsMuting()));
//    connect(ui->listview, SIGNAL(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)),
//            this, SLOT(selectedSigs(QList<qulonglong>, QList<QPointF>, bool)));
    connect(ui->listview, SIGNAL(mapSigs(QList<qulonglong>, qulonglong)),
            this, SLOT(mapSigs(QList<qulonglong>,qulonglong)));

    connect(ui->mapProps, SIGNAL(setExpression(QString)),
            this, SLOT(setExpression(QString)));

//    ui->listview->installEventFilter(this);
}

ListTab::~ListTab()
{
    delete ui;
}

void ListTab::deviceEvent(const mapper::Device& dev, mapper_record_event event)
{
    int direction = (  (dev.num_signals(MAPPER_DIR_INCOMING) ? MAPPER_DIR_INCOMING : 0)
                     | (dev.num_signals(MAPPER_DIR_OUTGOING) ? MAPPER_DIR_OUTGOING : 0));

    switch (event) {
    case MAPPER_ADDED:
        qDebug() << "MAPPER_ADDED" << QString::fromStdString(dev.name()) << dev.num_signals();
        qDebug() << "  out" << dev.num_signals(MAPPER_DIR_OUTGOING);
        qDebug() << "  in" << dev.num_signals(MAPPER_DIR_INCOMING);
        ui->listview->addDevice(dev.id(), QString::fromStdString(dev.name()),
                                dev.num_signals(MAPPER_DIR_OUTGOING),
                                dev.num_signals(MAPPER_DIR_INCOMING));
        break;
    case MAPPER_REMOVED:
        ui->listview->removeDevice(dev.id());
        break;
    default:
        break;
    }
}

void ListTab::signalEvent(const mapper::Signal& sig, mapper_record_event event)
{
    switch (event) {
    case MAPPER_ADDED:
    case MAPPER_MODIFIED:
        // may need to add device as well
        deviceEvent(sig.device(), MAPPER_ADDED);
        ui->listview->addSignal(sig.device().id(), sig.id(),
                                QString::fromStdString(sig.name()),
                                QChar(sig.type()), sig.length(),
                                sig.direction() == MAPPER_DIR_OUTGOING);
        // redraw maps
        // TODO: mark maps as dirty and update at end of poll() instead
        for (auto const& map : mapper_data->db.maps()) {
            mapEvent(map, MAPPER_ADDED);
        }
        break;
    case MAPPER_REMOVED:
        ui->listview->removeSignal(sig.id());
        break;
    default:
        break;
    }
}

void ListTab::mapEvent(const mapper::Map& map, mapper_record_event event)
{
    switch (event) {
    case MAPPER_ADDED:
    case MAPPER_MODIFIED: {
        QList<qulonglong> srcs;
        mapper::Signal dst = map.destination().signal();
        for (int i = 0; i < map.num_sources(); i++) {
            srcs << map.source(i).signal();
        }
        ui->listview->addMap(map.id(), srcs, dst, map.muted());
        }
        break;
    case MAPPER_REMOVED:
        ui->listview->removeMap(map.id());
        break;
    default:
        break;
    }
}

void ListTab::signalUpdateEvent(const mapper::Signal &sig, mapper_id instance,
                                const void *value, int count,
                                mapper_timetag_t *timetag)
{
//    qDebug() << "signalUpdateEvent" << props->name;
}

void ListTab::updateMaps()
{
    for (auto const &map : mapper_data->db.maps()) {
        mapEvent(map, MAPPER_ADDED);
    }
}

void ListTab::setSelectedMaps(QList<qulonglong> ids)
{
    if (ids.isEmpty()) {
        ui->mapProps->clearProps();
//        ui->listview->clearSelection;
        return;
    }
    QList<int> mode;
    QList<bool> muted;
    QList<QString> expr;
    for (auto id : ids) {
        mapper::Map map = mapper_data->db.map(id);
        if (!map)
            continue;
        mode << map.mode();
        muted << map.muted();
        expr << QString::fromStdString(map.expression());
    }
    ui->mapProps->displayProps(mode, muted, expr);
    selectedMaps = ids;
}

void ListTab::releaseSelectedMaps()
{
    printf("ListTab::releaseSelectedMaps()\n");
    for (auto id : selectedMaps) {
        mapper::Map map = mapper_data->db.map(id);
        if (map)
            map.release();
    }
    selectedMaps.clear();
    ui->mapProps->clearProps();
}

void ListTab::toggleSelectedMapsMuting()
{
    int muting = -1;
    for (auto id : selectedMaps) {
        mapper::Map map = mapper_data->db.map(id);
        if (!map)
            continue;
        if (map.muted()) {
            if (muting == -1)
                muting = 0;
            else if (muting == 1)
                muting = 2;
            map.set_muted(false);
        }
        else {
            if (muting == -1)
                muting = 1;
            else if (muting == 0)
                muting = 2;
            map.set_muted(true);
        }
        map.push();
    }
    ui->mapProps->setMuted(muting);
}

void ListTab::setModeLinear()
{

}

void ListTab::setModeExpression()
{

}

void ListTab::setExpression(QString expr)
{
    for (auto id : selectedMaps) {
        mapper::Map map = mapper_data->db.map(id);
        if (map)
            map.set_mode(MAPPER_MODE_EXPRESSION)
               .set_expression(expr.toStdString())
               .push();
    }
}

void ListTab::selectedSigs(QList<qulonglong> ids, QList<QPointF> positions,
                           bool is_src)
{
    return;

    QList<qulonglong> *listptr = is_src ? &selectedSrcSigIds : &selectedDstSigIds;
    QList<qulonglong> diff;
    for (auto const& id : *listptr) {
        if (!ids.contains(id)) {
            diff << id;
        }
    }
    for (auto const& id : diff) {
        // remove item from list
        (*listptr).removeAll(id);
    }
    diff.clear();
    for (auto const& id : ids) {
        if (!(*listptr).contains(id)) {
            diff << id;
        }
    }
//    for (auto const& id : diff) {
//        // map signal
//        mapper_data->getSignalUpdates(this, id);

//        // add to list
//        *listptr << id;
//    }
}

void ListTab::mapSigs(QList<qulonglong> srcIds, qulonglong dstId)
{
    std::vector<mapper::Signal> srcSigs, dstSigs;
    mapper::Signal tmp = mapper_data->db.signal(dstId);
    if (!tmp)
        return;
    dstSigs.push_back(tmp);
    for (qulonglong id : srcIds) {
        tmp = mapper_data->db.signal(id);
        if (!tmp)
            return;
        srcSigs.push_back(tmp);
    }
    mapper::Map map(srcSigs, dstSigs);
    map.push();
}

void ListTab::unmapSigs(QList<qulonglong> srcs, qulonglong dst)
{

}

void ListTab::update()
{
//    ui->listview->update();
}

void ListTab::resizeEvent(QResizeEvent *event)
{
    ui->listview->resize();
}

//bool ListTab::eventFilter(QObject *object, QEvent *event)
//{
//    switch (event->type()) {
//    case QEvent::MouseButtonPress:
//        qDebug() << "QEvent::MouseButtonPress";
//        break;
//    case QEvent::MouseMove:
//        qDebug() << "QEvent::MouseMove";
//        break;
//    case QEvent::MouseButtonRelease:
//        qDebug() << "MouseButtonRelease";
//        break;
//    case QEvent::Paint:
//        return QObject::eventFilter(object, event);

//    default:
//        qDebug() << "other";
//        return QObject::eventFilter(object, event);
//    }
//    event->setAccepted(true);
//    return true;
//}
