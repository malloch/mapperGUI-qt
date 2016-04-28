#include "signalsearchbar.h"
#include "ui_signalsearchbar.h"

SignalSearchBar::SignalSearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalSearchBar)
{
    ui->setupUi(this);

    connect(ui->pattern, SIGNAL(textEdited(const QString&)),
            this, SIGNAL(patternChanged(const QString&)));
    connect(ui->prefix, SIGNAL(currentIndexChanged(const QString&)),
            this, SIGNAL(prefixChanged(const QString&)));
}

SignalSearchBar::~SignalSearchBar()
{
    delete ui;
}

void SignalSearchBar::addDevice(qulonglong id, const QString &name)
{
    ui->prefix->addItem(name, id);
}

void SignalSearchBar::removeDevice(qulonglong id)
{
    int index = ui->prefix->findData(id);
    if (index != -1)
        ui->prefix->removeItem(index);
}
