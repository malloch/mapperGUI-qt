#ifndef LISTTAB_H
#define LISTTAB_H

#include "tab.h"
#include "signallist.h"
#include "linkview.h"
#include "connectionprops.h"

#include <QSplitter>
#include <QGroupBox>
#include <QVBoxLayout>

class ListTab : public Tab
{
//    Q_OBJECT

public:
    ListTab(QTabWidget *parent, mapperGUIData _data);
    ~ListTab();

    void update();
    void deviceEvent();
    void signalEvent();
    void connectionEvent();

private:
    ConnectionProps *props;
    SignalList *sources;
    SignalList *destinations;
    LinkView *links;
};

#endif // LISTTAB_H
