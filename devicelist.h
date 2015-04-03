#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QWidget>

namespace Ui {
class DeviceList;
}

class DeviceList : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceList(QWidget *parent, const char *_label);
    ~DeviceList();

    void clear();
    void update();

private:
//    Ui::DeviceList *ui;
    // selected
};

#endif // DEVICELIST_H
