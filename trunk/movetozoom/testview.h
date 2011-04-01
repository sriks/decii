#ifndef TESTVIEW_H
#define TESTVIEW_H
#include <QGraphicsView>

class QGraphicsTextItem;
class TestView : public QGraphicsView
{
    Q_OBJECT
public:
    TestView(QWidget* parent = 0);
public slots:
    void onZoom(qreal factor);
private:
    QGraphicsTextItem* item;
};

#endif // TESTVIEW_H
