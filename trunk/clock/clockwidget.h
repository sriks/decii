#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H


#include <QGraphicsWidget>

class QGraphicsWidget;
class clockwidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    clockwidget(QObject *parent = 0);
    ~clockwidget();
};

#endif // CLOCKWIDGET_H
