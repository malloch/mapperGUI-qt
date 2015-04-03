#ifndef TAB_H
#define TAB_H

#include <mapperstuff.h>
#include <QTabWidget>

class Tab : public QTabWidget
{
public:
    Tab(QTabWidget *_parent, mapperGUIData _data);
    ~Tab();

    virtual void update() = 0;
    virtual void deviceEvent() = 0;
    virtual void signalEvent() = 0;
    virtual void linkEvent() = 0;
    virtual void connectionEvent() = 0;

protected:
    mapperGUIData data;
};

#endif // TAB_H
