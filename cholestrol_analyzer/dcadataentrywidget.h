#ifndef DCADATAENTRYWIDGET_H
#define DCADATAENTRYWIDGET_H

#include <QWidget>

class QtScrollWheel;
class QLabel;
class QSize;
class QBoxLayout;
class DCADataEntryWidget : public QWidget
{
    Q_OBJECT

public:
    DCADataEntryWidget(QWidget* parent = 0);
    ~DCADataEntryWidget();
    void setTitle(QString title);
    QString title();
    void setInitialValue(int initialValue);
    int readingValue();

protected:
    void resizeEvent(QResizeEvent *event);
protected slots:
    void valueChanged(int value);

private:
    QLabel* mTitle;
    QLabel* mReading;
    QLabel* mUnits;
    QLabel* mSeverityIndicator;
    QBoxLayout* mMasterLayout;
    QBoxLayout* mMasterReadingLayout;
    QBoxLayout* mTitleLayout;
    QBoxLayout* mReadingLayout;
    QBoxLayout* mDataEntryLayout;

    QtScrollWheel* mQtScrollWheel;
    QSize mScreenSize;
};

#endif // DCADATAENTRYWIDGET_H
