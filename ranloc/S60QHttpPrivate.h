
#ifndef S60QHttpPRIVATE_H
#define S60QHttpPRIVATE_H

#include <coecntrl.h>
#include <http\mhttpdatasupplier.h>
#include <http\mhttptransactioncallback.h>
#include <http\mhttpauthenticationcallback.h>
#include <es_sock.h>

#include <QString>
//#include <QByteArray>
#include <QIODevice>

/*
* Forward declarations
*/
class RHTTPSession;
class RHTTPTransaction;
class S60QHttp;

class S60QHttpPrivate  :	public CBase, public MHTTPTransactionCallback, public MHTTPDataSupplier, public MHTTPAuthenticationCallback
{
public:
    /*
    * NewL()
    *
    * Create a S60QHttpPrivate object.
    *
    * Params:
    *		iObserver:
    *
    * Returns:
    * 		A pointer to the created instance of S60QHttpPrivate
    *
    */
    //static S60QHttpPrivate* NewL(MClientObserver& iObserver);
    static S60QHttpPrivate* NewL(S60QHttp *aPublicAPI = 0);

    /*
    * NewLC()
    *
    * Create a S60QHttpPrivate object.
    *
    * Params:
    *		iObserver:
    *
    * Returns:
    * 		A pointer to the created instance of S60QHttpPrivate
    *
    */
    //static S60QHttpPrivate* NewLC(MClientObserver& iObserver);
    static S60QHttpPrivate* NewLC(S60QHttp *aPublicAPI = 0);

    /*
    * ~S60QHttpPrivate()
    *
    * Destroy the object
    *
    * Params:
    *		-
    *
    * Returns:
    * 		-
    *
    */
    ~S60QHttpPrivate();

    /*
    * IssueHTTPGetL()
    *
    * Starts a new HTTP GET transaction.
    *
    * Params:
    *		aUri: URI to get. (e.g. http://host.org")
    *
    * Returns:
    * 		-
    *
    */
    //void IssueHTTPGetL(const TDesC8& aUri);
    int IssueHTTPGetL(const QString&, QByteArray& data);
    int IssueHTTPGetL(const QString&, QIODevice* to=NULL);


    /*
    * IssueHTTPPostL()
    *
    * Starts a new HTTP POST transaction.
    *
    * Params:
    *		aUri: URI where to post the data (e.g. http://host.org")
    *		aContentType: Content type of the body, e.g. "text/plain"
    *		aBody: Body data for the transaction.
    *
    * Returns:
    * 		-
    *
    */
    void IssueHTTPPostL(const TDesC8& aUri,
                                            const TDesC8& aContentType,
                                            const TDesC8& aBody);

    /*
    * CancelTransaction()
    *
    * Closes currently running transaction and frees resources related to it.
    *
    * Params:
    *		-
    *
    * Returns:
    * 		-
    *
    */
    void CancelTransaction();

    /*
    * IsRunning()
    *
    * Checks if the transaction is running.
    *
    * Params:
    *		-
    *
    * Returns:
    * 		ETrue, if transaction is currently running.
    *
    */
    inline TBool IsRunning() { return iRunning; };

private:
    /*
    * ConstructL()
    *
    * Perform the second phase construction of a S60QHttpPrivate object.
    *
    * Params:
    *		-
    *
    * Returns:
    * 		-
    *
    */
    void ConstructL();

    /*
    * S60QHttpPrivate()
    *
    * Perform the first phase of two phase construction.
    *
    * Params:
    *		iObserver:
    *
    * Returns:
    * 		-
    *
    */
    //S60QHttpPrivate(MClientObserver& iObserver);
    S60QHttpPrivate(S60QHttp *aPublicAPI = 0);

    /*
    * SetHeaderL()
    *
    * Sets header value of an HTTP request.
    *
    * Params:
    *		aHeaders:	Headers of the HTTP request
    *		aHdrField:	Enumerated HTTP header field, e.g. HTTP::EUserAgent
    *		aHdrValue:	New value for header field
    *
    * Returns:
    * 		-
    *
    */
    void SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField,
                                    const TDesC8& aHdrValue);

