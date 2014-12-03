#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "logic.h"
#include "graphtab.h"

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

//    void on_addTab_clicked();

//    void on_views_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;
    mapperGUIData data;
    GraphTab *overviewTab;
    int numTabs;
    int currentTab;
//    tab *tabs;
};

#endif // MAINWINDOW_H
