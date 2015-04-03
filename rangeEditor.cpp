#include "rangeeditor.h"
#include "ui_rangeeditor.h"

RangeEditor::RangeEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RangeEditor)
{
    ui->setupUi(this);
}

RangeEditor::~RangeEditor()
{
    delete ui;
}
