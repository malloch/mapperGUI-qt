#include "signallist.h"
#include "ui_signallist.h"
#include "iostream"

SignalList::SignalList(QWidget *parent, const char *_label, int _is_src) :
    QWidget(parent),
    ui(new Ui::SignalList)
{
    ui->setupUi(this);
    ui->label->setText(_label);
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
    QTreeWidgetItem *qdev = new QTreeWidgetItem((QTreeWidget*)0,
                                                QStringList(name));
    ui->tree->insertTopLevelItem(index, qdev);
    return ui->tree->indexOfTopLevelItem(qdev);
}

void SignalList::addSignal(int parentindex, const QString &name, QChar type,
                           qreal length)
{
    QTreeWidgetItem *qdev = ui->tree->topLevelItem(parentindex);

    QTreeWidgetItem *qsig = new QTreeWidgetItem();
    qsig->setText(0, name);
    qsig->setText(1, QString(type));
    qsig->setText(2, QString::number(length));
    qdev->addChild(qsig);
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
