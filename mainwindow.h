#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tab.h"

#include "graphtab.h"
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
    MapperStuff *data;
    QList<Tab *> tabs;
    GraphTab *overviewTab;
    ListTab *listTab;
    int numTabs;
    int currentTab;
};

#endif // MAINWINDOW_H
