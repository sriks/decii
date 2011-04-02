#include <QGraphicsTextItem>
#include <QDesktopWidget>
#include "FavoritesView.h"
#include "KSegmentView.h"
#include "KSegmentWidget.h"
#include "HistoryEngine.h"

struct FavoritesViewPrivate {
    HistoryEngine* engine;
};

FavoritesView::FavoritesView(HistoryEngine* engine, QWidget *parent)
              :KSegmentView(Qt::Vertical,parent)
{
    d = new FavoritesViewPrivate;
    d->engine = engine;
    prepareView();
}

FavoritesView::~FavoritesView() {
    delete d;
}

void FavoritesView::prepareView() {
    QStringList favList = d->engine->favoriteTitles();

    if(favList.isEmpty()) {
        this->scene()->addText("You don't have any favorites.");
    }

    else {
    for(int i=0;i<favList.count();++i) {
        int w = QDesktopWidget().screenGeometry().size().width()-30;
        KSegmentWidget* fav = new KSegmentWidget;
        fav->setPreferredWidth(w);
        QGraphicsTextItem* text = new QGraphicsTextItem;
        text->setHtml(favList.at(i));
        text->setTextWidth(w);
        fav->addItem(text);
        this->addSegmentWidget(fav);
    }
    }
}

// eof
