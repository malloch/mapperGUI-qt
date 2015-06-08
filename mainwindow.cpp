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

    data = new MapperStuff();

    // try adding a new ListTab
    ListTab *listTab = new ListTab(ui->tabs, data);
    tabs << listTab;
    ui->tabs->addTab(listTab, "List");

//    // try adding a new GraphTab
//    GraphTab *graphTab = new GraphTab(ui->tabs, data);
//    tabs << graphTab;
//    ui->tabs->addTab(graphTab, "Overview");
    ui->tabs->setCurrentIndex(0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(poll()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    // TODO: save mapping(s)
    // close all tabs
    delete ui;
    delete data;
}

void MainWindow::poll()
{
    int count = data->poll();
    if (data->ready == false && data->device.ready()) {
        data->ready = true;
        setWindowTitle(QString::fromStdString(data->device.name()));
    }
//    if (count) {
//        // drawing updates depend on current mode...
//        if (data->deviceFlags) {
//            foreach (Tab *tab, tabs)
//                tab->deviceEvent();
//            data->deviceFlags = 0;
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

    // TODO: don't update unless tab is selected
    data->now = data->device.now();
    foreach (Tab *tab, tabs)
        tab->update();
}

//void MainWindow::on_views_tabBarClicked(int index)
//{
//    printf("tab %i clicked\n", index);
//}
