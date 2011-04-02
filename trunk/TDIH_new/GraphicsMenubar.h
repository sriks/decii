#ifndef GRAPHICSMENUBAR_H
#define GRAPHICSMENUBAR_H

#include "KSegmentWidget.h"

class GraphicsMenubarPrivate;
class QAction;
class QAbstractScrollArea;
class GraphicsMenubar : public KSegmentWidget
{
    Q_OBJECT
public:
    GraphicsMenubar(Qt::Orientation orientation=Qt::Horizontal,QGraphicsItem* parent=0);
    ~GraphicsMenubar();
    void setAlignment(QAbstractScrollArea* parentScrollArea,Qt::Alignment);
    void addAction(QAction* action);

private slots:
    void updatePosition();

private:
    GraphicsMenubarPrivate* d;
};

#endif // GRAPHICSMENUBAR_H
