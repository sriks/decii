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
#include <QApplication>
#include <QSettings>
#include <QDesktopWidget>

// HTTP, HTML Specific declarations
const QString KHoroscopeRequestUrl("http://www.astroyogi.com/gadget/horoscopetodayXML.asp?up_zsign=%1");
const QString KSelectionRequested("selectionrequested");
const QString KCancelRequested("cancelrequested");
const QString KSelectionViewSource("selectionwebview_source.html");
const QString KSavedZodiacSign("savedzodiacsign");

// HTML template from which main display page has to be constructed
const QString KHoroscopeHTMLTemplateSource("<html><head><title>%1</title></head><p style=\"text-align: center;\"><font size = 6><a href = \"selectionrequested\" style=\"color:#99FF33 \">%2</font></a></br><font size = 3  color = \"white\">%3</font></br></br><font size = 3 color = \"white\">%4</font></p></html>");
//const QString KHoroscopeSelectionPageSource("<html><head><title>Select your sign</title></head><p style=\"text-align: center\"><font size = 6 color = #99FF33>Select your sign</font></br><font size = 4></br><a href = \"Aries\">Aries: Mar 21-Apr 20</a></br><a href = \"Taurus\">Taurus: Apr 21-May 21</a></br><a href =\"Gemini\">Gemini: May 22-Jun 21</a></br><a href = \"Cancer\">Cancer: June 22-Jul 22</a></br><a href = \"Leo\">Leo: July 23-Aug 21</a></br><a href = \"Virgo\">Virgo: Aug 22-Sep 23</a></br><a href= \"Libra\">Libra: Sept 24-Oct 23</a></br><a href = \"Scorpio\">Scorpio: Oct 24-Nov 22</a></br><a href= \"Sagittarius\">Sagittarius: Nov 23-Dec 22</a></br><a href = \"Capricorn\">Capricorn: Dec 23-Jan 20</a></br><a href = \"Aquarius\">Aquarius: Jan 21-Feb 19</a></br><a href = \"Pisces\">Pisces: Feb 20- Mar 20</a></br><a href=\"cancelrequested\">Cancel</a></font></p></html>");
const QString KHoroscopeSelectionPageSource("<html><head><title>Select your sign</title></head><p style=\"text-align: center\"><font size = 6 color = #99FF33>Select your sign</font></br><font size = 4></br><a href = \"Aries\" style=\"color:#FFFFFF \">Aries: Mar 21-Apr 20</a></br><a href = \"Taurus\"style=\"color:#FFFFFF \">Taurus: Apr 21-May 21</a></br><a href =\"Gemini\"style=\"color:#FFFFFF \">Gemini: May 22-Jun 21</a></br><a href = \"Cancer\"style=\"color:#FFFFFF \">Cancer: June 22-Jul 22</a></br><a href = \"Leo\"style=\"color:#FFFFFF \">Leo: July 23-Aug 21</a></br><a href = \"Virgo\"style=\"color:#FFFFFF \">Virgo: Aug 22-Sep 23</a></br><a href= \"Libra\"style=\"color:#FFFFFF \">Libra: Sept 24-Oct 23</a></br><a href = \"Scorpio\"style=\"color:#FFFFFF \">Scorpio: Oct 24-Nov 22</a></br><a href= \"Sagittarius\"style=\"color:#FFFFFF \">Sagittarius: Nov 23-Dec 22</a></br><a href = \"Capricorn\"style=\"color:#FFFFFF \">Capricorn: Dec 23-Jan 20</a></br><a href = \"Aquarius\"style=\"color:#FFFFFF \">Aquarius: Jan 21-Feb 19</a></br><a href = \"Pisces\"style=\"color:#FFFFFF \">Pisces: Feb 20- Mar 20</a></br><a href=\"cancelrequested\"style=\"color:#99FF33 \">Cancel</a></font></p></html>");
const QString KElementSign("sign");
const QString KElementDate("date");
const QString KElementContent("content");
const QString KImagesRoot(":/resources/images/");

Horoscope::Horoscope(QGraphicsItem *parent)
    : QGraphicsWebView(parent)
{

    // Give an object name
    setObjectName("webview");

    // Delegate link policy to ourself
    QWebPage* webpage = this->page();
    webpage->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
    this->setPage(webpage);
    connect(this,SIGNAL(linkClicked(QUrl)),this,SLOT(on_webview_linkClicked(QUrl)),Qt::UniqueConnection);

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
    iGetSettingsState->addTransition(this,SIGNAL(selectionChanged()),iHttpRequestState);
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
// Save settings
QSettings settings(QApplication::organizationName(),QApplication::applicationName());
settings.setValue(KSavedZodiacSign,iCurrentZodiacSign);
qDebug()<<"Horoscope::~Horoscope<-";
}

void Horoscope::keyPressEvent(QKeyEvent* keyevent)
{
    switch(keyevent->key())
    {
    case Qt::Key_Escape:
        {
            // accept so that it is not propogated
            keyevent->accept();
            qApp->quit();
        }
        break;
    }
}

void Horoscope::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = (event->globalPos() - this->geometry().topLeft().toPoint());
        event->accept();
    }
}

void Horoscope::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPoint newpos = event->globalPos() - dragPosition;

        this->moveBy(newpos.x(),newpos.y());
        event->accept();
    }
}

void Horoscope::on_getsettings_entered()
{
qDebug()<<"on_getsettings_entered->";

    QSettings settings(QApplication::organizationName(),QApplication::applicationName());
    iCurrentZodiacSign = settings.value(KSavedZodiacSign).toString();
    qDebug()<<"Horoscope::on_getsettings_entered():"<<iCurrentZodiacSign;

    // First time app is started
    if(iCurrentZodiacSign.isEmpty())
    {
    qDebug()<<"No last saved settings, showing zodiac sign selection";
    setHtml(KHoroscopeSelectionPageSource);
    update();
    }

    else
    {
    emit initiateNetworkRequest();
    }
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

    // Cancel selection page
    else if(KCancelRequested == url.toString())
    {
        if(!iPrevHtmlContent.isEmpty())
        {
        qDebug()<<"Horoscope::on_webview_linkClicked cancel:"<<url.toString();
        qDebug()<<"prevcontent\n"<<iPrevHtmlContent;
        setHtml(iPrevHtmlContent);
        update();
        }
    }

    // Some zodiac sign selected
    else
    {
        qDebug()<<"Horoscope::on_webview_linkClicked sign change:"<<url.toString();
        if(iCurrentZodiacSign != url.toString())
        {
        iCurrentZodiacSign = url.toString();
        emit selectionChanged();
        }
        else
        {
            if(!iPrevHtmlContent.isEmpty())
            {
            setHtml(iPrevHtmlContent);
            update();
            }
        }
    }
}


// eof
