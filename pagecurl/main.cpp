#include <QtGui/QApplication>
#include "dpagecurl.h"
#include "samplewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SampleWidget widget;
    widget.show();

    DPageCurl pagecurl;
    pagecurl.setWidget(&widget);

    int count = 1;
    while(1)
    {
     QPixmap pixmap(widget.size());
     pixmap.fill(QColor(Qt::transparent));
     pixmap = pagecurl.nextPageCut();
     if(!pixmap.isNull()){break;}
     count++;
     pixmap.save("page"+QString().setNum(count),"png",100);

     QPixmap curlPixmap = pagecurl.nextCurlCut();
     if(!curlPixmap.isNull()){break;}
     curlPixmap.save("curl"+QString().setNum(count),"png",100);
    }

    return a.exec();
}
