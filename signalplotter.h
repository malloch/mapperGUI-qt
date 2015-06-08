#ifndef SIGNALPLOTTER_H
#define SIGNALPLOTTER_H

#include <QWidget>
#include "qcustomplot.h"

class SignalPlot
{
public:
    SignalPlot(const QString& _name, int _index) : name(_name), index(_index)
    {
        plotter = new QCustomPlot();
    };
    QCustomPlot *plotter;
    QString name;
    int index;
};

namespace Ui {
class SignalPlotter;
}

class SignalPlotter : public QWidget
{
    Q_OBJECT

public:
    explicit SignalPlotter(QWidget *parent = 0);
    ~SignalPlotter();

    void addPlot(const QString& name);
    void removePlot(const QString& name = 0);
    void update(double now);

public slots:
    void updatePlot(const QString &name, double value, double time);

private:
    Ui::SignalPlotter *ui;
    QList<SignalPlot*> plots;
    int indexCounter;
};

#endif // SIGNALPLOTTER_H
