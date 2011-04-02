#ifndef FAVORITESVIEW_H
#define FAVORITESVIEW_H
#include "KSegmentView.h"

class FavoritesViewPrivate;
class HistoryEngine;
class FavoritesView : public KSegmentView
{
public:
    FavoritesView(HistoryEngine* engine, QWidget *parent = 0);
    ~FavoritesView();
    void prepareView();
private:
    FavoritesViewPrivate* d;
};

#endif // FAVORITESVIEW_H
