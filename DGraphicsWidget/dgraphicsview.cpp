#include <QDebug>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QDesktopWidget>
#include "dgraphicsview.h"

DGraphicsView::DGraphicsView(QWidget* parent)
                 :QGraphicsView(parent)
{
    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(rect());
    setScene(mScene);

    // prepare see through window
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setAttribute(Qt::WA_ContentsPropagated,true);

    // transparent background
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Base, Qt::transparent);
    setPalette(palette);
    setAttribute(Qt::WA_OpaquePaintEvent, false); // trasmit color
    setFrameStyle(QFrame::NoFrame);
//    showMaximized();

}

DGraphicsView::~DGraphicsView()
{
    qDebug()<<__PRETTY_FUNCTION__;
}

void DGraphicsView::resizeEvent(QResizeEvent *event)
{
    mScene->setSceneRect(rect());
}

void DGraphicsView::moveToCenter()
{
    QRect rect(this->rect());
    QPoint center = QDesktopWidget().screenGeometry().center();
    rect.moveCenter(center);
    this->setGeometry(rect);
    updateGeometry();
}

QGraphicsScene* DGraphicsView::scene()
{
    return mScene;
}
