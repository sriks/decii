#ifndef TDIH_H
#define TDIH_H

#include <QtGui/QWidget>
#include "historyengine.h"
#include "KSegmentView.h"

class QMenu;
class QGraphicsTextItem;
class QGraphicsWidget;
class KSegmentWidget;
class FlickCharm;
class TDIH : public KSegmentView
{
    Q_OBJECT
    enum Page {
        TodayPage,
        FavoritesPage,
        FavoriteContent
    };

public:
    TDIH(QWidget *parent = 0);
    ~TDIH();
    QSize sizeHint() const;
private slots:
    void handleUpdate(HistoryInfo);
    void showContent(const HistoryInfo& aHistoryInfo);
    void showToday();
    void showFavorites();
    void showFavorite(int aIndex);
    void onLongPress();
    void prePageShow();
    void preDialogShow();
    void positionToCenter(QGraphicsWidget* aWidget);
    void positionMenuLauncher(QPointF aBottom);
    void saveAsFavorite();
    void shareViaSMS();
    void showLucky();
    void onError(QString errorDescription);
private:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void showMessage(QString aMessage);
    void showMenu();
    void prepareBlurBackground(int aBlurRadius = 10);
    KSegmentWidget* prepareSegmentWidget(const HistoryInfo& aHistoryInfo);
    QGraphicsTextItem* prepareTitle(const QString& aText);
    KSegmentWidget* prepareMenuLauncher();

private:
    FlickCharm* mFlickCharm;
    HistoryEngine* mHistoryEngine;
    HistoryInfo mHistoryInfo;
    QGraphicsTextItem* mApplicationTitle;
    QGraphicsTextItem* mTitle;
    QGraphicsTextItem* mDescription;
    KSegmentWidget* mMessageDlg;
    KSegmentWidget* mMenu;
    QPixmap* mBackgroundPixmap;
    QGraphicsPixmapItem* mBackgroundPixmapItem;
    QGraphicsPixmapItem* mBlurBackground;
    KSegmentWidget* mMenuLauncher;
    QMenu* mMenuList;
    bool mUpdateAvailable;
    int mDisplayWidth;
    int mCurrentInfoIndex;
    Page mCurrentPage;
};

#endif // TDIH_H
