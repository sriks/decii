#include <QDebug>
#include <QTimerEvent>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QGraphicsItem>
#include "MoveToZoom.h"
QTM_USE_NAMESPACE

struct MoveToZoomPrivate {
    int idleTimerId;
    QAccelerometer* accelerometer;
    QGraphicsItem* target;
    qreal z;
    qreal initialZ;
    bool readStarted;
    ~MoveToZoomPrivate() {
        delete accelerometer;
    }
};

MoveToZoom::MoveToZoom(QGraphicsItem* target,QObject *parent) :
    QObject(parent)
{
    d = new MoveToZoomPrivate;
    d->initialZ = 0;
    d->target = target;
    d->accelerometer = new QAccelerometer(this);
    d->z = 0;
    d->idleTimerId = startTimer(0);
}

MoveToZoom::~MoveToZoom() {
    delete d;
}

bool MoveToZoom::event(QEvent *event) {
    if(QTimerEvent::Timer == event->type() &&
      ((QTimerEvent*)event)->timerId() == d->idleTimerId) {
      startSensor();
    }
}

void MoveToZoom::startSensor() {
    if(!d->accelerometer->isActive()) {
        d->readStarted = true;
        d->accelerometer->start();
    }
    handleSensorReading(d->accelerometer->reading());
}

void MoveToZoom::captureInitialReading() {

}

void MoveToZoom::handleSensorReading(const QAccelerometerReading* sensorReading) {
//    qDebug()<<__PRETTY_FUNCTION__;
    // z is reducing when moving up.
//        QString text("x:%1,y:%2,z:%3");
//        text = text.arg(QString().setNum(sensorReading->x()),
//                        QString().setNum(sensorReading->y()),
//                        QString().setNum(sensorReading->z()));
//        qDebug()<<text;

    qDebug()<<sensorReading->z();

    if(0 == d->initialZ) {
       d->initialZ = sensorReading->z();
       qDebug()<<"-initial z:"<<d->initialZ;
       return;
    }

    if(d->z != 0) {
        bool zoomin = false;
        qreal scaleFactor = 0;
        // device is moved towards face
        if(sensorReading->z() < d->z) {
            scaleFactor = d->z - sensorReading->z();
            zoomin= true;
        }
        // device is moved away from face
        else if(sensorReading->z() > d->z){
            scaleFactor = sensorReading->z() - d->z;
            zoomin = false;
        }

//        qreal standard = 8.1;

            if(sensorReading->z() < /*standard*/d->initialZ) {
            // zoom in
            if( /*sensorReading->z() > d->z*/ zoomin && scaleFactor > 0.15) {
                    scaleFactor = d->target->scale()+ 0.2/*scaleFactor*/;
                }

            // zoom out
            else if( /*(sensorReading->z() - d->z)*/!zoomin && scaleFactor > 0.2){
                    scaleFactor = d->target->scale()- 0.2/*scaleFactor*/;
                }
            }
            else {
                scaleFactor = 1;
            }


            if(scaleFactor >=1 && scaleFactor <= 8) {
            d->target->setScale(scaleFactor);
            }
    }
    d->z = sensorReading->z();
}

// eof
