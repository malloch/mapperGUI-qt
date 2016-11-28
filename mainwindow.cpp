#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    MapperWidget(parent, 0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->tabs->layout()->setContentsMargins(0, 0, 0, 0);
//    ui->tabs->layout()->setSpacing(0);
//    ui->tabs->layout()->setContentsMargins(0, 0, 0, 0);
//    setCentralWidget(ui->tabs);

//    data = new MapperStuff();

    // try adding a new ListTab
    ListTab *listTab = new ListTab(ui->tabs, mapper_data);
    tabs << listTab;
    ui->tabs->addTab(listTab, "List");

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
}

void MainWindow::poll()
{
    int count = mapper_data->poll();
    if (mapper_data->ready == false) {
        mapper_data->ready = true;
        setWindowTitle("mapperGUI-qt");
    }
    if (count) {
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
    }

    // TODO: don't update unless tab is selected
//    data->now = data->device.now();
    for (auto tab : tabs)
        tab->update();
}

void MainWindow::update()
{
    ;
}

void MainWindow::deviceEvent(const mapper::Device& dev, mapper_record_action action)
{
    ;
}

void MainWindow::signalEvent(const mapper::Signal& sig, mapper_record_action action)
{
    ;
}

void MainWindow::mapEvent(const mapper::Map& map, mapper_record_action action)
{
    ;
}

void MainWindow::signalUpdateEvent(const mapper::Signal& sig, mapper_id instance,
                                   const void *value, int count,
                                   mapper_timetag_t *timetag)
{
    ;
}

//void MainWindow::on_views_tabBarClicked(int index)
//{
//    printf("tab %i clicked\n", index);
//}
