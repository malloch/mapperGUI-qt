#include "modeeditor.h"
#include "ui_modeeditor.h"

ModeEditor::ModeEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeEditor)
{
    ui->setupUi(this);
}

ModeEditor::~ModeEditor()
{
    delete ui;
}
