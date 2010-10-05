#include <QGraphicsItem>
#include <QPixmap>
#include <QDebug>
#include "samplewidget.h"

SampleWidget::SampleWidget(QWidget* parent)
             :QGraphicsView(parent)
{
    mScene = new QGraphicsScene(this);
    setScene(mScene);
    QPixmap pixmap;
    qDebug()<<"pixmap loaded "<<pixmap.load(":/resource/images/image1.JPG");
    pixmap = pixmap.scaled(size());
    QGraphicsItem* item = mScene->addPixmap(pixmap);
}
