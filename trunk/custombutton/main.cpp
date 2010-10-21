#include <QtGui/QApplication>
#include <QVBoxLayout>
#include "custombutton.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    QVBoxLayout vLayout;

    // Button with quality rendering
    CustomButton* myButton = new CustomButton("Button 1");
    myButton->setImproveRendering(true);
    vLayout.addWidget(myButton);

    // Normal button
    vLayout.addWidget(new CustomButton("Button 2"));

    // Glass Button
    CustomButton* glassButton = new CustomButton("Button 3");
    glassButton->setImproveRendering(true);
    glassButton->setGlassEffect(true);
    vLayout.addWidget(glassButton);

    w.setLayout(&vLayout);
    w.show();
    return a.exec();
}
