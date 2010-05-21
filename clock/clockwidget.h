#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QGraphicsWidget>
#include "analoghand.h"

/*
 Holds a clock frame onto which analog hands
 are drawn. Any text items like date etc can
 be added to this class.
 */
class QStateMachine;
class QState;
class ClockWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    ClockWidget(QGraphicsItem *parent = 0);
    ~ClockWidget();

private:
    AnalogHand* iSecondsHand;
    AnalogHand* iMinutesHand;

    QStateMachine* iStateMachine;
    QState*        iSecondsHandState;
    QState*        iMinutesHandState;

};

#endif // CLOCKWIDGET_H
