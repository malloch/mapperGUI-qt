#include "connectionprops.h"
#include "ui_connectionprops.h"

ConnectionProps::ConnectionProps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionProps)
{
    ui->setupUi(this);
}

ConnectionProps::~ConnectionProps()
{
    delete ui;
}

void ConnectionProps::displayProps(int mode, bool muted, bool calibrating,
                                   const QString& expression)
{
    ui->modeButtonLinear->setEnabled(true);
    ui->modeButtonExpression->setEnabled(true);
    switch (mode) {
    case _MO_LINEAR:
        ui->modeButtonLinear->setDown(true);
        break;
    case _MO_EXPRESSION:
        ui->modeButtonExpression->setDown(true);
        break;
    default:
        break;
    }

    ui->checkBoxMuted->setEnabled(true);
    ui->checkBoxMuted->setChecked(muted);

    ui->checkBoxCalibrating->setEnabled(true);
    ui->checkBoxCalibrating->setChecked(calibrating);

    ui->expression->setEnabled(true);
    ui->expression->setPlainText(expression);
}


void ConnectionProps::clearProps()
{
    ui->modeButtonLinear->setChecked(false);
    ui->modeButtonLinear->setEnabled(false);
    ui->modeButtonExpression->setChecked(false);
    ui->modeButtonExpression->setEnabled(false);

    ui->checkBoxMuted->setChecked(false);
    ui->checkBoxMuted->setEnabled(false);
    ui->checkBoxCalibrating->setChecked(false);
    ui->checkBoxCalibrating->setEnabled(false);

    ui->expression->clear();
    ui->expression->setEnabled(false);
}
