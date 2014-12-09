#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->centralWidget->layout()->setSpacing(0);
//    ui->tabs->layout()->setContentsMargins(0, 0, 0, 0);
//    setCentralWidget(ui->tabs);

    data = (mapperGUIData) calloc(1, sizeof(struct _mapperGUIData));

    data->admin = mapper_admin_new(0, 0, 0);
    data->monitor = mapper_monitor_new(data->admin, SUB_DEVICE);
    data->db = mapper_monitor_get_db(data->monitor);
    mapper_db_add_device_callback(data->db, deviceHandler, &data->deviceFlags);
    mapper_db_add_link_callback(data->db, linkHandler, &data->linkFlags);
    mapper_db_add_signal_callback(data->db, signalHandler, &data->signalFlags);
    mapper_db_add_connection_callback(data->db, connectionHandler, &data->connectionFlags);

    data->device = mdev_new("_monitor", 0, data->admin);

    // try adding a new ListTab
    listTab = new ListTab(ui->tabs, data);
    tabs << listTab;

    // try adding a new GraphTab
    overviewTab = new GraphTab(ui->tabs, data);
    tabs << overviewTab;
    ui->tabs->setCurrentIndex(0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(poll()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    // save mapping(s)?
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
    mdev_poll(data->device, 0);
    if (count) {
        // drawing updates depend on current mode...
        if (data->deviceFlags) {
            printf("Should be calling device events...\n");
            // for now assume that the overview graph tab is selected
            foreach (Tab *tab, tabs)
                tab->deviceEvent();
            data->deviceFlags = 0;
        }
        if (data->linkFlags) {
            foreach(Tab *tab, tabs)
                tab->linkEvent();
            data->linkFlags = 0;
        }
    }

    // TODO: don't update unless tab is selected
    foreach (Tab *tab, tabs)
        tab->update();
}

//void MainWindow::on_views_tabBarClicked(int index)
//{
//    printf("tab %i clicked\n", index);
//}
