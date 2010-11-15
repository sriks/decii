#include "S60QHttp.h"
#include "S60QHttpPrivate.h"

#include <QTimer>

S60QHttp::S60QHttp(QObject *parent) : QObject(parent),
                                      //requestId(0),
                                      _state(Unconnected),
                                      _error(NoError),
                                      //data(NULL)
                                      to(NULL)

{
     QT_TRAP_THROWING(d = S60QHttpPrivate::NewL(this));

     /*if( this->d ) {
         this->d->SetupConnectionL();
     }*/


}

S60QHttp::~S60QHttp()
{
    delete d;
}

void S60QHttp::changeState(S60QHttp::State value) {
    this->_state = value;

    emit stateChanged(this->_state);
}

void S60QHttp::setupConnection() {
    this->d->SetupConnectionL();
}

bool S60QHttp::isRunning() {
    return this->d->IsRunning();
}

int S60QHttp::currentId(){
    return -1; //this->d->i
}

int S60QHttp::setHost( const QString& hostName, quint16 port  ) {
    return -1;
}

S60QHttp::State S60QHttp::state() {
    return _state;
}

S60QHttp::Error S60QHttp::error() {
    return _error;
}

void S60QHttp::abort() {
    this->d->CancelTransaction();
}

void S60QHttp::setError(int requestId, S60QHttp::Error value, QString errorMessage, bool emitSignal) {
    this->_error = value;
    this->_errorString = "> "+errorMessage;

    if( emitSignal ) { emit this->errorOccurred(); }
    finished(requestId);
    emit done(true);
}

void S60QHttp::finished(int requestId) {
    emit requestFinished(requestId, (( this->_error != S60QHttp::NoError ) ? true : false) );
    //this->requestId++;
}

void S60QHttp::submit() {
    this->d->submit();
}

int S60QHttp::get(const QString& aUri, QByteArray& data) {

    //this->data = &data;
//    this->to = to;
   /* if( this->to ) {
        connect(this->to, SIGNAL(bytesWritten(qint64)), this , SLOT(bytesWritten(qint64)));
    }*/

    //this->requestId++;
    this->_error = S60QHttp::NoError;
    this->_state = S60QHttp::Unconnected;

    int id = this->d->IssueHTTPGetL(aUri, data);

    if( id != -1 ) {

        //this->d->submit();
        QTimer::singleShot(0, this, SLOT(submit()));


    } /*else {
        this->d->iConnectionSetupDone = false;
        //this->d->iSession
    }*/

    return id;
}

//int S60QHttp::get(const QString& aUri, QByteArray& data) {
int S60QHttp::get(const QString& aUri, QIODevice* to) {

    //this->data = &data;
    this->to = to;
   /* if( this->to ) {
        connect(this->to, SIGNAL(bytesWritten(qint64)), this , SLOT(bytesWritten(qint64)));
    }*/

    //this->requestId++;
    this->_error = S60QHttp::NoError;
    this->_state = S60QHttp::Unconnected;

    int id = this->d->IssueHTTPGetL(aUri, this->to);

    if( id != -1 ) {
        this->d->submit();
    } /*else {
        this->d->iConnectionSetupDone = false;
        //this->d->iSession
    }*/

    return id;
}

QString S60QHttp::errorString() {

    QString resp = "";
    switch( this->_error ) {
    case S60QHttp::NoError: resp = "No error occurred"; break;
    case S60QHttp::HostNotFound : resp = "The host name lookup failed"; break;
    case S60QHttp::ConnectionRefused : resp = "The server refused the connection"; break;
    case S60QHttp::UnexpectedClose : resp = "The server closed the connection unexpectedly"; break;
    case S60QHttp::InvalidResponseHeader : resp = "The server sent an invalid response header"; break;
    case S60QHttp::WrongContentLength : resp="The client could not read the content correctly because an error with respect to the content length occurred"; break;
    case S60QHttp::Aborted : resp="The request was aborted with abort()"; break;
    case S60QHttp::ProxyAuthenticationRequiredError : resp="QHttp is using a proxy, and the proxy server requires authentication to establish a connection"; break;
    case S60QHttp::AuthenticationRequiredError : resp="The web server requires authentication to complete the request"; break;
    case S60QHttp::UnknownError: resp = ( this->_errorString != "" ) ? this->_errorString : resp="An error other than those specified above occurred"; break;
    }

    this->_errorString = resp;

    return resp;
}

void S60QHttp::resetErrorStatus() {
    this->_errorString = "";
    this->_error = S60QHttp::NoError;
}

void S60QHttp::bytesWritten(qint64 value){
    emit done( (value>=0) ? false : true);
}

// --------------------------------------------------------

/*S60QHttpResponseHeader::S60QHttpResponseHeader(){
    this->_statusCode = -1;
    this->_reasonPhrase = "";
    this->minorVer = -1;
    this->majorVer = -1;
}

S60QHttpResponseHeader::S60QHttpResponseHeader(const S60QHttpResponseHeader& value){
    this->operator=(value);
}

S60QHttpResponseHeader::S60QHttpResponseHeader(const QString& str){
    this->_reasonPhrase = str;
}

S60QHttpResponseHeader::S60QHttpResponseHeader(int code, const QString& text, int majorVer, int minorVer ){
    this->setStatusLine(code, text, majorVer, minorVer);
}

S60QHttpResponseHeader::~S60QHttpResponseHeader(){

}

int S60QHttpResponseHeader::statusCode() const {
    return this->_statusCode;
}

int S60QHttpResponseHeader::minorVersion(){
    return this->minorVer;
}

int S60QHttpResponseHeader:: majorVersion(){
    return this->majorVer;
}

QString S60QHttpResponseHeader::reasonPhrase(){
    return _reasonPhrase;
}

void S60QHttpResponseHeader::setStatusLine ( int code, const QString& text, int majorVer, int minorVer){
    this->_statusCode = code;
    this->_reasonPhrase = text;
    this->minorVer = minorVer;
    this->majorVer = majorVer;
}

QString S60QHttpResponseHeader::toString() const{
    return this->_reasonPhrase;
}

S60QHttpResponseHeader& S60QHttpResponseHeader::operator= ( const S60QHttpResponseHeader& header ){

    if( this != &header ) return *this;

    this->_statusCode = header._statusCode;
    this->_reasonPhrase = header._reasonPhrase;
    this->minorVer = header.minorVer;
    this->majorVer = header.majorVer;

    return  *this;
}
*/

