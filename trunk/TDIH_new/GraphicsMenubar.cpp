#include <QPen>
#include <QDebug>
#include <QAction>
#include <QTextDocument>
#include <QTextOption>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QGraphicsPathItem>
#include <QGraphicsBlurEffect>
#include <QGraphicsLinearLayout>
#include <QGraphicsView>
#include "GraphicsMenubar.h"

struct GraphicsMenubarPrivate {
    QGraphicsView* view; // non owning
};

GraphicsMenubar::GraphicsMenubar(Qt::Orientation orientation,QGraphicsItem* parent)
                :KSegmentWidget(orientation,parent)
{
    d = new GraphicsMenubarPrivate;
    this->setClickable(false);
    // Preparing default skin.
    QGraphicsPathItem* bkg = new QGraphicsPathItem(this);
    bkg->setOpacity(0.7);
    QColor bkgColor("#AAAAAA");
    bkg->setBrush(QBrush(bkgColor));
    bkg->setPen(QPen(Qt::NoPen));
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(20);
    bkg->setGraphicsEffect(blur);
    this->setNormalBackground(bkg);
}

GraphicsMenubar::~GraphicsMenubar() {
    qDebug()<<__PRETTY_FUNCTION__;
    delete d;
    for(int i=0;i<actions().count();++i) {
        this->removeAction(actions().at(i));
    }
    qDebug()<<__PRETTY_FUNCTION__<<" end";
}

void GraphicsMenubar::setAlignment(QAbstractScrollArea* parentScrollArea,Qt::Alignment alignment) {
    qDebug()<<__PRETTY_FUNCTION__;
    Q_UNUSED(alignment);
    connect(parentScrollArea->verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(updatePosition()));
    connect(parentScrollArea->horizontalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(updatePosition()));
    d->view = scene()->views().at(0);
    updatePosition();
    qDebug()<<__PRETTY_FUNCTION__<<" end";
}

void GraphicsMenubar::updatePosition() {
    qDebug()<<__PRETTY_FUNCTION__;
    QPointF scrollBarScenePos = d->view->mapToScene(0,d->view->verticalScrollBar()->value());
    int yPos = scrollBarScenePos.y() + (d->view->viewport()->height() - d->view->verticalScrollBar()->value());
    yPos -= this->size().height(); // adjust so that menubar is visible.
    this->setPos(0,yPos);
    qDebug()<<__PRETTY_FUNCTION__<<" end";
}

void GraphicsMenubar::addAction(QAction* action) {
    qDebug()<<__PRETTY_FUNCTION__;
    QGraphicsWidget::addAction(action);
    KSegmentWidget* menuItem = new KSegmentWidget(Qt::Vertical);
    menuItem->setObjectName(action->text());
    int preferredWidth = 70;
    menuItem->setPreferredWidth(preferredWidth);
    menuItem->setSelectable(false);

//    if(!action->icon().isNull()) {
//        menuItem->addItem(new QGraphicsPixmapItem(action->icon().pixmap()));
//        menuItem->setPreferredWidth(action->icon().pixmap().width());
//        preferredWidth = action->icon().pixmap().width();
//    }

    if(!action->text().isEmpty()) {
        QGraphicsTextItem* menuItemText = new QGraphicsTextItem(action->text());
        QTextDocument* doc = menuItemText->document();
        doc->setTextWidth(preferredWidth-20);
        QFont f = doc->defaultFont();
        f.setPixelSize(10);
        doc->setDefaultFont(f);
//        QTextOption textOpt = doc->defaultTextOption();
//        textOpt.setAlignment(Qt::AlignCenter);
//        doc->setDefaultTextOption(textOpt);
        menuItemText->setDocument(doc);
        QGraphicsWidget* item = menuItem->addItem(menuItemText);
        menuItem->layout()->setAlignment(item,Qt::AlignRight);
    }
    connect(menuItem,SIGNAL(clicked(int,QPointF)),action,SIGNAL(triggered()));
    addItem(menuItem);
    layout()->setAlignment(menuItem,Qt::AlignCenter);
}

// eof
