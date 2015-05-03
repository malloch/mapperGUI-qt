#include "signalsearchbar.h"
#include "ui_signalsearchbar.h"

SignalSearchBar::SignalSearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalSearchBar)
{
    ui->setupUi(this);
}

SignalSearchBar::~SignalSearchBar()
{
    delete ui;
}
