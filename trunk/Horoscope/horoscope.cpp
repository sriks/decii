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
#include <QWebpage>
#include <QWebFrame>

// HTTP, HTML Specific declarations
const QString KHoroscopeRequestUrl("http://www.astroyogi.com/gadget/horoscopetodayXML.asp?up_zsign=%1");
const QString KSelectionRequested("selectionrequested");
const QString KCancelRequested("cancelrequested");
const QString KSelectionViewSource("selectionwebview_source.html");

// HTML template from which main display page has to be constructed
const QString KHoroscopeHTMLTemplateSource("<html><head><title>%1</title></head><p style=\"text-align: center;\"><font size = 5><a href = \"selectionrequested\">%2</font></p></a></br><font size = 4>%3</font></br><font size = 3>%4</font></html>");
const QString KHoroscopeSelectionPageSource("<html><head><title>Select your sign</title></head><p style=\"text-align: center\"><font size = 6>Select your sign</font><font size = 4></br><a href = \"aries\">Aries: Mar 21-Apr 20</a></br><a href = \"Taurus\">Taurus: Apr 21-May 21</a></br><a href =\"Gemini\">Gemini: May 22-Jun 21</a></br><a href = \"Cancer\">Cancer: June 22-Jul 22</a></br><a href = \"Leo\">Leo: July 23-Aug 21</a></br><a href = \"Virgo\">Virgo: Aug 22-Sep 23</a></br><a href= \"Libra\">Libra: Sept 24-Oct 23</a></br><a href = \"Scorpio\">Scorpio: Oct 24-Nov 22</a></br><a href= \"Sagittarius\">Sagittarius: Nov 23-Dec 22</a></br><a href = \"Capricorn\">Capricorn: Dec 23-Jan 20</a></br><a href = \"Aquarius\">Aquarius: Jan 21-Feb 19</a></br><a href = \"Pisces\">Pisces: Feb 20- Mar 20</a></br><a href=\"cancelrequested\">Cancel</a></font></p></html>");
const QString KElementSign("sign");
const QString KElementDate("date");
const QString KElementContent("content");
const QString KImagesRoot(":/resources/images/");

Horoscope::Horoscope(QGraphicsItem *parent)
    : QGraphicsWebView(parent)
{
    // Give an object name
    setObjectName("webview");
    this->resize(240,320);
    // Delegate link policy to ourself
    QWebPage* webpage = this->page();
    webpage->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
    this->setPage(webpage);
    connect(this,SIGNAL(linkClicked(QUrl)),this,SLOT(on_webview_linkClicked(QUrl)));

    // Create selection webview
    iSelectionWebView = new QGraphicsWebView(this);
    iSelectionWebView->setObjectName("selectionwebview");
    iSelectionWebView->setVisible(false);
    connect(this,SIGNAL(linkClicked(QUrl)),this,SLOT(on_webview_linkClicked(QUrl)));

    // Create http instance
    iHttp = new QHttp(this);
    iHttp->setObjectName("http");

    // Create states and assign object names
    QStateMachine* statemachine = new QStateMachine(this);

    iGroupState = new QState(statemachine);
    iGetSettingsState = new QState(iGroupState);
    iGetSettingsState->setObjectName("getsettings");

    iHttpRequestState = new QState(iGroupState);
    iHttpRequestState->setObjectName("httprequest");

    iDisplayContentState = new QState(iGroupState);
    iDisplayContentState->setObjectName("displaycontent");

    iDisplaySelectionState = new QState(iGroupState);
    iDisplaySelectionState->setObjectName("showselection");

    // Set initial state
    iGroupState->setInitialState(iGetSettingsState);

    // Add transitions
    iGetSettingsState->addTransition(this,SIGNAL(initiateNetworkRequest()),iHttpRequestState);
    //iHttpRequestState->addTransition(this,SIGNAL(networkResponseRecieved()),iParseXMLState);
    iHttpRequestState->addTransition(this,SIGNAL(displayContentReady()),iDisplayContentState);
    iDisplayContentState->addTransition(this,SIGNAL(linkClicked(QUrl)),iDisplaySelectionState);
    iDisplaySelectionState->addTransition(this,SIGNAL(selectionChanged()),iHttpRequestState);

    // By this step all child objects are created
    // Now connect signals to slots
    QMetaObject::connectSlotsByName(this);

    // Add states to statemachine
    statemachine->setInitialState(iGroupState);
    statemachine->start();
}

Horoscope::~Horoscope()
{
qDebug()<<"Horoscope::~Horoscope->";
// left empty
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
iCurrentResponse.clear();
iCurrentResponse = iHttp->readAll();
qDebug()<<iCurrentResponse;
if(!error && !iCurrentResponse.isEmpty())
{
    qDebug()<<"Horoscope::on_parsexml_entered->";
    qDebug()<<iCurrentResponse;

    bool isFound = false;
    QXmlStreamReader xml;
    qDebug()<<iCurrentResponse;
    xml.addData(iCurrentResponse);
    iHoroscopeData.iDescription.clear();

        while(!xml.atEnd())
        {
            xml.readNext();
            if(xml.isStartElement())
            {
                // sign //c1051525131
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
                    qDebug()<<"found\n"<<iHoroscopeData.iDescription;
                    }
                }
            }
        } // while

    emit displayContentReady();
    qDebug()<<"Horoscope::on_parsexml_entered<-";
}
qDebug()<<"Horoscope::on_http_requestFinished <-";
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


void Horoscope::on_webview_linkClicked(const QUrl & url)
{
    qDebug()<<"Horoscope::on_webview_linkClicked "<<url.toString();

    // Display selection page
    if(KSelectionRequested == url.toString())
    {
       qDebug()<<"selection source\n"<<KHoroscopeSelectionPageSource;
       iPrevHtmlContent = this->page()->mainFrame()->toHtml();
       setHtml(KHoroscopeSelectionPageSource);
       update();
    }

    else if(KCancelRequested == url.toString())
    {
        setHtml(iPrevHtmlContent);
        update();
    }

    else
    {
        if(iCurrentZodiacSign != url.toString())
        {
        iCurrentZodiacSign = url.toString();
        emit initiateNetworkRequest();
        }
        else
        {
        setHtml(iPrevHtmlContent);
        update();
        }
    }
}

void Horoscope::on_selectionwebview_linkClicked(const QUrl& aSelectedUrl)
{
    qDebug()<<"Horoscope::on_selectionwebview_linkClicked";
    iSelectionWebView->setVisible(false);
}

// eof
