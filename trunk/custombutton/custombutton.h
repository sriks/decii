#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QAbstractButton>
#include <QPushButton>

class CustomButton : public QAbstractButton
{
public:
    CustomButton(const QString& aButtonText,QWidget* aParent = NULL);
    ~CustomButton();

    // Adds glass effect
    void setGlassEffect(bool aValue){iGlassEffect = aValue;}
    bool glassEffect(){return iGlassEffect;}

    // Improves rendering at the cost of performance
    void setImproveRendering(bool aValue){iImproveRendering = aValue;}
    bool improveRendering(){return iImproveRendering;}

protected:
    // From QAbstractButton
    virtual void paintEvent(QPaintEvent *e);

    // From QWidget
    // Return the size occupied by this widget
    virtual QSize sizeHint() const;

private:
    bool iImproveRendering;
    bool iGlassEffect;
};

#endif // CUSTOMBUTTON_H
