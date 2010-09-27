#ifndef DGRAPHICSWIDGET_H
#define DGRAPHICSWIDGET_H

#include <QGraphicsWidget>
class QStyleOptionGraphicsItem;
class QGraphicsAnchorLayout;
class QGraphicsLinearLayout;
class DGraphicsTextItem;
class DGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    DGraphicsWidget(QGraphicsItem*  parent=0);
    ~DGraphicsWidget();

    void setTitleText(QString titleText);
    QString titleText();
    void setBackgroundColor(QColor color){mBackgroundColor = color;}
    QColor backgroundColor(){return mBackgroundColor;}
    void addContentLayout(QGraphicsLayoutItem* contentLayout);

private:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
private:
    void addDefaultActions();

private:
    QColor mBackgroundColor;
    QGraphicsAnchorLayout* mAnchorLayout;
    QGraphicsLinearLayout* mTitleLayout;
    QGraphicsLinearLayout* mCommandLayout;
    DGraphicsTextItem* mTextItem;
};

#endif // DGRAPHICSWIDGET_H
