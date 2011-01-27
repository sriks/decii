/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#ifndef KSEGMENT_H
#define KSEGMENT_H

#include <QGraphicsWidget>
#include <QPointer>

class QSizeF;
class QGraphicsLinearLayout;
class KSegment : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit KSegment(Qt::Orientation aOrientation = Qt::Vertical,QGraphicsItem *aParent = 0);
    QGraphicsLinearLayout* layout() { return mLayout;}
    void addItem(QGraphicsWidget* aGraphicsLayoutItem);
    void addItem(QGraphicsItem* aGraphicsItem);
    void addItem(QWidget* aWidget);
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    void setPreferredWidth(qreal width){mPreferredSize.setWidth(width);};

protected slots:
    void onHeightChanged() const;

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int contentHieght() const {return mContentHeight;}
    void setInternalContainer(bool value){mInternalContainer =  value;}
    bool internalContainer() const {return mInternalContainer;}
    QSizeF internalItemSize() const;

private:
   QGraphicsLinearLayout* mLayout;
   mutable int mContentHeight;
   bool mInternalContainer;
   QSizeF mPreferredSize;
};

#endif // KSEGMENT_H
