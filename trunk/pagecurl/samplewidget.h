#ifndef SAMPLEWIDGET_H
#define SAMPLEWIDGET_H
#include <QGraphicsView>
#include <QGraphicsScene>

class SampleWidget : public QGraphicsView
{
public:
    SampleWidget(QWidget* parent=0);

private:
    QGraphicsScene* mScene;
};

#endif // SAMPLEWIDGET_H
