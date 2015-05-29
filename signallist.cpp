#include "signallist.h"
#include "ui_signallist.h"
#include "iostream"
#include <QDebug>
#include <QAbstractItemView>

SignalList::SignalList(QWidget *parent, const char *_label, int _is_src) :
    QWidget(parent),
    ui(new Ui::SignalList)
{
    ui->setupUi(this);
    ui->label->setText(_label);
    ui->tree->setColumnWidth(0, ui->widget->width()-120);
    ui->tree->setColumnWidth(1, 50);
    ui->tree->setColumnWidth(2, 50);
    ui->tree->setColumnHidden(3, true);
    ui->tree->setAlternatingRowColors(true);
    is_src = _is_src;

//    ui->tree->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(ui->tree, SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this, SIGNAL(updateMaps()));
    connect(ui->tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this, SIGNAL(updateMaps()));
    connect(ui->tree, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

    // also trigger redraw when columns are sorted
    ui->tree->header()->setSectionsClickable(true);
    connect(ui->tree->header(), SIGNAL(sectionClicked(int)),
            this, SIGNAL(updateMaps()));
}

SignalList::~SignalList()
{
    delete ui;
}

void SignalList::clear()
{
    ui->tree->clear();
}

int SignalList::addDevice(int index, const QString &name)
{
    QList<QTreeWidgetItem *> qdevlist = ui->tree->findItems(name, Qt::MatchExactly, 0);
    if (qdevlist.count())
        return -1;
    QTreeWidgetItem *qdev = new QTreeWidgetItem((QTreeWidget*)0,
                                                QStringList(name));
    ui->tree->insertTopLevelItem(index, qdev);
    ui->tree->expandItem(qdev);
    return ui->tree->indexOfTopLevelItem(qdev);
}

void SignalList::removeDevice(const QString &name)
{
    QList<QTreeWidgetItem *> qdevlist = ui->tree->findItems(name, Qt::MatchExactly, 0);
    int num = qdevlist.count();
    for (int i = 0; i < num; i++)
        delete(qdevlist.takeAt(0));
}

void SignalList::addSignal(const QString &devname, const QString &signame, QChar type,
                           qreal length)
{
    QTreeWidgetItem *qdev;
    QList<QTreeWidgetItem *> qlist = ui->tree->findItems(devname, Qt::MatchExactly | Qt::MatchRecursive, 0);
    if (!qlist.count()) {
        qdev = new QTreeWidgetItem((QTreeWidget*)0, QStringList(devname));
        ui->tree->insertTopLevelItem(0, qdev);
    }
    else
        qdev = qlist.takeAt(0);

    QString qkey;
    qkey.append(devname);
    qkey.append("/");
    qkey.append(signame);

    qlist = ui->tree->findItems(qkey, Qt::MatchExactly, 3);
    if (qlist.count()) {
        // entry already exists
        // TODO: update metadata
        return;
    }

    QTreeWidgetItem *qsig = new QTreeWidgetItem();
    qsig->setText(0, signame);
    qsig->setText(1, QString(type));
    qsig->setText(2, QString::number(length));
    qsig->setText(3, qkey);
    qdev->addChild(qsig);
}

void SignalList::removeSignal(const QString &devname, const QString &signame)
{
    QString qkey;
    qkey.append(devname);
    qkey.append("/");
    qkey.append(signame);

    QList<QTreeWidgetItem *> qlist = ui->tree->findItems(qkey, Qt::MatchExactly | Qt::MatchRecursive, 3);
    if (!qlist.count())
        return;

    int num = qlist.count();
    for (int i = 0; i < num; i++)
        delete(qlist.takeAt(0));
}

QPointF SignalList::signalPosition(const QString &devname, const QString & signame)
{
    QString qkey;
    qkey.append(devname);
    qkey.append("/");
    qkey.append(signame);

    QList<QTreeWidgetItem *> qlist = ui->tree->findItems(qkey, Qt::MatchExactly | Qt::MatchRecursive, 3);
    if (!qlist.count())
        return QPointF(0, 0);

    QTreeWidgetItem *item = qlist.first();
    if (item->parent()->isExpanded()) {
        QRect rect = ui->tree->visualItemRect(item);
        return QPointF(is_src ? 0 : 1, rect.center().y());
    }
    else {
        QRect rect = ui->tree->visualItemRect(item->parent());
        return QPointF(is_src ? 0 : 1, rect.center().y());
    }
}

void SignalList::selectionChanged()
{
    qDebug() << "selectionChanged(), is_src:" << is_src << "selections" << ui->tree->selectedItems();
    QList<QTreeWidgetItem*> selected = ui->tree->selectedItems();
    // pass list of selected signal names to parent
    QList<QString> names;
    foreach(QTreeWidgetItem *item, selected) {
        QString name = item->text(3);
        if (name.length())
            names << name;
    }
    selectedSigs(is_src, names);
}
