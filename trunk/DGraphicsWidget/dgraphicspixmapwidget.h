#ifndef DGraphicsPixmapWidget_H
#define DGraphicsPixmapWidget_H
#include <QObject>
#include <QGraphicsWidget>

class QGraphicsItem;
class QGraphicsPixmapItem;
class QPixmap;
class DGraphicsPixmapWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    DGraphicsPixmapWidget(QPixmap pixmap,QGraphicsItem* parent = 0);
    ~DGraphicsPixmapWidget();
    QGraphicsPixmapItem* pixmapItem(){return mPixmapItem;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void resizeEvent(QGraphicsSceneResizeEvent* event);
signals:
    void triggered();
private:
    QGraphicsPixmapItem* mPixmapItem;
};

#endif // DGraphicsPixmapWidget_H
