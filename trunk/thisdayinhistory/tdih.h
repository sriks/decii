#ifndef TDIH_H
#define TDIH_H

#include <QtGui/QWidget>
#include "dgraphicsview.h"
#include "dgraphicswidget.h"
#include "dgraphicstextwidget.h"
#include "historyengine.h"

class QGraphicsWebView;
class TDIH : public QWidget
{
    Q_OBJECT

public:
    TDIH(QWidget *parent = 0);
    ~TDIH();
private slots:
    void handleUpdate(HistoryInfo info);
    void showWidget();

private:
    DGraphicsView* mDGraphicsView;
    DGraphicsWidget* mDGraphicsWidget;
    DGraphicsTextWidget* mDTextWidget;
    QGraphicsWebView* mWebContent;
    HistoryEngine* mHistoryEngine;
    HistoryInfo mHistoryInfo;
    bool mUpdateAvailable;
};

#endif // TDIH_H
