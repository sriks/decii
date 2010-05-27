#include <QtGui/QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include "clockwidget.h"
#include "analoghand.h"

//#define SHOW_IN_LAYOUT
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifndef SHOW_IN_LAYOUT
    ClockWidget clockWidget;
    clockWidget.showMaximized();
#else
    QWidget w;
    QVBoxLayout vlayout;
    ClockWidget clockWidget;
    clockWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    clockWidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vlayout.addWidget(&clockWidget);

    QLabel label;
    label.setText("Qt Clock");
    label.setFont(QFont("Arial",15));
    vlayout.addWidget(&label,0,Qt::AlignCenter);

    w.setLayout(&vlayout);
    w.showMaximized();
   // w.setWindowFlags(Qt::WindowStaysOnTopHint);
#endif
    return a.exec();
}



