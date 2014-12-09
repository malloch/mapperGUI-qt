#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "logic.h"
#include "tab.h"
#include "graphtab.h"
#include "listtab.h"

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
