#include <QtGui/QApplication>
#include <QDebug>
#include "dpagecurl.h"
#include "samplewidget.h"
#include "dpageturneffect.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SampleWidget widget;
    widget.show();

    DPageTurnEffect effect;
    effect.setWidget(&widget);
    effect.startEffect();


//    DPageCurl pagecurl;
//    pagecurl.setWidget(&widget);
//    int count = 1;
//    while(1)
//    {
//        QPixmap pagecutpixmap = pagecurl.nextPageCut();
//        if(pagecutpixmap.isNull() /*|| count > 1*/)
//        {
//            qDebug()<<"Invalid pixmap "<<count;
//            break;
//        }
//        pagecutpixmap.save("pagecut_"+QString().setNum(count)+".png","png",100);
//        pagecurl.nextCurlCut().save("pagecurl_"+QString().setNum(count)+".png","png",100);
//        count++;
//    }
    return a.exec();
}
