#ifndef SNOWFLAKES_H
#define SNOWFLAKES_H

#include <QGraphicsView>

class QGraphicsWidget;
class Snowflakes : public QGraphicsView
{
    Q_OBJECT
public:
    Snowflakes(QObject* aParent=0);
    void startSnowfall();
    void stopSnowfall();
private:
    void timerEvent(QTimerEvent *aEvent);
    QGraphicsWidget* createSnowflake();
private:
    QPixmap mSnowflakePixmap;
    int mTimerId;
};

#endif // SNOWFLAKES_H
