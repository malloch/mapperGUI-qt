#ifndef CONNECTIONPROPS_H
#define CONNECTIONPROPS_H

#include <QWidget>

typedef enum {
    _MO_UNDEFINED,    //!< Not yet defined
    _MO_RAW,          //!< No type coercion
    _MO_LINEAR,       //!< Linear scaling
    _MO_EXPRESSION,   //!< Expression
};

namespace Ui {
class ConnectionProps;
}

class ConnectionProps : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionProps(QWidget *parent = 0);
    ~ConnectionProps();

    void displayProps(int mode, bool muted, bool calibrating,
                      const QString& expression);
    void clearProps();

private:
    Ui::ConnectionProps *ui;
};

#endif // CONNECTIONPROPS_H
