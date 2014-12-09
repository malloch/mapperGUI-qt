#include "connectionprops2.h"
#include "ui_connectionprops2.h"

ConnectionProps2::ConnectionProps2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionProps2)
{
    ui->setupUi(this);
}

ConnectionProps2::~ConnectionProps2()
{
    delete ui;
}
