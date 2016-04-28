#ifndef MAPPROPS_H
#define MAPPROPS_H

#include <QWidget>

typedef enum {
    _MO_UNDEFINED,    //!< Not yet defined
    _MO_RAW,          //!< No type coercion
    _MO_LINEAR,       //!< Linear scaling
    _MO_EXPRESSION,   //!< Expression
} _mode;

namespace Ui {
class MapProps;
}

class MapProps : public QWidget
{
    Q_OBJECT

public:
    explicit MapProps(QWidget *parent);
    ~MapProps();

    void displayProps(int mode, bool muted, const QString& expression);
    void clearProps();
    void setMuting(int state);

Q_SIGNALS:
    void muteChecked(bool state);
    void linearModeButtonPressed();
    void expressionModeButtonPressed();
    void expressionChanged(const QString& expression);

private:
    Ui::MapProps *ui;
};

#endif // MAPPROPS_H
