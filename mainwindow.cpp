#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    monitor = mapper_monitor_new(0, SUB_DEVICE);
    db = mapper_monitor_get_db(monitor);
    mapper_db_add_device_callback(db, deviceHandler, &updateFlags);
    mapper_db_add_link_callback(db, linkHandler, &updateFlags);
    mapper_db_add_signal_callback(db, signalHandler, &updateFlags);
    mapper_db_add_connection_callback(db, connectionHandler, &updateFlags);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(poll()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (monitor) {
        mapper_monitor_free(monitor);
    }
}

void MainWindow::poll()
{
    int count = mapper_monitor_poll(monitor, 0);
    if (count)
        printf("received %i messages from monitor\n", count);
    // drawing updates depend on current mode...
    if (updateFlags & DEVICES_UPDATED) {
        ui->srcTree->clear();
        ui->dstTree->clear();
        mapper_db_device *pdev = mapper_db_get_all_devices(db);
        while (pdev) {
            if ((*pdev)->num_outputs > 0) {
                QTreeWidgetItem *dev = new QTreeWidgetItem((QTreeWidget*)0,
                                                           QStringList(QString((*pdev)->name)));
                ui->srcTree->insertTopLevelItem(0, dev);
                mapper_db_signal *psig = mapper_db_get_outputs_by_device_name(db, (*pdev)->name);
                while (psig) {
                    QTreeWidgetItem *sig = new QTreeWidgetItem();
                    sig->setText(0, QString((*psig)->name));
                    sig->setText(1, QString(QChar((*psig)->type)));
                    sig->setText(2, QString::number((*psig)->length));
                    dev->addChild(sig);
                    psig = mapper_db_signal_next(psig);
                }
            }
            if ((*pdev)->num_inputs > 0) {
                QTreeWidgetItem *dev = new QTreeWidgetItem((QTreeWidget*)0,
                                                           QStringList(QString((*pdev)->name)));
                ui->dstTree->insertTopLevelItem(0, dev);
                mapper_db_signal *psig = mapper_db_get_inputs_by_device_name(db, (*pdev)->name);
                while (psig) {
                    QTreeWidgetItem *sig = new QTreeWidgetItem();
                    sig->setText(0, QString((*psig)->name));
                    sig->setText(1, QString(QChar((*psig)->type)));
                    sig->setText(2, QString::number((*psig)->length));
                    dev->addChild(sig);
                    psig = mapper_db_signal_next(psig);
                }
            }
            pdev = mapper_db_device_next(pdev);
        }
        updateFlags &= ~DEVICES_UPDATED;
    }
    if (updateFlags & CONNECTIONS_UPDATED) {
        QGraphicsScene *scene = new QGraphicsScene(this);
        ui->links->setScene(scene);

        scene->addRect(100, 0, 80, 100, QPen(Qt::black), QBrush(Qt::blue));
        float width = ui->links->width();
        QPainterPath *path = new QPainterPath();
        path->moveTo(0, 0);
        path->cubicTo(20, 0, 40, 40, 60, 40);
        scene->addPath(*path, QPen(Qt::black, 2));
    }
}

void MainWindow::on_addTab_clicked()
{
//    ui->views.addTab("foo");
//    ui->views->addTab(ui->tab_4, 0);
}

void MainWindow::on_views_tabBarClicked(int index)
{
    printf("tab %i clicked\n", index);
}
