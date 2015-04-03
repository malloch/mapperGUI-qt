#ifndef MODEEDITOR_H
#define MODEEDITOR_H

#include <QWidget>

namespace Ui {
class ModeEditor;
}

class ModeEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ModeEditor(QWidget *parent = 0);
    ~ModeEditor();

private:
    Ui::ModeEditor *ui;
};

#endif // MODEEDITOR_H
