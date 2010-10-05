#include <QtGui/QApplication>
#include "dpagecurl.h"
#include "samplewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SampleWidget widget;
    widget.show();

    DPageCurl pagecurl;
    pagecurl.doCurl(&widget,40,40);
    return a.exec();
}
