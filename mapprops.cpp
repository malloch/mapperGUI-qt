#include <QDebug>
#include "mapprops.h"
#include "ui_mapprops.h"

//TODO: syntax highlighting

MapProps::MapProps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapProps)
{
    ui->setupUi(this);

    connect(ui->checkBoxMuted, SIGNAL(toggled(bool)),
                this, SIGNAL(setMuted(bool)));
//    connect(ui->modeButtonLinear, SIGNAL(triggered(QAction*)),
//            this, SIGNAL(linearModeButtonPressed()));
//    connect(ui->modeButtonExpression, SIGNAL(pressed(QAction*)),
//            this, SIGNAL(expressionModeButtonPressed()));
    connect(ui->expression, SIGNAL(returnPressed()),
            this, SLOT(expressionChanged()));
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
    // check if expressions are identical
    bool multipleExpr = false;
    for (int i = 1; i < expression.length(); i++) {
        if (expression[i].compare(expression[i-1])) {
            multipleExpr = true;
            break;
        }
    }
    if (multipleExpr)
        ui->expression->setText("multiple expressions");
    else {
        ui->expression->setText(expression[0]);
        // check if expr is new
        QString old = ui->history->toPlainText();
        qDebug() << "hist: " << old;
        if (!old.endsWith("\n"+expression[0]))
            ui->history->appendPlainText(expression[0]);
    }
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

void MapProps::setMuted(int state)
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

void MapProps::expressionChanged()
{
    QString expr = ui->expression->text();
    qDebug() << "MapProps::expressionChanged to " << expr;
    if (expr.isNull() || expr.isEmpty())
        return;
    // check if expr is new
    QString old = ui->history->toPlainText();
    qDebug() << "hist: " << old;
    if (!old.endsWith("\n"+expr))
        ui->history->appendPlainText(expr);
    Q_EMIT setExpression(expr);
}
