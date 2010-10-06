#ifndef DGRAPHICSWEBVIEW_H
#define DGRAPHICSWEBVIEW_H

#include <QGraphicsView>
class QGraphicsScene;
class DGraphicsView : public QGraphicsView
{
public:
    DGraphicsView(QWidget* parent=0);
    ~DGraphicsView();
    virtual void resizeEvent(QResizeEvent *event);
    QGraphicsScene* scene();
    void moveToCenter();
private:
    QGraphicsScene* mScene;
};

#endif // DGRAPHICSWEBVIEW_H
