#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "listtab.h"
#include <QMainWindow>
#include <QTimer>

enum t_tabType {
    TAB_TYPE_LIST,
    TAB_TYPE_GRAPH
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, MapperWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    void update();
//    void deviceEvent(mapper_device dev, mapper_record_action action) Q_DECL_OVERRIDE;
//    void signalEvent(mapper_signal sig, mapper_record_action action) Q_DECL_OVERRIDE;
//    void mapEvent(mapper_map map, mapper_record_action action) Q_DECL_OVERRIDE;
    void update();
    void deviceEvent(const mapper::Device& dev, mapper_record_event event) Q_DECL_OVERRIDE;
    void signalEvent(const mapper::Signal& sig, mapper_record_event event) Q_DECL_OVERRIDE;
    void mapEvent(const mapper::Map& map, mapper_record_event event) Q_DECL_OVERRIDE;
    void signalUpdateEvent(const mapper::Signal& sig, mapper_id instance,
                           const void *value, int count,
                           mapper_timetag_t *timetag) Q_DECL_OVERRIDE;


private Q_SLOTS:
    void poll();

private:
    Ui::MainWindow *ui;
//    MapperWidget *mapper;
    QList<Tab *> tabs;
//    GraphTab *overviewTab;
//    ListTab *listTab;
    int numTabs;
    int currentTab;
};

#endif // MAINWINDOW_H
