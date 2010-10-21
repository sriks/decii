#include <QPainter>
#include <QTransform>
#include <QDebug>
#include <QLinearGradient>
#include "custombutton.h"

CustomButton::CustomButton(const QString& aButtonText,QWidget* aParent)
    :QAbstractButton(aParent),
     iImproveRendering(false)
{
   setText(aButtonText);
   setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

CustomButton::~CustomButton()
{
// left blank
}


QSize CustomButton::sizeHint() const
{
    // Use this extrasize just incase
    const int extrasize(50);

    // Use fontmetrics to calculate the size occupied by current text
    return QSize(fontMetrics().width(text())+extrasize,
                 fontMetrics().height()+extrasize);
}

void CustomButton::paintEvent(QPaintEvent *e)
{
// For simplicity im ignoring paintevent
Q_UNUSED(e);
const int roundness(5);
const int penWidth(1);

    // Create a painterpath, this is the region in which we paint
    QPainterPath paintPath;
    paintPath.addRoundedRect(rect(),roundness,roundness);

    // Save the rect in which we paint since the size is static
    QRect paintRect = paintPath.boundingRect().toRect();

    // Initialize painter on this paintdevice
    QPainter p(this);

    // Check if rendering has to be improved
    if(improveRendering())
    {
        p.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    }

    // Glass effect
    if(glassEffect())
    {
    // Save painter state as we are about to change it
    p.save();

    // Create base gradient
    QColor color1(Qt::gray),color2;
    (isDown()|!isEnabled())?(color2 = Qt::gray):(color2 = Qt::black);
    QLinearGradient gradient(0, 0, 0, paintRect.height());
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, color1);
    gradient.setColorAt(0.6, color2);
    QBrush brush(gradient);
    p.setBrush(brush);
    p.fillPath(paintPath,brush);

    // glass highligh
    p.setBrush(QBrush(Qt::gray));
    p.setPen(QPen(QBrush(Qt::white), 0.1));
    p.setOpacity(0.30);

    int startAngle = 30 * 16;
    int spanAngle = 120 * 16;
    p.drawRect(1,1,paintRect.width()-2,paintRect.height()/2);

    // restore painter to last saved state
    p.restore();
    }

    // Draw without effect
    else
    {
    p.fillPath(paintPath,QBrush(QColor( (isDown()|!isEnabled())?(Qt::gray):(Qt::black)) ));
    }

    // Draw button text
    p.setPen(QPen(QBrush(QColor((isDown()|!isEnabled())?(Qt::black):(Qt::green))),penWidth));
    p.drawText(paintRect,Qt::AlignCenter,text());
}

// eof
