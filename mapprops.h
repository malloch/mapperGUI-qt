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

    void displayProps(QList<int> mode, QList<bool> muted,
                      QList<QString> expression);
    void clearProps();
    void setMuted(int state);

public Q_SLOTS:
    void expressionChanged();

Q_SIGNALS:
    void setMuted(bool state);
    void setModeLinear();
    void setModeExpression();
    void setExpression(const QString& expression);

private:
    Ui::MapProps *ui;
};

#endif // MAPPROPS_H