/*
* From MHTTPSessionEventCallback
*/
private:
    /*
    * MHFRunL()
    *
    * Called by framework to notify about transaction events.
    *
    * Params:
    *		aTransaction: Transaction, where the event occured.
    *		aEvent: Occured event.
    *
    * Returns:
    * 		-
    *
    */
    void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

    /*
    * MHFRunError()
    *
    * Called by framework when *leave* occurs in handling of transaction event.
    *
    * Params:
    *		aError:			The leave code that occured.
    *		aTransaction:	The transaction that was being processed when leave
    *						occured.
    *		aEvent:			The event that was being processed when leave
    *						occured.
    *
    * Returns:
    * 		KErrNone, if the error was handled. Otherwise the value of aError, or
    *		some other error value. Returning error value causes causes
    *		HTTP-CORE 6 panic.
    *
    */
    TInt MHFRunError(	TInt aError,
                                            RHTTPTransaction aTransaction,
                                            const THTTPEvent& aEvent);

/*
* From MHTTPDataSupplier (needed for HTTP POST)
*/
private:
    /*
    * ReleaseData()
    *
    * Called by framework to allow data supplier to release resources
    * related to previous data part.
    *
    * Params:
    *		-
    *
    * Returns:
    * 		-
    *
    */
    void ReleaseData();

    /*
    * GetNextDataPart()
    *
    * Called when next data part is needed.
    *
    * Params:
    *		aDataPart: Must be set to point to the next data part.
    *
    * Returns:
    * 		ETrue if the provided data part is the last one. If more data parts
    *		are needed after the provided one, return EFalse.
    *
    */
    TBool GetNextDataPart(TPtrC8& aDataPart);

    /*
    * Reset()
    *
    * Called by framework to reset the data supplier to its initial state.
    *
    * Params:
    *		-
    *
    * Returns:
    *		KErrNone if successfull.
    *
    */
    TInt Reset();

    /*
    * OverallDataSize()
    *
    * Called by framework when it needs to know the size of the
    * body data.
    *
    * Params:
    *		-
    *
    * Returns:
    * 		Size of the data, or KErrNotFound (or KErrNotSupported)
    *		if the size of the data is not known.
    *
    */
    TInt OverallDataSize();

public:
    /*
     * SetupConnectionL()
     *
     * The method set the internet access point and connection setups.
     *
     */
    void SetupConnectionL();

/*
* From MHTTPAuthenticationCallback (needed for HTTP authentication)
*/
private:
    /*
    * GetCredentialsL()
    *
    * Called by framework when username and password for requested URI is
    * needed.
    *
    * Params:
    *		aURI: The URI being requested (e.g. "http://host.org")
    *		aRealm: The realm being requested (e.g. "user@host.org")
    *		aAuthenticationType: Authentication type. (e.g. "Basic")
    *		aUsername: Given user name.
    *		aPassword: Given password.
    *
    * Returns:
    * 		A pointer to the created document
    *
    */
    TBool GetCredentialsL(	const TUriC8& aURI,
                                                    RString aRealm,
                                                    RStringF aAuthenticationType,
                                                    RString& aUsername,
                                                    RString& aPassword);

private:
    // declare members
    RSocketServ 			iSocketServ;
    RConnection 			iConnection;

    RHTTPSession			iSession;
    RHTTPTransaction		iTransaction;

    //MClientObserver&		iObserver;	// Used for passing body data and
                                                                            // events to UI.
    HBufC8*					iPostData;	// Data for HTTP POST
    TBool					iRunning;	// ETrue, if transaction running

    //TBool 					iConnectionSetupDone;
public:
    bool 					iConnectionSetupDone;

private:
    int dataSize;
    QByteArray* data;
    QIODevice* to;
    S60QHttp* iPublicS60QHttp;

public:
    void submit();

public:
//    int requestId;
};

#endif // S60QHttpPRIVATE_H
