#include <QtGui/QApplication>
#include "horoscope.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QTime>

const QString KLogFileName("logfile.log");
QFile logfile;

// This is the methid to handle all debug msgs
void MyOutputHandler(QtMsgType , const char *);

void MyOutputHandler(QtMsgType type, const char *msg)
{
        if(QApplication::instance()->closingDown() || false == logfile.isOpen())
        {
                return;
        }

        QString logmsg;
        logmsg += QTime::currentTime().toString() + " " + msg + "\n";

        switch (type)
    {
        case QtDebugMsg:
        {
                logmsg.insert(0,"Debug: ");
        }
            break;
        case QtCriticalMsg:
        {
                logmsg.insert(0,"Critical: ");
        }
            break;
        case QtWarningMsg:
        {
                logmsg.insert(0,"Warning: ");
        }
            break;
        case QtFatalMsg:
        {
                logmsg.insert(0,"Fatal: ");
            abort();
        }
            break;
    }

    logfile.write(logmsg.toAscii());
    logfile.flush();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("test_org");
    QApplication::setApplicationName("test_app");

// Prepare debug log
logfile.setParent(&a);

logfile.setFileName(KLogFileName);
logfile.open(QIODevice::Append);
qInstallMsgHandler(MyOutputHandler);

    QGraphicsView gv;
    gv.setAttribute(Qt::WA_TranslucentBackground, true);
    gv.setWindowFlags(Qt::FramelessWindowHint);

    // set transparency to view
    QPalette viewPalette = gv.palette();
    viewPalette.setBrush(QPalette::Base,QBrush(QColor(0,0,0,200)));
    gv.setPalette(viewPalette);
    gv.setAttribute(Qt::WA_OpaquePaintEvent,false);

    // Resize to fit target platform
    QDesktopWidget desktopWidget;
    QRect screenRect = desktopWidget.screenGeometry();
    qDebug()<<"device screen:"<<screenRect;

#ifdef Q_OS_SYMBIAN
    const int pixelsToIgnoreOnView = 0;
    const int pixelsToIgnoreOnItem = 0;
    const int newWidth = screenRect.width() - pixelsToIgnoreOnView;
    const int newHeight = screenRect.height() - pixelsToIgnoreOnView;
#else
    gv.resize(400,450);
#endif


    QGraphicsScene scene;
    Horoscope w;
    scene.addItem(&w);
#ifdef Q_OS_SYMBIAN
    w.resize(newWidth-pixelsToIgnoreOnItem,newHeight-pixelsToIgnoreOnItem);
#else
    w.resize(400-5,450-5);
#endif

    gv.setScene(&scene);
    gv.show();
    return a.exec();
}


