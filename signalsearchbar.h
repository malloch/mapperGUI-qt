#ifndef SIGNALSEARCHBAR_H
#define SIGNALSEARCHBAR_H

#include <QWidget>

namespace Ui {
class SignalSearchBar;
}

class SignalSearchBar : public QWidget
{
    Q_OBJECT

public:
    explicit SignalSearchBar(QWidget *parent = 0);
    ~SignalSearchBar();

    void addDevice(qulonglong id, const QString& name);
    void removeDevice(qulonglong id);

Q_SIGNALS:
    void patternChanged(const QString& text);
    void prefixChanged(const QString& text);

private:
    Ui::SignalSearchBar *ui;
};

#endif // SIGNALSEARCHBAR_H
