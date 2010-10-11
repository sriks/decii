#include <QtGui/QApplication>
#include <QDebug>
#include "dpagecurl.h"
#include "samplewidget.h"
#include "dpageturneffect.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SampleWidget widget;
//    qDebug()<<"hostwidget rect:"<<widget.rect();
//    qDebug()<<"hostwidget frame rect:"<<widget.frameRect();
//    widget.setGeometry(1000,500,100,100);
//    qDebug()<<"maptoglobal:"<<widget.mapToGlobal(QPoint(0,0));
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
