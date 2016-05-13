#include "mapprops.h"
#include "ui_mapprops.h"

MapProps::MapProps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapProps)
{
    ui->setupUi(this);

    connect(ui->checkBoxMuted, SIGNAL(toggled(bool)),
                this, SIGNAL(muteChecked(bool)));
//    connect(ui->modeButtonLinear, SIGNAL(triggered(QAction*)),
//            this, SIGNAL(linearModeButtonPressed()));
//    connect(ui->modeButtonExpression, SIGNAL(pressed(QAction*)),
//            this, SIGNAL(expressionModeButtonPressed()));
//    connect(ui->expression, SIGNAL(textChanged()),
//            this, SIGNAL(expressionChanged()));
}

MapProps::~MapProps()
{
    delete ui;
}

void MapProps::displayProps(QList<int> mode, QList<bool> muted,
                            QList<QString> expression)
{
    ui->modeTabs->setEnabled(true);
//    ui->modeButtonLinear->setEnabled(true);
//    ui->modeTabs->setTabEnabled(1, true);
//    ui->modeButtonExpression->setEnabled(true);
    switch (mode[0]) {
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
    ui->checkBoxMuted->setChecked(muted[0]);

    ui->expression->setEnabled(true);
    ui->expression->setPlainText(expression[0]);
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
