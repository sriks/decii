#ifndef DCADATAENTRYWIDGET_H
#define DCADATAENTRYWIDGET_H

#include <QWidget>

class QtScrollWheel;
class QLabel;
class QSize;
class DCADataEntryWidget : public QWidget
{
    Q_OBJECT

public:
    DCADataEntryWidget(QWidget* parent = 0);
    ~DCADataEntryWidget();
    void setTitle(QString title);
    QString title();

protected slots:
    void valueChanged(int value);

private:
    QLabel* mTitle;
    QLabel* mReading;
    QLabel* mUnits;
    QLabel* mSeverityIndicator;
    QtScrollWheel* mQtScrollWheel;
    QSize mScreenSize;
};

#endif // DCADATAENTRYWIDGET_H
