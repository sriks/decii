#ifndef DGRAPHICSWIDGET_H
#define DGRAPHICSWIDGET_H

#include <QGraphicsWidget>
class QStyleOptionGraphicsItem;
class QGraphicsAnchorLayout;
class QGraphicsLinearLayout;
class DGraphicsTitleWidget;
class DGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    DGraphicsWidget(QGraphicsItem*  parent=0);
    ~DGraphicsWidget();
    void setTitleText(QString titleText);
    void setTitleFont(QFont font);
    void setTitlePixmap(QPixmap pixmap,bool autoResize = false);
    void addContent(QGraphicsLayoutItem* content);

private:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
private:
    void addDefaultActions();
    void prepareWidget();
    void prepareTitleWidget();

private:
    QGraphicsAnchorLayout* mAnchorLayout;
    QGraphicsLinearLayout* mCommandLayout;
    DGraphicsTitleWidget* mTitleWidget;
};

#endif // DGRAPHICSWIDGET_H
