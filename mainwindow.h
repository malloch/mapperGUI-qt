#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tab.h"

#include "graphtab.h"
#include "listtab.h"
#include <QMainWindow>
#include <QTimer>

// function prototypes
void deviceHandler(mapper_db_device dev, mapper_db_action_t a, void *user);
void signalHandler(mapper_db_signal sig, mapper_db_action_t a, void *user);
void connectionHandler(mapper_db_connection con, mapper_db_action_t a, void *user);

enum t_tabType {
    TAB_TYPE_LIST,
    TAB_TYPE_GRAPH
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void poll();

private:
    Ui::MainWindow *ui;
    mapperGUIData data;
    QList<Tab *> tabs;
    GraphTab *overviewTab;
    ListTab *listTab;
    int numTabs;
    int currentTab;
};

#endif // MAINWINDOW_H
