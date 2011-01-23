#include <QApplication>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsLinearLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QGraphicsWebView>
#include "feedview.h"
#include "rssparser.h"
#include "ksegment.h"
#include "ksegmentwidget.h"
#include "ksegmentview.h"


FeedView::FeedView(QWidget *parent) :
    SegmentView(Qt::Vertical,parent)
{
}

void FeedView::showFeedView(RSSParser* aParser)
{
    qDebug()<<__PRETTY_FUNCTION__;

    if(aParser->isValid())
    {
        QFont dateFont(QApplication::font());
        QGraphicsTextItem* channelTitle = new QGraphicsTextItem;
        channelTitle->setHtml(aParser->decodeHtml(aParser->channelElement(RSSParser::title)).toUpper());
        scene()->addItem(channelTitle);
        channelTitle->setPos(0,0);
        channelTitle->setZValue(5);
        channelTitle->setOpacity(0.7);
        this->container()->addItem(channelTitle);


        dateFont.setItalic(true);
        dateFont.setPixelSize(10);

        for(int i=1;i<=aParser->count();++i)
        {
            QGraphicsTextItem* title = new QGraphicsTextItem;
            title->setHtml(aParser->decodeHtml(aParser->itemElement(i,RSSParser::title)));
            title->setTextWidth(360); // tmp
            QGraphicsTextItem* date = new QGraphicsTextItem;
            date->setHtml(aParser->decodeHtml(aParser->itemElement(i,RSSParser::pubDate)));
            date->setFont(dateFont);
            SegmentWidget* segWidget = new SegmentWidget(Qt::Vertical);
            segWidget->addItem(title);
            segWidget->addItem(date);

            QGraphicsWebView* content = new QGraphicsWebView(segWidget);
            content->setHtml(aParser->decodeHtml(aParser->itemElement(i,RSSParser::description)));
            content->show();
            segWidget->addItem(content);
            this->addSegmentWidget(segWidget);
        }
    }
    scene()->setSceneRect(this->rect());
    this->showMaximized();
}
