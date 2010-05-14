#include "horoscope.h"
#include <QGridLayout>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include <QXmlStreamReader>
#include <QGraphicsWebView>
#include <QDebug>
#include <QState>
#include <QStateMachine>

// HTTP, HTML Specific declarations
const QString KHoroscopeRequestUrl("http://www.astroyogi.com/gadget/horoscopetodayXML.asp?up_zsign=%1");

// HTML template from which main display page has to be constructed
const QString KHoroscopeHTMLTemplateSource("<html><head><title>%1</title></head><font size = 5>%2</font></br><font size = 4>%3</font></br><font size = 3>%4</font></html>");
const QString KElementSign("sign");
const QString KElementDate("date");
const QString KElementContent("content");
const QString KImagesRoot(":/resources/images/");

Horoscope::Horoscope(QGraphicsItem *parent)
    : QGraphicsWebView(parent)
{
    // Give an object name
    setObjectName("webview");

    // Create selection webview
    iSelectionWebView = new QGraphicsWebView(this);
    iSelectionWebView->setObjectName("selectionwebview");
    iSelectionWebView->setVisible(false);

    // Create http instance
    iHttp = new QHttp(this);
    iHttp->setObjectName("http");

    // Create states and assign object names
    iGroupState = new QState;
    iGetSettingsState = new QState(iGroupState);
    iGetSettingsState->setObjectName("getsettings");

    iHttpRequestState = new QState(iGroupState);
    iHttpRequestState->setObjectName("httprequest");

    iParseXMLState = new QState(iGroupState);
    iParseXMLState->setObjectName("parsexml");

    iDisplayContentState = new QState(iGroupState);
    iDisplayContentState->setObjectName("displaycontent");

    iDisplaySelectionState = new QState(iGroupState);
    iDisplaySelectionState->setObjectName("showselection");

    // Set initial state
    iGroupState->setInitialState(iGetSettingsState);

    // Add transitions
    iGetSettingsState->addTransition(this,SIGNAL(initiateNetworkRequest()),iHttpRequestState);
    iHttpRequestState->addTransition(this,SIGNAL(networkResponseRecieved()),iParseXMLState);
    iParseXMLState->addTransition(this,SIGNAL(displayContentReady()),iDisplayContentState);
    iDisplayContentState->addTransition(this,SIGNAL(linkClicked(QUrl)),iDisplaySelectionState);
    iDisplaySelectionState->addTransition(this,SIGNAL(selectionChanged()),iHttpRequestState);

    // By this step all child objects are created
    // Now connect signals to slots
    QMetaObject::connectSlotsByName(this);
    QObject::connect(iGroupState,SIGNAL(entered()),this,SLOT(on_getsettings_entered()));
    // Add states to statemachine
    QStateMachine statemachine;
    statemachine.addState(iGroupState);
    statemachine.setInitialState(iGroupState);
    statemachine.start();

    if(statemachine.isRunning())
    {
    qDebug()<<"state machine started";
    }

    else
    {
    qDebug()<<"state machine NOT started";
    }
}

Horoscope::~Horoscope()
{
qDebug()<<"Horoscope::~Horoscope->";
/*   if(iGroupState)
    {
       delete iGroupState;
    }*/
qDebug()<<"Horoscope::~Horoscope<-";
}

void Horoscope::keyPressEvent(QKeyEvent* keyevent)
{
    /*switch(keyevent->key())
    {
    case Qt::Key_Escape:
        {
            // accept so that it is not propogated
            keyevent->accept();
            this->close()
        }
        break;
    }*/
}

void Horoscope::mousePressEvent(QMouseEvent *event)
{
   /* if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - this->geometry().topLeft().topLeft();
        event->accept();
    }*/
}

void Horoscope::mouseMoveEvent(QMouseEvent *event)
{
    /*if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }*/
}

void Horoscope::on_getsettings_entered()
{
qDebug()<<"on_getsettings_entered->";
    // TODO: This is used for test, replace with QSettings
    iCurrentZodiacSign = "Leo";
    emit initiateNetworkRequest();
qDebug()<<"on_getsettings_entered<-";
}

void Horoscope::on_httprequest_entered()
{
qDebug()<<"Horoscope::on_httprequest_entered->";
    // Prepare request url
    QString requestUrl = KHoroscopeRequestUrl.arg(iCurrentZodiacSign);
    qDebug()<<"request url:"<<requestUrl;
    iHttp->setHost("www.astroyogi.com");
    iHttp->get(requestUrl);
qDebug()<<"Horoscope::on_httprequest_entered<-";
}

void Horoscope::on_http_requestFinished (int id,bool error)
{
qDebug()<<"Horoscope::on_http_requestFinished ->";
qDebug()<<"error"<<error;

if(!error)
{
emit networkResponseRecieved();
}

/*
    if(!error && parseXML(iHttp->readAll(),iHoroscopeData))
    {
        // Found required content
        qDebug()<<"\n"<<iHoroscopeData.iZodiacSign<<"\n"<<iHoroscopeData.iDate<<"\n"<<iHoroscopeData.iDescription;
        QString displayContent = QString(KHoroscopeHTMLTemplateSource).arg(iCurrentZodiacSign)
                                                                      .arg(iHoroscopeData.iZodiacSign)
                                                                      .arg(iHoroscopeData.iDate)
                                                                      .arg(iHoroscopeData.iDescription);
        qDebug()<<"\nDisplay content\n"<<displayContent;
        this->setHtml(displayContent);
        this->update();
    }*/
qDebug()<<"Horoscope::on_http_requestFinished <-";
}


void Horoscope::on_parsexml_entered()
{
qDebug()<<"Horoscope::on_parsexml_entered->";
bool isFound = false;
QXmlStreamReader xml;
xml.addData(iHttp->readAll());
iHoroscopeData.iDescription.clear();

    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement())
        {
            // sign
            if(KElementSign == xml.name().toString())
            {
                iHoroscopeData.iZodiacSign = xml.readElementText();
            }

            // date
            else if(KElementDate == xml.name().toString())
            {
                iHoroscopeData.iDate = xml.readElementText();
            }

            // content
            else if(KElementContent == xml.name().toString())
            {
                iHoroscopeData.iDescription = xml.readElementText();
                if(!iHoroscopeData.iDescription.isEmpty())
                {
                isFound = true;
                }
            }
        }
    } // while
qDebug()<<"Horoscope::on_parsexml_entered<-";
}


void Horoscope::on_displaycontent_entered()
{
qDebug()<<"Horoscope::on_displaycontent_entered->";
    QString displayContent;

    // Found required content
    if(!iHoroscopeData.iDescription.isEmpty())
    {
    qDebug()<<"\n"<<iHoroscopeData.iZodiacSign<<"\n"<<iHoroscopeData.iDate<<"\n"<<iHoroscopeData.iDescription;
    displayContent = QString(KHoroscopeHTMLTemplateSource).arg(iCurrentZodiacSign)
                                                          .arg(iHoroscopeData.iZodiacSign)
                                                          .arg(iHoroscopeData.iDate)
                                                          .arg(iHoroscopeData.iDescription);
    qDebug()<<"\nDisplay content\n"<<displayContent;
    }

    // update with error
    else
    {
    displayContent += "Opps! Error in fetching your horoscope!";
    }

    this->setHtml(displayContent);
    this->update();
qDebug()<<"Horoscope::on_displaycontent_entered<-";
}

// eof
