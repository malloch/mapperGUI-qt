#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "logic.h"
//#include "graphtab.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    data = (mapperGUIData) calloc(1, sizeof(struct _mapperGUIData));

    data->monitor = mapper_monitor_new(0, SUB_DEVICE);
    data->db = mapper_monitor_get_db(data->monitor);
    mapper_db_add_device_callback(data->db, deviceHandler, &data->deviceFlags);
    mapper_db_add_link_callback(data->db, linkHandler, &data->linkFlags);
    mapper_db_add_signal_callback(data->db, signalHandler, &data->signalFlags);
    mapper_db_add_connection_callback(data->db, connectionHandler, &data->connectionFlags);

    // try adding a new graphTab
    overviewTab = new GraphTab(ui->views, data);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(poll()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    // save mapping?
    // close all tabs
    delete ui;
    if (data) {
        if (data->device)
            mdev_free(data->device);
        if (data->db) {
            mapper_db_remove_device_callback(data->db, deviceHandler, &data->deviceFlags);
            mapper_db_remove_link_callback(data->db, linkHandler, &data->linkFlags);
            mapper_db_remove_signal_callback(data->db, signalHandler, &data->signalFlags);
            mapper_db_remove_connection_callback(data->db, connectionHandler, &data->connectionFlags);
        }
        if (data->monitor)
            mapper_monitor_free(data->monitor);
        if (data->admin)
            mapper_admin_free(data->admin);
        free(data);
    }
}

void MainWindow::poll()
{
    int count = mapper_monitor_poll(data->monitor, 0);
    if (count)
        printf("received %i messages from monitor\n", count);
    // drawing updates depend on current mode...
    if (data->deviceFlags) {
        // for now assume that the overview graph tab is selected
        printf("should be calling overviewTab deviceEvent()\n");
        overviewTab->deviceEvent();

        ui->srcTree->clear();
        ui->dstTree->clear();
        mapper_db_device *pdev = mapper_db_get_all_devices(data->db);
        while (pdev) {
            if ((*pdev)->num_outputs > 0) {
                QTreeWidgetItem *dev = new QTreeWidgetItem((QTreeWidget*)0,
                                                           QStringList(QString((*pdev)->name)));
                ui->srcTree->insertTopLevelItem(0, dev);
                mapper_db_signal *psig = mapper_db_get_outputs_by_device_name(data->db, (*pdev)->name);
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
                mapper_db_signal *psig = mapper_db_get_inputs_by_device_name(data->db, (*pdev)->name);
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
        data->deviceFlags = 0;
    }
    if (data->linkFlags) {
        overviewTab->linkEvent();
        data->linkFlags = 0;
    }
//    if (data->connectionFlags) {
//        QGraphicsScene *scene = new QGraphicsScene(this);
//        ui->links->setScene(scene);

//        scene->addRect(100, 0, 80, 100, QPen(Qt::black), QBrush(Qt::blue));
//        float width = ui->links->width();
//        QPainterPath *path = new QPainterPath();
//        path->moveTo(0, 0);
//        path->cubicTo(20, 0, 40, 40, 60, 40);
//        scene->addPath(*path, QPen(Qt::black, 2));
//        data->connectionFlags = 0;
//    }
    overviewTab->update();
}

//void MainWindow::on_views_tabBarClicked(int index)
//{
//    printf("tab %i clicked\n", index);
//}
