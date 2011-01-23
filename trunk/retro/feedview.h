#ifndef FEEDVIEW_H
#define FEEDVIEW_H
#include <QWidget>
#include <QGraphicsView>
#include "ksegmentview.h"

typedef KSegment Segment;
typedef KSegmentWidget SegmentWidget;
typedef KSegmentView SegmentView;

class RSSManager;
class RSSParser;

class FeedView : public SegmentView
{
    Q_OBJECT
public:
    explicit FeedView(QWidget *parent = 0);

signals:

public slots:
    void showFeedView(RSSParser* aParser);

};

#endif // FEEDVIEW_H
