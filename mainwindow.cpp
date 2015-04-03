#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->tabs->layout()->setContentsMargins(0, 0, 0, 0);
//    ui->tabs->layout()->setSpacing(0);
//    ui->tabs->layout()->setContentsMargins(0, 0, 0, 0);
//    setCentralWidget(ui->tabs);

    data = (mapperGUIData) calloc(1, sizeof(struct _mapperGUIData));

//    data->admin = new mapper::Admin();
    data->monitor = new mapper::Monitor(SUBSCRIBE_DEVICE);
//    data->db = data->monitor->db();
//    data->db.add_device_callback(deviceHandler, &data->deviceFlags);
//    data->db.add_link_callback(linkHandler, &data->linkFlags);
//    data->db.add_signal_callback(signalHandler, &data->signalFlags);
//    data->db.add_connection_callback(connectionHandler, &data->connectionFlags);

//    data->device = new mapper::Device("_monitor");

//    // try adding a new ListTab
//    listTab = new ListTab(ui->tabs, data);
//    tabs << listTab;

//    // try adding a new GraphTab
//    overviewTab = new GraphTab(ui->tabs, data);
//    tabs << overviewTab;
//    ui->tabs->setCurrentIndex(0);

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
        if (data->monitor)
            mmon_free(data->monitor);
        if (data->admin)
            mapper_admin_free(data->admin);
        free(data);
    }
}

void MainWindow::poll()
{
//    int count = mapper_monitor_poll(data->monitor, 0);
//    printf("monitor count: %d\n", count);
    return;
//    mdev_poll(data->device, 0);
//    if (count) {
//        // drawing updates depend on current mode...
//        if (data->deviceFlags) {
//            foreach (Tab *tab, tabs)
//                tab->deviceEvent();
//            data->deviceFlags = 0;
//        }
//        if (data->linkFlags) {
//            foreach(Tab *tab, tabs)
//                tab->linkEvent();
//            data->linkFlags = 0;
//        }
//        if (data->signalFlags) {
//            foreach(Tab *tab, tabs)
//                tab->signalEvent();
//            data->signalFlags = 0;
//        }
//        if (data->connectionFlags) {
//            foreach(Tab *tab, tabs)
//                tab->connectionEvent();
//            data->connectionFlags = 0;
//        }
//    }

//    // TODO: don't update unless tab is selected
//    foreach (Tab *tab, tabs)
//        tab->update();
}

//void MainWindow::on_views_tabBarClicked(int index)
//{
//    printf("tab %i clicked\n", index);
//}

void deviceHandler(mapper_db_device dev, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from device '%s'\n", dev->name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void signalHandler(mapper_db_signal sig, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from signal '%s'\n", sig->name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void connectionHandler(mapper_db_connection con, mapper_db_action_t action, void *user)
{
//    printf("QtMonitor got update from connection '%s'->'%s'\n", con->src_name, con->dest_name);
    int *flags = (int*)user;
    *flags |= 1 << action;
}

void signalHandler(mapper_signal sig, mapper_db_signal props, int instance_id,
                   void *value, int count, mapper_timetag_t *timetag)
{
    ;
}
