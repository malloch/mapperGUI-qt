#include "signallist.h"
#include "ui_signallist.h"
#include "iostream"
#include <QDebug>
#include <QAbstractItemView>

SignalList::SignalList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalList)
{
    ui->setupUi(this);
    ui->tree->setColumnWidth(0, ui->searchBar->width()-120);
    ui->tree->setColumnWidth(1, 50);
    ui->tree->setColumnWidth(2, 50);
    ui->tree->setColumnHidden(3, true);
    ui->tree->setAlternatingRowColors(true);

//    ui->tree->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(ui->tree, SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this, SIGNAL(updated()));
    connect(ui->tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this, SIGNAL(updated()));
    connect(ui->tree, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
    connect(ui->searchBar, SIGNAL(patternChanged(const QString&)),
            this, SLOT(filterPatternChanged(const QString&)));
    connect(ui->searchBar, SIGNAL(prefixChanged(const QString&)),
            this, SLOT(filterPrefixChanged(const QString&)));

    // also trigger redraw when columns are sorted
    ui->tree->header()->setSectionsClickable(true);
    connect(ui->tree->header(), SIGNAL(sectionClicked(int)),
            this, SIGNAL(updated()));
}

SignalList::~SignalList()
{
    delete ui;
}

void SignalList::clear()
{
    ui->tree->clear();
}

void SignalList::setRole(bool _is_src)
{
    is_src = _is_src;
    ui->label->setText(is_src ? "Sources" : "Destinations");
}

int SignalList::addDevice(qulonglong id, const QString &name)
{
    for (int i = 0; i < ui->tree->topLevelItemCount(); i++) {
        QTreeWidgetItem *dev = ui->tree->topLevelItem(i);
        QVariant dev_id = dev->data(0, Qt::UserRole);
        if (dev_id.toULongLong() == id)
            return -1;
    }
    QTreeWidgetItem *qdev = new QTreeWidgetItem((QTreeWidget*)0,
                                                QStringList(name));
    qdev->setData(0, Qt::UserRole, QVariant(id));
    ui->tree->addTopLevelItem(qdev);
    ui->tree->expandItem(qdev);
    ui->searchBar->addDevice(id, name);
    Q_EMIT updated();
    return ui->tree->indexOfTopLevelItem(qdev);
}

void SignalList::removeDevice(qulonglong id)
{
    for (int i = 0; i < ui->tree->topLevelItemCount(); i++) {
        QTreeWidgetItem *dev = ui->tree->topLevelItem(i);
        QVariant dev_id = dev->data(0, Qt::UserRole);
        if (dev_id.toULongLong() == id) {
            delete(dev);
            ui->searchBar->removeDevice(id);
            Q_EMIT updated();
            return;
        }
    }
}

void SignalList::addSignal(qulonglong dev_id, qulonglong sig_id,
                           const QString &signame, QChar type, qreal length)
{
    // find device
    QTreeWidgetItem *dev;
    int i;
    for (i = 0; i < ui->tree->topLevelItemCount(); i++) {
        dev = ui->tree->topLevelItem(i);
        QVariant temp = dev->data(0, Qt::UserRole);
        if (temp.toULongLong() == dev_id)
            break;
    }
    if (i == ui->tree->topLevelItemCount()) {
        // device not found
        return;
    }

    // find signal
    QTreeWidgetItem *sig;
    for (i = 0; i < dev->childCount(); i++) {
        sig = dev->child(i);
        QVariant temp = sig->data(0, Qt::UserRole);
        if (temp.toULongLong() == sig_id) {
            // todo: overwrite metadata?
            return;
        }
    }

    sig = new QTreeWidgetItem();
    sig->setText(0, signame);
    sig->setText(1, QString(type));
    sig->setText(2, QString::number(length));
    sig->setData(0, Qt::UserRole, QVariant(sig_id));
    dev->addChild(sig);
    Q_EMIT updated();
}

void SignalList::removeSignal(qulonglong id)
{
    QTreeWidgetItem *dev, *sig;
    int i, j;
    for (i = 0; i < ui->tree->topLevelItemCount(); i++) {
        dev = ui->tree->topLevelItem(i);
        for (j = 0; j < dev->childCount(); j++) {
            sig = dev->child(j);
            if (sig->data(0, Qt::UserRole).toULongLong() == id) {
                delete(sig);
                Q_EMIT updated();
                return;
            }
        }
    }
}

QPointF SignalList::signalPosition(qulonglong id)
{
    QTreeWidgetItem *dev, *sig;
    int i, j, found = 0;
    for (i = 0; i < ui->tree->topLevelItemCount(); i++) {
        dev = ui->tree->topLevelItem(i);
        for (j = 0; j < dev->childCount(); j++) {
            sig = dev->child(j);
            if (sig->data(0, Qt::UserRole).toULongLong() == id) {
                found = 1;
                break;
            }
        }
        if (found)
            break;
    }
    if (!found || dev->isHidden() || sig->isHidden())
        return QPointF(0, 0);
    if (dev->isExpanded()) {
        QRect rect = ui->tree->visualItemRect(sig);
        return QPointF(is_src ? 0 : 1, rect.center().y());
    }
    else {
        QRect rect = ui->tree->visualItemRect(dev);
        return QPointF(is_src ? 0 : 1, rect.center().y());
    }
}

void SignalList::selectionChanged()
{
    QList<QTreeWidgetItem*> selected = ui->tree->selectedItems();
    // pass list of selected signal names to parent
    QList<qulonglong> ids;
    QList<QPointF> positions;
    for (auto const& item : selected) {
        if (item->childCount())
            continue;
        ids << item->data(0, Qt::UserRole).toULongLong();
        positions << QPointF(is_src ? 0 : 1,
                             ui->tree->visualItemRect(item).center().y());
    }
    if (ids.length() != 0)
        selectedSigs(ids, positions, is_src);
}

void SignalList::filterPatternChanged(const QString &pattern)
{
    printf("filterPatternChanged('%s')\n", pattern.toStdString().c_str());
    QTreeWidgetItem *dev, *sig;
    int i, j;
    // build a regular expression
    QRegExp regexp("("+pattern+")");
    for (i = 0; i < ui->tree->topLevelItemCount(); i++) {
        dev = ui->tree->topLevelItem(i);
        for (j = 0; j < dev->childCount(); j++) {
            sig = dev->child(j);
            sig->setHidden(regexp.indexIn(sig->text(0)) == -1);
        }
    }
    Q_EMIT updated();
}

void SignalList::filterPrefixChanged(const QString &prefix)
{
    ;
}


