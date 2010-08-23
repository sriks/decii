#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QObject>

#include "analoghand.h"

/*
 Holds a clock frame onto which analog hands
 are drawn. Any text items like date etc can
 be added to this class.
 */

class QPropertyAnimation;
class QParallelAnimationGroup;
class ClockWidget : public QGraphicsView
{
    Q_OBJECT

public:
    ClockWidget(QObject *parent = 0);
    ~ClockWidget();

//public:
    //void mouseMoveEvent( QMouseEvent * event ){/*handleInterruption();*/}
    //void keyPressEvent ( QKeyEvent * event){handleInterruption();}

private slots:
    // sync with system time and start clock
    void on_sync_entered();
    void on_secondshand_entered();
    void on_secondshand_exited();

private:
    void handleInterruption();

private:
    AnalogHand* iSecondsHand;
    AnalogHand* iMinutesHand;
    AnalogHand* iHoursHand;

    QGraphicsScene* iScene;
    QParallelAnimationGroup* iStartupAnimation;
    QPropertyAnimation* iSecHandAnimation;
    QPropertyAnimation* iMinHandAnimation;
    QPropertyAnimation* iHourHandAnimation;
};

#endif // CLOCKWIDGET_H
