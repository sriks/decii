#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "analoghand.h"

/*
 Holds a clock frame onto which analog hands
 are drawn. Any text items like date etc can
 be added to this class.
 */
class QStateMachine;
class QState;
class QPropertyAnimation;
class QParallelAnimationGroup;
class ClockWidget : public QGraphicsView
{
    Q_OBJECT

public:
    ClockWidget(QObject *parent = 0);
    ~ClockWidget();

public:
    //void mouseMoveEvent( QMouseEvent * event ){/*handleInterruption();*/}
    //void keyPressEvent ( QKeyEvent * event){handleInterruption();}

private slots:
    void on_syncstate_entered();
    void on_secondshand_entered();
    void on_secondshand_exited();

private:
    void handleInterruption();

private:
    AnalogHand* iSecondsHand;
    AnalogHand* iMinutesHand;
    AnalogHand* iHoursHand;

    QGraphicsScene* iScene;
    QStateMachine* iStateMachine;
    QState*        iParentState;
    QState*        iSyncState;
    QState*        iSecondsHandState;
    QState*        iMinutesHandState;
    QState*        iHoursHandState;
    QParallelAnimationGroup* iStartupAnimation;
    QPropertyAnimation* iSecHandAnimation;
    QPropertyAnimation* iMinHandAnimation;
    QPropertyAnimation* iHourHandAnimation;
};

#endif // CLOCKWIDGET_H
