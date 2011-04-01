#ifndef MOVETOZOOM_H
#define MOVETOZOOM_H

#include <QObject>
#include <QAccelerometerReading>

QTM_USE_NAMESPACE
class QGraphicsItem;
class MoveToZoomPrivate;
class MoveToZoom : public QObject
{
    Q_OBJECT
public:
    explicit MoveToZoom(QGraphicsItem* target,QObject *parent = 0);
    ~MoveToZoom();

public:
    bool event(QEvent *event);

public slots:
    void startSensor();
    void handleSensorReading(const QAccelerometerReading* sensorReading);
    void captureInitialReading();
signals:
    void zoom(qreal factor);

private:
    MoveToZoomPrivate* d;
};

#endif // MOVETOZOOM_H
