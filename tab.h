#ifndef TAB_H
#define TAB_H

#include <QTabWidget>
#include "logic.h"

class Tab : public QTabWidget
{
public:
    Tab(QTabWidget *parent, mapperGUIData data);
private:
    mapperGUIData data;
};

#endif // TAB_H
