#include "signallist.h"
#include "ui_signallist.h"
#include "iostream"

SignalList::SignalList(QWidget *parent, const char *_label, int _is_src) :
    QWidget(parent),
    ui(new Ui::SignalList)
{
    ui->setupUi(this);
    ui->label->setText(_label);
    ui->tree->setColumnWidth(0, ui->widget->width()-120);
    ui->tree->setColumnWidth(1, 50);
    ui->tree->setColumnWidth(2, 50);
    is_src = _is_src;
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
    printf("SIGNALLIST ADDING SIGNAL\n");
    QTreeWidgetItem *qdev;
    QList<QTreeWidgetItem *> qdevlist = ui->tree->findItems(devname, Qt::MatchExactly, 0);
    if (!qdevlist.count()) {
        qdev = new QTreeWidgetItem((QTreeWidget*)0, QStringList(devname));
        ui->tree->insertTopLevelItem(0, qdev);
    }
    else
        qdev = qdevlist.takeAt(0);

    QTreeWidgetItem *qsig = new QTreeWidgetItem();
    qsig->setText(0, signame);
    qsig->setText(1, QString(type));
    qsig->setText(2, QString::number(length));
    qdev->addChild(qsig);
}

void SignalList::removeSignal(const QString &devname, const QString &signame)
{
    QTreeWidgetItem *qdev;
    QList<QTreeWidgetItem *> qdevlist = ui->tree->findItems(devname, Qt::MatchExactly, 0);
    if (!qdevlist.count()) {
        qdev = new QTreeWidgetItem((QTreeWidget*)0, QStringList(devname));
        ui->tree->insertTopLevelItem(0, qdev);
    }
    else
        qdev = qdevlist.takeAt(0);

    int numsigs = qdev->childCount();
    for (int i = 0; i < numsigs; i++) {
        printf("child[%d] text = %s\n", i, qdev->child(i)->text(0).toStdString().c_str());
    }
//    QList <QTreeWidgetItem *> qsiglist = qdev->

//    qdev->removeChild(qdev->);
//    QTreeWidgetItem *qsig = new QTreeWidgetItem();
//    qsig->setText(0, signame);
//    qsig->setText(1, QString(type));
//    qsig->setText(2, QString::number(length));
//    qdev->addChild(qsig);
}

float SignalList::signalOffset(const QString & name)
{
    QList<QTreeWidgetItem *> matched = ui->tree->findItems(name, Qt::MatchRecursive, 0);
    if (matched.isEmpty())
        return -1;
    QTreeWidgetItem *item = matched.first();
    QRect rect = ui->tree->visualItemRect(item);
    std::cout << "found item" << item->text(0).toStdString() << " at " << rect.top() << std::endl;
//    return rect.top() + rect.height()*0.5;
    return rect.top();
}
