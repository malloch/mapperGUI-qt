#include "livesignalview.h"
#include "ui_livesignalview.h"

LiveSignalView::LiveSignalView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LiveSignalView)
{
    ui->setupUi(this);
}

LiveSignalView::~LiveSignalView()
{
    delete ui;
}
