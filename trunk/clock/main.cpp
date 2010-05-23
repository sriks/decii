#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPropertyAnimation>

#include "clockwidget.h"
#include "analoghand.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget parentWidget;
    parentWidget.resize(400,400);
    ClockWidget* clockWidget = new ClockWidget(&parentWidget);
clockWidget->resize(400,400);
#ifdef xx
    QGraphicsView gv;
    QGraphicsScene scene;

    ClockWidget clockwidget;
    scene.addItem(&clockwidget);
    gv.setScene(&scene);

#if defined(Q_WS_S60)
    //w.showMaximized();
#else
    //w.show();
#endif
#endif
    //clockWidget->rotate(6);

    clockWidget->showMaximized();
//clockWidget->show();
    return a.exec();
}


/*
 Reference code, remove once finalized
    //clockwidget w;
    AnalogHand secondsHand(QLine(0,0,0,60));

    scene.addItem(&secondsHand);
    gv.setScene(&scene);
    QTransform transform;
    transform = transform.translate(gv.width(),gv.height());
    secondsHand.setTransform(transform);
    //secondsHand.setTransformOriginPoint(gv.width()/2,gv.height()/2);
    // add animations
    QPropertyAnimation* anim = new QPropertyAnimation(&secondsHand,"rotation");
    anim->setDuration(10*1000); // 60 secs
    anim->setStartValue(qreal(0));
    anim->setEndValue(qreal(360));
    anim->setEasingCurve(QEasingCurve::Linear);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
*/
