#include "mapprops.h"
#include "ui_mapprops.h"

MapProps::MapProps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapProps)
{
    ui->setupUi(this);
}

MapProps::~MapProps()
{
    delete ui;
}

void MapProps::displayProps(int mode, bool muted, const QString& expression)
{
    ui->modeTabs->setEnabled(true);
//    ui->modeButtonLinear->setEnabled(true);
//    ui->modeTabs->setTabEnabled(1, true);
//    ui->modeButtonExpression->setEnabled(true);
    switch (mode) {
    case _MO_LINEAR:
        ui->modeTabs->setCurrentIndex(1);
        break;
    case _MO_EXPRESSION:
        ui->modeTabs->setCurrentIndex(0);
//        ui->modeButtonExpression->setDown(true);
        break;
    default:
        break;
    }

    ui->checkBoxMuted->setEnabled(true);
    ui->checkBoxMuted->setChecked(muted);

    ui->expression->setEnabled(true);
    ui->expression->setPlainText(expression);
}


void MapProps::clearProps()
{
    ui->modeTabs->setEnabled(false);
    ui->modeTabs->setCurrentIndex(0);
    ui->expression->clear();
//    ui->expression->setEnabled(false);

    ui->checkBoxMuted->setChecked(false);
    ui->checkBoxMuted->setEnabled(false);

//    ui->checkBoxCalibrating->setChecked(false);
//    ui->checkBoxCalibrating->setEnabled(false);
}

void MapProps::setMuting(int state)
{
    Qt::CheckState checkState;
    switch (state) {
    case 1:
        checkState = Qt::Checked;
        break;
    case 2:
        checkState = Qt::PartiallyChecked;
        break;
    default:
        checkState = Qt::Unchecked;
        break;
    }
    ui->checkBoxMuted->setCheckState(checkState);
}
