#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPropertyAnimation>

#include "clockwidget.h"
#include "analoghand.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsView gv;
    QGraphicsScene scene;
    //clockwidget w;
    analoghand secondsHand(QLine(0,0,0,40));
    scene.addItem(&secondsHand);
    gv.setScene(&scene);
    // add animations
    QPropertyAnimation* anim = new QPropertyAnimation(&secondsHand,"rotation");
    anim->setDuration(10*1000); // 60 secs
    anim->setStartValue(qreal(0));
    anim->setEndValue(qreal(360));
    anim->setEasingCurve(QEasingCurve::Linear);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

#if defined(Q_WS_S60)
    //w.showMaximized();
#else
    //w.show();
#endif
    gv.show();
    return a.exec();
}
