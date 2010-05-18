#include <QtGui/QApplication>
#include "horoscope.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QTime>
#include <QNetworkProxyFactory>

#ifdef WRITE_DEBUG_TO_FILE
#ifdef Q_OS_SYMBIAN
const QString KLogFileName("c:\\data\\qtapplogger.txt");
#else
const QString KLogFileName("qtapplogger.txt");
#endif
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
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("decii");
    QApplication::setApplicationName("horoscope");
    QNetworkProxyFactory::setUseSystemConfiguration(true);

#ifdef WRITE_DEBUG_TO_FILE
// Prepare debug log
logfile.setParent(&a);
logfile.setFileName(KLogFileName);
logfile.open(QIODevice::Append);
qInstallMsgHandler(MyOutputHandler);
#endif

    QGraphicsView gv;
    gv.setAttribute(Qt::WA_TranslucentBackground, true);
    gv.setWindowFlags(Qt::FramelessWindowHint);
    gv.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // set transparency to view
    QPalette viewPalette = gv.palette();
    viewPalette.setBrush(QPalette::Base,QBrush(QColor(0,0,0,200)));
    gv.setPalette(viewPalette);
    gv.setAttribute(Qt::WA_OpaquePaintEvent,false);
    gv.setFrameShape(QFrame::NoFrame);

    // Resize to fit target platform
    QDesktopWidget desktopWidget;
    QRect screenRect = desktopWidget.screenGeometry();
    qDebug()<<"device screen:"<<screenRect;

#ifdef Q_OS_SYMBIAN
    const int pixelsToIgnoreOnView = 0;
    const int pixelsToIgnoreOnItem = 10;
    const int newWidth = screenRect.width() - pixelsToIgnoreOnView;
    const int newHeight = screenRect.height() - pixelsToIgnoreOnView;
    gv.resize(newWidth,newHeight);
#else
    const int pixelsToIgnoreOnView = 0;
    const int pixelsToIgnoreOnItem = 5;
    const int newWidth = 400 - pixelsToIgnoreOnView;
    const int newHeight = 450 - pixelsToIgnoreOnView;
    gv.resize(newWidth,newHeight);
#endif
    QGraphicsScene scene;
    Horoscope w;
    scene.addItem(&w);
#ifdef Q_OS_SYMBIAN
    w.resize(newWidth-pixelsToIgnoreOnItem,newHeight-pixelsToIgnoreOnItem);
#else
    w.resize(newWidth-pixelsToIgnoreOnItem,newHeight-pixelsToIgnoreOnItem);
#endif
    gv.setScene(&scene);
    gv.showFullScreen();
    return a.exec();
}


