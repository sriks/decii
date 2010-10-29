#ifndef DCADATAENTRYWIDGET_H
#define DCADATAENTRYWIDGET_H

#include <QGraphicsWidget>

class QGraphicsProxyWidget;
class QGraphicsSceneResizeEvent;
class QtScrollWheel;
class DGraphicsTextWidget;
class DCADataEntryWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    DCADataEntryWidget(QGraphicsItem* parent = 0);
    ~DCADataEntryWidget();
    void setTitle(QString title);
    QString title();

protected slots:
    void valueChanged(int value);

protected:
     bool eventFilter(QObject *obj, QEvent *event);
     void resizeEvent(QGraphicsSceneResizeEvent *event);
private:
    DGraphicsTextWidget* mTitle;
    DGraphicsTextWidget* mReading;
    DGraphicsTextWidget* mUnits;
    DGraphicsTextWidget* mSeverityIndicator;
    QGraphicsProxyWidget* mProxySlider;
    QtScrollWheel* mQtScrollWheel;
};

#endif // DCADATAENTRYWIDGET_H
