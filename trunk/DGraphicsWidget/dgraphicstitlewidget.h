#ifndef DGRAPHICSTITLEWIDGET_H
#define DGRAPHICSTITLEWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsItem>

class QGraphicsLinearLayout;
class DGraphicsTextWidget;
class DGraphicsPixmapWidget;
class DGraphicsTitleWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit DGraphicsTitleWidget(QGraphicsItem *parent = 0);
    ~DGraphicsTitleWidget();
    void setTitleText(QString titleText);
    DGraphicsTextWidget* textWidget(){return mTitleText;}
    DGraphicsPixmapWidget* pixmapWidget(){return mTitlePixmap;}
    // add resize flag
    void setTitlePixmap(QPixmap titlePixmap,bool autoResize);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    void resizeEvent(QGraphicsSceneResizeEvent* event);

signals:
    void clicked();

public slots:

private:
    void addItemsToLayout();
    QPixmap resizePixmap(QPixmap pixmap);
    void prepareTitle(QSizeF size);
private:
    DGraphicsTextWidget* mTitleText;
    DGraphicsPixmapWidget* mTitlePixmap;
    QGraphicsLinearLayout* mLinearLayout;
    bool mTitlePixmapAutoResize;
};

#endif // DGRAPHICSTITLEWIDGET_H
