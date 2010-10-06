#ifndef DGRAPHICSWIDGET_H
#define DGRAPHICSWIDGET_H

#include <QGraphicsWidget>
class QStyleOptionGraphicsItem;
class QGraphicsAnchorLayout;
class QGraphicsLinearLayout;
class DGraphicsTitleWidget;
class DGraphicsPixmapWidget;
class DGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    DGraphicsWidget(QGraphicsItem*  parent=0);
    ~DGraphicsWidget();
    void setTitleText(QString titleText);
    void setTitleFont(QFont font);
    void setTitlePixmap(QPixmap pixmap,bool autoResize = false);
    void addContent(QGraphicsWidget* contentWidget);
    bool glassEffect(){return mGlassEffect;}
    void setGlassEffect(bool value){mGlassEffect = value;}
    QSizeF contentWidgetSize();
    void closeApplicationOnExit(bool value);
    void setCloseButtonVisible(bool value);

signals:
    void closeButtonClicked();
private slots:
    void testResize()
    {
        static int i = 0;
        i++;
        if(i%2==0)
        {
            resize(500,400);
        }
        else
        {
        resize(200,200);
        }
    }

private:
    void addItemsToLayout();
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    void resizeEvent(QGraphicsSceneResizeEvent* event);

private:
    void addDefaultActions();
    void prepareWidget();
    void prepareTitleWidget();
    void prepareContentWidget(QGraphicsWidget* content);

private:
    bool mGlassEffect;
    QGraphicsAnchorLayout* mAnchorLayout;
    QGraphicsLinearLayout* mTitleLayout;
    QGraphicsLinearLayout* mCommandLayout;
    DGraphicsTitleWidget* mTitleWidget;
    QGraphicsWidget* mContentWidget; // non owning
    DGraphicsPixmapWidget* mCloseCommandIcon;
};

#endif // DGRAPHICSWIDGET_H
