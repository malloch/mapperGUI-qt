#ifndef LISTTAB_H
#define LISTTAB_H

#include "tab.h"

namespace Ui {
class ListTab;
}

class ListTab : public Tab
{
    Q_OBJECT

public:
    explicit ListTab(QTabWidget *parent = 0, MapperStuff *_data = 0);
    ~ListTab();

    void update();
    void deviceEvent(const mapper::Device& dev, mapper_record_action action) Q_DECL_OVERRIDE;
    void signalEvent(const mapper::Signal& sig, mapper_record_action action) Q_DECL_OVERRIDE;
    void mapEvent(const mapper::Map& map, mapper_record_action action) Q_DECL_OVERRIDE;
    void signalUpdateEvent(const mapper::Signal& sig, mapper_id instance,
                           const void *value, int count,
                           mapper_timetag_t *timetag) Q_DECL_OVERRIDE;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
//    bool eventFilter(QObject *object, QEvent *event);

public Q_SLOTS:
    void updateMaps();
    void setSelectedMaps(QList<qulonglong> ids);
    void releaseSelectedMaps();
    void toggleSelectedMapsMuting();
    void selectedSigs(QList<qulonglong> ids, QList<QPointF> positions, bool is_src);
    void setModeLinear();
    void setModeExpression();
    void setExpression(QString expr);
    void mapSigs(QList<qulonglong> srcs, qulonglong dst);
    void unmapSigs(QList<qulonglong> srcs, qulonglong dst);

private:
    Ui::ListTab *ui;
    QList<qulonglong> selectedSrcSigIds;
    QList<QPointF> selectedSrcSigPositions;
    QList<qulonglong> selectedDstSigIds;
    QList<QPointF> selectedDstSigPositions;
    QList<qulonglong> selectedMaps;
};

#endif // LISTTAB_H
