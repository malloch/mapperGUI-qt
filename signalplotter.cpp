#include "signalplotter.h"
#include "ui_signalplotter.h"

SignalPlotter::SignalPlotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalPlotter)
{
    ui->setupUi(this);
    ui->plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->plot->xAxis->setAutoTickStep(false);
    ui->plot->xAxis->setTickStep(2);
    ui->plot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->plot->yAxis2, SLOT(setRange(QCPRange)));
}

SignalPlotter::~SignalPlotter()
{
    delete ui;
}

void SignalPlotter::addPlot(const QString &name)
{
    // check if plot already exists
    foreach(const SignalPlot* plot, plots) {
        if (plot->name.compare(name)==0)
            return;
    }
    plots << new SignalPlot(name, indexCounter++);
    ui->plot->addGraph();
}

void SignalPlotter::removePlot(const QString &name)
{
    for (int i = 0; i < plots.length(); i++) {
        SignalPlot *plot = plots.at(i);
        if (plot->name.compare(name)==0) {
            ui->plot->removeGraph(plot->index);
            plots.removeAll(plot);
            return;
        }
    }
}

// TODO: extend for bector signal values
void SignalPlotter::updatePlot(const QString &name, double value, double time)
{
    foreach(const SignalPlot* plot, plots) {
        if (plot->name.compare(name)==0) {
            ui->plot->graph(plot->index)->addData(time, value);
            ui->plot->graph(plot->index)->removeDataBefore(time-8);
            return;
        }
    }
}

void SignalPlotter::update(double now)
{
//    qDebug() << "SignalPlotter::update";
    return;
    foreach(const SignalPlot *plot, plots) {
        ui->plot->graph(plot->index)->rescaleValueAxis();
    }
    ui->plot->xAxis->setRange(now + 0.25, 8, Qt::AlignRight);
    ui->plot->replot();
}
