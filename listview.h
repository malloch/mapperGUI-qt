#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>

namespace Ui {
class ListView;
}

class ListView : public QWidget
{
    Q_OBJECT

public:
    explicit ListView(QWidget *parent = 0);
    ~ListView();

    void clear();
    void resize();
    void update();

    void addDevice(int index, const QString & name, int direction);
    void removeDevice(const QString & name);

    void addSignal(const QString &devname, const QString &signame,
                   QChar type, qreal length, int direction);
    void removeSignal(const QString &devname, const QString &signame);
    QPointF signalPosition(const QString &devname, const QString &signame);

    void addMap(uint32_t hash,
                const QString &srcdevname, const QString &srcsigname,
                const QString &dstdevname, const QString &dstsigname, bool muted);
    void removeMap(uint32_t hash);

signals:
    void updateMaps();
    void selectedMaps(QList<uint32_t> hashes);
    void selectedSigs(bool is_src, QList<QString> signames);

private:
    Ui::ListView *ui;
};

#endif // LISTVIEW_H
