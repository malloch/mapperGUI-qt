#ifndef RANGEEDITOR_H
#define RANGEEDITOR_H

#include <QWidget>

namespace Ui {
class RangeEditor;
}

class RangeEditor : public QWidget
{
    Q_OBJECT

public:
    explicit RangeEditor(QWidget *parent = 0);
    ~RangeEditor();

private:
    Ui::RangeEditor *ui;
};

#endif // RANGEEDITOR_H
