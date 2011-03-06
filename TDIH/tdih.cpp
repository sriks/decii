
#include <QDebug>
#include <QMenu>
#include <QList>
#include <QApplication>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsBlurEffect>
#include <QDesktopWidget>
#include <QTextDocument>
#include <QPalette>
#include <QPainter>
#include <QImage>
#include <QScrollBar>
#include <QPropertyAnimation>
#include "flickcharm.h"
#include "tdih.h"
#include "KSegment.h"
#include "KSegmentWidget.h"

const QString KMainTitle("THIS DAY IN HISTORY");
const QString KFavoritesTitle("MY FAVORITES");
const QString KFavoritesSubTitle("Tap on your favorite to read more");
const QString KLoading("Loading...");
const int KBackgroundZOrder(-10);
const int KDialogZOrder(10);

TDIH::TDIH(QWidget *parent)
    : KSegmentView(Qt::Vertical,parent),
      mUpdateAvailable(false),
      mDisplayWidth(QDesktopWidget().screenGeometry().width()-50),
      mMessageDlg(NULL),
      mMenu(NULL),
      mBlurBackground(NULL),
      mMenuLauncher(NULL),
      mCurrentInfoIndex(0)
{
    mMenuList = new QMenu(this);
    mMenuList->addAction("Favorites", this, SLOT(showFavorites()));
    mMenuList->addAction("Today's",this,SLOT(showToday()));
    mMenuList->addAction("Lucky 0",this,SLOT(showLucky()));
    // Create Options CBA
    QAction *optionsAction = new QAction("Options", this);
    // Set defined menu into Options button
    optionsAction->setMenu(mMenuList);
    optionsAction->setSoftKeyRole(QAction::PositiveSoftKey);
    addAction(optionsAction);

    setSelectionPolicy(KSegmentView::NoSelection);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    QPixmap bkg(":/images/images/bkg.png");
    if(bkg.size() != QDesktopWidget().screenGeometry().size()) {
    bkg = bkg.scaled(size(),
                     Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    // TODO: Prefer to cache pixmap
    }
    mBackgroundPixmap = new QPixmap(bkg);
    mFlickCharm = new FlickCharm(this);
    mFlickCharm->activateOn(this);
    mHistoryEngine = new HistoryEngine(this);
    connect(mHistoryEngine,SIGNAL(updateReady(HistoryInfo)),
            this,SLOT(handleUpdate(HistoryInfo)));
    connect(mHistoryEngine,SIGNAL(error(QString,QUrl)),this,SLOT(onError(QString)));
    mHistoryEngine->update();
//    connect(this,SIGNAL(longPress(QPoint)),this,SLOT(onLongPress()));
    prepareMenuLauncher();
    showMessage(KLoading);
}

TDIH::~TDIH()
{
    delete mMenuLauncher;
    delete mBackgroundPixmap;
    delete mBlurBackground;
    delete mMessageDlg;
}

void TDIH::handleUpdate(HistoryInfo info)
{
   mHistoryInfo = info;
   mUpdateAvailable = true;
   showToday();
}

void TDIH::showToday()
{
    prePageShow();
    showContent(mHistoryInfo);
    mCurrentPage = TDIH::TodayPage;
}

void TDIH::showContent(const HistoryInfo& aHistoryInfo)
{
    addSegmentWidget(prepareSegmentWidget(aHistoryInfo));
    showFullScreen();
}

void TDIH::showFavorites()
{
//      prePageShow();
//      QStringList favTitles = mHistoryEngine->favoriteTitles();
//      if(!favTitles.isEmpty()) {

//          container()->addItem(prepareTitle(KFavoritesTitle));
//          QGraphicsTextItem* subtitle = new QGraphicsTextItem(KFavoritesSubTitle);
//          subtitle->setTextWidth(mDisplayWidth);
//          container()->addItem(subtitle);
//          for(int i=0;i<favTitles.count();++i) {
//            KSegmentWidget* titleWidget = new KSegmentWidget;
//            titleWidget->setPreferredWidth(mDisplayWidth);
//            QGraphicsTextItem* titleText = new QGraphicsTextItem(favTitles.at(i));
//            titleText->setTextWidth(mDisplayWidth);
//            titleWidget->addItem(titleText);
//            addSegmentWidget(titleWidget);
//            connect(titleWidget,SIGNAL(clicked(int,QPointF)),this,SLOT(showFavorite(int)));
//          }
//      }
//      mCurrentPage = TDIH::FavoritesPage;
//      showMaximized();
//      verticalScrollBar()->setValue(0);
}

void TDIH::showFavorite(int aIndex)
{
//    const int itemsToIgnore = 2; // fav's title and subtitle
//    int favIndex = aIndex - itemsToIgnore;
//    if(favIndex >= 0) {
//        qDebug()<<"looking for fav index:"<<favIndex;
//        prePageShow();
//        showContent(mHistoryEngine->favorite(favIndex));
//        showMaximized();
//        mCurrentPage = TDIH::FavoriteContent;
//    }
}

void TDIH::showLucky()
{
    showFavorite(2);
}

void TDIH::onError(QString errorDescription)
{
    showMessage(errorDescription);
}

QSize TDIH::sizeHint() const
{
    return KSegmentView::sizeHint();
}

void TDIH::drawBackground(QPainter *painter, const QRectF &rect)
{
    if(painter->isActive()) {
        painter->setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
        painter->drawPixmap(rect.toRect(),*mBackgroundPixmap);
        positionMenuLauncher(rect.bottomLeft());
    }
}

void TDIH::showMessage(QString aMessage)
{
    preDialogShow();
    if(NULL == mMessageDlg) {
        mMessageDlg =  new KSegmentWidget(Qt::Vertical);
        mMessageDlg->setPreferredWidth(mDisplayWidth);
    }
    if(mMessageDlg->layout()->count()) {
    mMessageDlg->layout()->removeAt(0);
    }
    QGraphicsTextItem* msg = new QGraphicsTextItem;
    msg->setTextWidth(mDisplayWidth);
    msg->setHtml(aMessage);
    mMessageDlg->addItem(msg);
    mMessageDlg->setZValue(KDialogZOrder);
    scene()->addItem(mMessageDlg);
    positionToCenter(mMessageDlg);
}

void TDIH::showMenu()
{
    preDialogShow();
    if(mMenu) {
       mMenu->removeWithAnimation();
    }

    mMenu = new KSegmentWidget(Qt::Vertical);
    mMenu->setRoundness(0);
    mMenu->setPreferredWidth(mDisplayWidth);
    mMenu->setZValue(KDialogZOrder);
    mMenu->layout()->setSpacing(10);
    QGraphicsPathItem* menuItemBackground = new QGraphicsPathItem(mMenu);
    QColor bkgColor("#CC4400");
    bkgColor.setAlpha(160);
    menuItemBackground->setBrush(QBrush(bkgColor));
    menuItemBackground->setPen(QPen(Qt::NoPen));
    mMenu->setBackground(menuItemBackground);
    mMenu->addItem(new QGraphicsTextItem("Options"));

    if(TDIH::TodayPage != mCurrentPage) {
        KSegmentWidget* today = new KSegmentWidget(Qt::Vertical);
        today->addItem(new QGraphicsTextItem("Today's article"));
        today->setPreferredWidth(mDisplayWidth);
        mMenu->addItem(today);
        connect(today,SIGNAL(clicked(int,QPointF)),this,SLOT(showToday()));
    }

    if(TDIH::FavoritesPage != mCurrentPage) {
    KSegmentWidget* sendAsSMS = new KSegmentWidget(Qt::Vertical);
    sendAsSMS->addItem(new QGraphicsTextItem("Share via SMS"));
    sendAsSMS->setPreferredWidth(mDisplayWidth);
    mMenu->addItem(sendAsSMS);
    connect(sendAsSMS,SIGNAL(clicked(int,QPointF)),this,SLOT(shareViaSMS()));
    }

    if(TDIH::TodayPage == mCurrentPage && !mHistoryEngine->isFavorite(mHistoryInfo)) {
    KSegmentWidget* fav = new KSegmentWidget(Qt::Vertical);
    fav->addItem(new QGraphicsTextItem("Mark as Favorite"));
    fav->setPreferredWidth(mDisplayWidth);
    connect(fav,SIGNAL(clicked(int,QPointF)),this,SLOT(saveAsFavorite()));
    mMenu->addItem(fav);
    }

    if(TDIH::FavoritesPage != mCurrentPage &&
       mHistoryEngine->favoriteTitles().count()) {
        KSegmentWidget* showFavs = new KSegmentWidget(Qt::Vertical);
        showFavs->addItem(new QGraphicsTextItem("My Favorites"));
        showFavs->setPreferredWidth(mDisplayWidth);
        connect(showFavs,SIGNAL(clicked(int,QPointF)),this,SLOT(showFavorites()));
        mMenu->addItem(showFavs);
    }

    KSegmentWidget* exit = new KSegmentWidget(Qt::Vertical);
    exit->addItem(new QGraphicsTextItem("Exit"));
    exit->setPreferredWidth(mDisplayWidth);
    connect(exit,SIGNAL(clicked(int,QPointF)),QApplication::instance(),SLOT(quit()));
    mMenu->addItem(exit);

    KSegmentWidget* cancel = new KSegmentWidget(Qt::Vertical);
    cancel->addItem(new QGraphicsTextItem("Back"));
    cancel->setPreferredWidth(mDisplayWidth);
    connect(cancel,SIGNAL(clicked(int,QPointF)),this,SLOT(prePageShow()));
    mMenu->addItem(cancel);

    scene()->addItem(mMenu);
    positionToCenter(mMenu);
}

void TDIH::onLongPress()
{
    if(mMessageDlg && mMessageDlg->isVisible()) {
       mMessageDlg->removeWithAnimation();
    }

    else if(mMenu && mMenu->isVisible()) {
       delete mMenu;
       mMenu = NULL;
    }

    else {
    showMenu();
    }
}

void TDIH::prePageShow()
{
    removeAll(false);
    if(mMessageDlg) {
        mMessageDlg->removeWithAnimation();
        mMessageDlg = NULL;
    }

    if(mMenu) {
        mMenu->removeWithAnimation();
        mMenu = NULL;
    }

    if(mBlurBackground) {
        delete mBlurBackground;
        mBlurBackground = NULL;
    }

    mFlickCharm->setScrolling(false,true);
}

void TDIH::preDialogShow()
{
    if(mMessageDlg) {
        mMessageDlg->remove();
        mMessageDlg = NULL;
    }
    prepareBlurBackground();
    mFlickCharm->setScrolling(false,false);
}

void TDIH::positionToCenter(QGraphicsWidget* aWidget)
{
    aWidget->setVisible(false);
    aWidget->setPos(0,QDesktopWidget().screenGeometry().height()+20);
    aWidget->setVisible(true);
    QPoint newPosition(0,verticalScrollBar()->value() + aWidget->size().height()/2);
    const int animDuration = 700;
    QPropertyAnimation* anim = new QPropertyAnimation(this);
    anim->setTargetObject(aWidget);
    anim->setPropertyName("pos");
    anim->setDuration(animDuration);
    anim->setStartValue(aWidget->pos());
    anim->setEndValue(newPosition);
    anim->setEasingCurve(QEasingCurve::OutBounce);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void TDIH::positionMenuLauncher(QPointF aBottom)
{
    if(mMenuLauncher) {
        QPointF launcherPoint(aBottom);
        launcherPoint.setY(aBottom.y() - mMenuLauncher->size().height());
        launcherPoint.setX(0);
        mMenuLauncher->setPos(launcherPoint);
        mMenuLauncher->show();
    }
}

void TDIH::saveAsFavorite()
{
    prePageShow();
    mHistoryEngine->saveAsFavorite(mHistoryInfo);
    qDebug()<<mHistoryEngine->favoriteTitles();
}

void TDIH::shareViaSMS()
{
    prePageShow();
}

void TDIH::prepareBlurBackground(int aBlurRadius)
{
    return;
    if(NULL == mBlurBackground) {
        QImage screenShot(QDesktopWidget().screenGeometry().size(),QImage::Format_ARGB32_Premultiplied);
        QPainter p(&screenShot);
        this->render(&p,rect(),rect());
        mBlurBackground = new QGraphicsPixmapItem(QPixmap().fromImage(screenShot));
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect(this);
        blur->setBlurRadius(aBlurRadius);
        mBlurBackground->setGraphicsEffect(blur);
        scene()->addItem(mBlurBackground);
        mBlurBackground->setZValue(KDialogZOrder-1);
        mBlurBackground->setPos(0,verticalScrollBar()->value());
    }
}

KSegmentWidget* TDIH::prepareSegmentWidget(const HistoryInfo& aHistoryInfo)
{
    container()->addItem(prepareTitle(KMainTitle));
    QGraphicsTextItem* title = new QGraphicsTextItem(aHistoryInfo.title);
    QFont titleFont = title->font();
    titleFont.setPixelSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setTextWidth(mDisplayWidth);
    QGraphicsTextItem* description = new QGraphicsTextItem;
    description->setTextWidth(mDisplayWidth);
    description->setHtml(aHistoryInfo.description);
    KSegmentWidget* seg = new KSegmentWidget(Qt::Vertical);
    seg->setPreferredWidth(mDisplayWidth);
    seg->addItem(title);
    seg->addItem(description);
    return seg;
}

QGraphicsTextItem* TDIH:: prepareTitle(const QString& aText)
{
    QGraphicsTextItem* widgetTitle = new QGraphicsTextItem(aText);
    widgetTitle->setTextWidth(mDisplayWidth);
    widgetTitle->setFont(QFont(font().family(),21));
    return widgetTitle;
}

KSegmentWidget* TDIH::prepareMenuLauncher()
{
    if(!mMenuLauncher) {
        mMenuLauncher = new KSegmentWidget(Qt::Horizontal);
        mMenuLauncher->setRoundness(0);
        mMenuLauncher->setPreferredWidth(QDesktopWidget().screenGeometry().width());
        mMenuLauncher->setZValue(KDialogZOrder+1);
        mMenuLauncher->background()->setOpacity(0.9);
        QGraphicsTextItem* menuText = new QGraphicsTextItem("\t\tMenu");
        mMenuLauncher->addItem(menuText);
        connect(mMenuLauncher,SIGNAL(clicked(int,QPointF)),this,SLOT(onLongPress()));
        scene()->addItem(mMenuLauncher);
    }
}

// eof
