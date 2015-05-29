#ifndef LISTTAB_H
#define LISTTAB_H

#include "mapperstuff.h"
#include "tab.h"

namespace Ui {
class ListTab;
}

class ListTab : public Tab
{
    Q_OBJECT

public:
    explicit ListTab(QTabWidget *parent = 0, MapperStuff *data = 0);
    ~ListTab();

    void update();
    void deviceEvent(mapper_db_device dev, mapper_db_action_t action);
    void signalEvent(mapper_db_signal sig, mapper_db_action_t action);
    void mapEvent(mapper_db_map map, mapper_db_action_t action);
    void signalUpdateEvent(mapper_signal sig, mapper_db_signal props,
                           int instance_id, void *value, int count,
                           mapper_timetag_t *timetag);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void updateMaps();
    void selectedMaps(QList<uint32_t> hashes);
    void selectedSigs(bool is_src, QList<QString> signames);

private:
    Ui::ListTab *ui;
    QList<QString> selectedSrcSigs;
    QList<QString> selectedDstSigs;
};

#endif // LISTTAB_H
