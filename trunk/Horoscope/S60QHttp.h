#ifndef S60QHttp_H
#define S60QHttp_H

#include <QObject>
#include <QHttpResponseHeader>

class S60QHttpPrivate;


class S60QHttp : public QObject
{
    Q_OBJECT

public:
    S60QHttp(QObject *parent = 0);
    virtual ~S60QHttp();

    enum State {
        Running     = -2,
        Unconnected = 0, //	There is no connection to the host.
        HostLookup  = 1, //	A host name lookup is in progress.
        Connecting  = 2, //	An attempt to connect to the host is in progress.
        Sending     = 3, //	The client is sending its request to the server.
        Reading     = 4, //	The client's request has been sent and the client is reading the server's response.
        Connected   = 5, //	The connection to the host is open, but the client is neither sending a request, nor waiting for a response.
        Closing     = 6 //	The connection is closing down, but is not yet closed. (The state will be Unconnected when the connection is closed.)
    };

    enum Error {
        NoError                          = 0, // No error occurred.
        HostNotFound                     = 2, // The host name lookup failed.
        ConnectionRefused                = 3, // The server refused the connection.
        UnexpectedClose                  = 4, // The server closed the connection unexpectedly.
        InvalidResponseHeader            = 5, // The server sent an invalid response header.
        WrongContentLength               = 6, // The client could not read the content correctly because an error with respect to the content length occurred.
        Aborted                          = 7, // The request was aborted with abort().
        ProxyAuthenticationRequiredError = 9, // QHttp is using a proxy, and the proxy server requires authentication to establish a connection.
        AuthenticationRequiredError      = 8, // The web server requires authentication to complete the request.
        UnknownError                     = 1 // An error other than those specified above occurred.
    };

    int get(const QString& path, QByteArray& data);
    int get(const QString& path, QIODevice * to = 0);

    int setHost( const QString& hostName, quint16 port = 80 );

    int currentId();
    S60QHttp::Error error();
    S60QHttp::State state();
    QString errorString();

    bool isRunning();
    void setupConnection();

signals:
    void stateChanged ( int state );
    void responseHeaderReceived( const QHttpResponseHeader& );
    void requestFinished(int, bool);
    void requestStarted(int);
    void done(bool);
    void dataReadProgress(int, int);
    void errorOccurred();
    void connectionSetup(bool);

public slots:
    void abort();
    void submit();

private slots:
    void bytesWritten(qint64);

private:
    State _state;
    Error _error;

    //int requestId;
    QString _errorString;
    //QByteArray* data;
    QIODevice* to;

private:
    void changeState(S60QHttp::State);
    void setError(int, S60QHttp::Error, QString errorMessage="", bool emitSignal=true);
    //void append(QByteArray& data);
    void finished(int);
    void resetErrorStatus();

private:
    S60QHttpPrivate *d;  //pointer to implementation

private:    // Friend class definitions
    friend class S60QHttpPrivate;
};

#endif // S60QHttp_H


