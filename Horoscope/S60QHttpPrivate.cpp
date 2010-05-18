
#include "S60QHttp.h"
#include "S60QHttpPrivate.h"

#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <uri8.h>
#include <http.h>
#include <chttpformencoder.h>
#include <HttpStringConstants.h>
#include <http\RHTTPTransaction.h>
#include <http\RHTTPSession.h>
#include <http\RHTTPHeaders.h>
//#include <HTTPClientExample.rsg>

#include <COMMDB.H> 		// Communications database
#include <CDBPREFTABLE.H>	// Connection Preferences table
#include <CommDbConnPref.h>

//#include "Client.pan"
//#include "Client.hrh"
#include <QMessageBox>

// Used user agent for requests
_LIT8(KUserAgent, "Geobino 1.0.1");

// This client accepts all content types.
// (change to e.g. "text/plain" for plain text only)
//_LIT8(KAccept, "*/*");
//_LIT8(KAccept, "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
//_LIT8(KAccept, "text/html,application/xhtml+xml,application/xml,text/plain;charset=UTF-8");
_LIT8(KAccept, "text/plain;charset=UTF-8");
const TInt KStatustextBufferSize = 32;
const TInt KInfotextBufferSize = 64;
const TInt KURIBufferSize = 128;
const TInt KDefaultBufferSize = 256;

// ----------------------------------------------------------------------------
// S60QHttpPrivate::NewL()
//
// Creates instance of S60QHttpPrivate.
// ----------------------------------------------------------------------------
//S60QHttpPrivate* S60QHttpPrivate::NewL(MClientObserver& aObserver)
S60QHttpPrivate* S60QHttpPrivate::NewL(S60QHttp *aPublicAPI)
{
    //S60QHttpPrivate* self = S60QHttpPrivate::NewLC(aObserver);
    S60QHttpPrivate* self = S60QHttpPrivate::NewLC(aPublicAPI);
    CleanupStack::Pop(self);
    return self;
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::NewLC()
//
// Creates instance of S60QHttpPrivate.
// ----------------------------------------------------------------------------
//S60QHttpPrivate* S60QHttpPrivate::NewLC(MClientObserver& aObserver)
S60QHttpPrivate* S60QHttpPrivate::NewLC(S60QHttp *aPublicAPI)
{
    //S60QHttpPrivate* self = new (ELeave) S60QHttpPrivate(aObserver);
    S60QHttpPrivate* self = new (ELeave) S60QHttpPrivate(aPublicAPI);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::S60QHttpPrivate()
//
// First phase constructor.
// ----------------------------------------------------------------------------
/*S60QHttpPrivate::S60QHttpPrivate(MClientObserver& aObserver)
:	iObserver(aObserver),*/
S60QHttpPrivate::S60QHttpPrivate(S60QHttp *aPublicAPI)
:	iPostData(NULL),
        iRunning(EFalse),
        //data(NULL)
        iConnectionSetupDone(false),
        to(NULL),
        iPublicS60QHttp(aPublicAPI)
        //requestId(-1)
{
    this->data = new QByteArray();
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::~S60QHttpPrivate()
//
// Destructor.
// ----------------------------------------------------------------------------
S60QHttpPrivate::~S60QHttpPrivate()
{
    iSession.Close();

    // and finally close handles
    iConnection.Close();
    iSocketServ.Close();

    delete iPostData;
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::ConstructL()
//
// Second phase construction.
// ----------------------------------------------------------------------------
void S60QHttpPrivate::ConstructL()
{

    // Open RHTTPSession with default protocol ("HTTP/TCP")
    TRAPD(err, iSession.OpenL());
    if(err != KErrNone)
    {
        // Most common error; no access point configured, and session creation
        // leaves with KErrNotFound.
        _LIT(KErrMsg,
                "Cannot create session. Is internet access point configured?");
        _LIT(KExitingApp, "Exiting app.");
        CEikonEnv::Static()->InfoWinL(KErrMsg, KExitingApp);
        User::Leave(err);
    }

    // Install this class as the callback for authentication requests. When
    // page requires authentication the framework calls GetCredentialsL to get
    // user name and password.
    InstallAuthenticationL(iSession);
    //SetupConnectionL();
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::SetHeaderL()
//
// Used to set header value to HTTP request
// ----------------------------------------------------------------------------
void S60QHttpPrivate::SetHeaderL(RHTTPHeaders aHeaders,
                                                         TInt aHdrField,
                                                         const TDesC8& aHdrValue)
{
    RStringF valStr = iSession.StringPool().OpenFStringL(aHdrValue);
    CleanupClosePushL(valStr);
    THTTPHdrVal val(valStr);
    aHeaders.SetFieldL(iSession.StringPool().StringF(aHdrField,
            RHTTPSession::GetTable()), val);
    CleanupStack::PopAndDestroy(); // valStr
}


void S60QHttpPrivate::submit() {
    iTransaction.SubmitL();
    //emit this->iPublicS60QHttp->requestStarted(this->requestId);
    emit this->iPublicS60QHttp->requestStarted(iTransaction.Id());

    iRunning = ETrue;

    //_LIT(KConnecting,"Connecting...");    iObserver.ClientEvent(KConnecting);
    this->iPublicS60QHttp->changeState(S60QHttp::Connecting);
}

// ----------------------------------------------------------------------------
// S60QHttpPrivate::IssueHTTPGetL()
//
// Start a new HTTP GET transaction.
// ----------------------------------------------------------------------------
//void S60QHttpPrivate::IssueHTTPGetL(const TDesC8& aUri)
//int S60QHttpPrivate::IssueHTTPGetL(const QString& value, QByteArray& data)
int S60QHttpPrivate::IssueHTTPGetL(const QString& value, QIODevice* to)
{
    if(iRunning)  return -2;

    //this->data = &data;

    delete this->data; this->data = new QByteArray();
    this->to = to;

    if( this->to ) {
        this->to->open(QIODevice::ReadWrite);
    }

    this->dataSize = 0;

   // this->data->clear();


    TPtrC16 textPtr(reinterpret_cast<const TUint16*>(value.utf16()));
    TBuf8<512> aUri;    aUri.Copy(textPtr);

    SetupConnectionL();

    // Parse string to URI (as defined in RFC2396)
    TUriParser8 uri;
    uri.Parse(aUri);

    // Get request method string for HTTP GET
    RStringF method = iSession.StringPool().StringF(HTTP::EGET, RHTTPSession::GetTable());

    // Open transaction with previous method and parsed uri. This class will
    // receive transaction events in MHFRunL and MHFRunError.
    iTransaction = iSession.OpenTransactionL(uri, *this, method);

    // Set headers for request; user agent and accepted content type
    RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
    SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
    SetHeaderL(hdr, HTTP::EAccept, KAccept);

    /*MHTTPDataSupplier* dataSupplier = this;
    iTransaction.Request().SetBody(*dataSupplier);*/
    //this->requestId = iTransaction.Id();
    // Submit the transaction. After this the framework will give transaction
    // events via MHFRunL and MHFRunError.
    /*iTransaction.SubmitL();
    //emit this->iPublicS60QHttp->requestStarted(this->requestId);
    emit this->iPublicS60QHttp->requestStarted(iTransaction.Id());

    iRunning = ETrue;

    //_LIT(KConnecting,"Connecting...");    iObserver.ClientEvent(KConnecting);
    this->iPublicS60QHttp->changeState(S60QHttp::Connecting);*/
    //return this->requestId;
    return iTransaction.Id();
}

int S60QHttpPrivate::IssueHTTPGetL(const QString& value, QByteArray& data)
{
    if(iRunning)  return -2;

    this->data = &data;

    this->dataSize = 0;

    this->data->clear();


    TPtrC16 textPtr(reinterpret_cast<const TUint16*>(value.utf16()));
    TBuf8<512> aUri;    aUri.Copy(textPtr);

    SetupConnectionL();

    // Parse string to URI (as defined in RFC2396)
    TUriParser8 uri;
    uri.Parse(aUri);

    // Get request method string for HTTP GET
    RStringF method = iSession.StringPool().StringF(HTTP::EGET, RHTTPSession::GetTable());

    // Open transaction with previous method and parsed uri. This class will
    // receive transaction events in MHFRunL and MHFRunError.
    iTransaction = iSession.OpenTransactionL(uri, *this, method);

    // Set headers for request; user agent and accepted content type
    RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
    SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
    SetHeaderL(hdr, HTTP::EAccept, KAccept);


    return iTransaction.Id();
}

// ----------------------------------------------------------------------------
// S60QHttpPrivate::IssueHTTPPostL()
//
// Start a new HTTP POST transaction.
// ----------------------------------------------------------------------------
void S60QHttpPrivate::IssueHTTPPostL(const TDesC8& aUri,
                                                                 const TDesC8& aContentType,
                                                                 const TDesC8& aBody)
{
    if(iRunning)  return;

    this->dataSize = 0;
    SetupConnectionL();

    // Parse string to URI
    TUriParser8 uri;
    uri.Parse(aUri);

    // Copy data to be posted into member variable; iPostData is used later in
    // methods inherited from MHTTPDataSupplier.
    delete iPostData;
    iPostData = aBody.AllocL();

    // Get request method string for HTTP POST
    RStringF method = iSession.StringPool().StringF(HTTP::EPOST,
            RHTTPSession::GetTable());

    // Open transaction with previous method and parsed uri. This class will
    // receive transaction events in MHFRunL and MHFRunError.
    iTransaction = iSession.OpenTransactionL(uri, *this, method);

    // Set headers for request; user agent, accepted content type and body's
    // content type.
    RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
    SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
    SetHeaderL(hdr, HTTP::EAccept, KAccept);
    SetHeaderL(hdr, HTTP::EContentType, aContentType);

    // Set this class as an data supplier. Inherited MHTTPDataSupplier methods
    // are called when framework needs to send body data.
    MHTTPDataSupplier* dataSupplier = this;
    iTransaction.Request().SetBody(*dataSupplier);

    // Submit the transaction. After this the framework will give transaction
    // events via MHFRunL and MHFRunError.
    iTransaction.SubmitL();

    iRunning = ETrue;
    //_LIT(KConnecting,"Connecting..."); iObserver.ClientEvent(KConnecting);
    this->iPublicS60QHttp->changeState(S60QHttp::Connecting);
}

// ----------------------------------------------------------------------------
// S60QHttpPrivate::CancelTransaction()
//
// Cancels currently running transaction and frees resources related to it.
// ----------------------------------------------------------------------------
void S60QHttpPrivate::CancelTransaction()
{
    if(!iRunning)  return;

    //int id = iTransaction.Id();
    // Close() also cancels transaction (Cancel() can also be used but
    // resources allocated by transaction must be still freed with Close())
    iTransaction.Close();

    // Not running anymore
    iRunning = EFalse;
    //_LIT(KTransactionCancelled, "Transaction cancelled");        iObserver.ClientEvent(KTransactionCancelled);

    this->iPublicS60QHttp->setError(-1, S60QHttp::Aborted, "", false);
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::MHFRunL()
//
// Inherited from MHTTPTransactionCallback
// Called by framework to pass transaction events.
// ----------------------------------------------------------------------------
void S60QHttpPrivate::MHFRunL(RHTTPTransaction aTransaction,
                                                  const THTTPEvent& aEvent)
        {
        switch (aEvent.iStatus)
                {
                case THTTPEvent::EGotResponseHeaders:
                     {
                        // HTTP response headers have been received. Use
                        // aTransaction.Response() to get the response. However, it's not
                        // necessary to do anything with the response when this event occurs.

                        // Get HTTP status code from header (e.g. 200)
                        RHTTPResponse resp = aTransaction.Response();
                        TInt status = resp.StatusCode();

                        this->dataSize = resp.Body()->OverallDataSize();

                        // Get status text (e.g. "OK")
                        TBuf<KStatustextBufferSize> statusText;
                        statusText.Copy(resp.StatusText().DesC());

                        /*TBuf<KDefaultBufferSize> text;
                        _LIT(KHeaderReceived, "Header received. Status: %d %S");
                        text.Format(KHeaderReceived, status, &statusText);
                        iObserver.ClientEvent(text);*/

                        QHttpResponseHeader qresp(status, QString((QChar*)statusText.Ptr(), statusText.Length()));

                        this->iPublicS60QHttp->responseHeaderReceived(qresp);
                     }
                     break;

                case THTTPEvent::EGotResponseBodyData:
                        {
                            // Part (or all) of response's body data received. Use
                            // aTransaction.Response().Body()->GetNextDataPart() to get the actual
                            // body data.

                            // Get the body data supplier
                            MHTTPDataSupplier* body = aTransaction.Response().Body();
                            TPtrC8 dataChunk;

                            // GetNextDataPart() returns ETrue, if the received part is the last
                            // one.
                            TBool isLast = body->GetNextDataPart(dataChunk);

                            QByteArray qdatachunk(reinterpret_cast<const char*>(dataChunk.Ptr()),dataChunk.Length());
                            //this->iPublicS60QHttp->append(qdatachunk);
                            if( this->data ) {
                                this->data->append(qdatachunk);
                                //if( this->to ) this->to->write(qdatachunk);

                                //iObserver.ClientBodyReceived(dataChunk);

                                /*TBuf<KInfotextBufferSize> text;
                                _LIT(KBodyPartReceived, "%d bytes received... ");
                                text.Format(KBodyPartReceived, dataChunk.Length());
                                iObserver.ClientEvent(text);*/

                                //emit this->iPublicS60QHttp->dataReadProgress(dataChunk.Length(), (( this->dataSize ) ? this->dataSize : 1) );
                                emit this->iPublicS60QHttp->dataReadProgress(this->data->size(), (( this->dataSize ) ? this->dataSize : 1) );
                            }

                            // NOTE: isLast may not be ETrue even if last data part received.
                            // (e.g. multipart response without content length field)
                            // Use EResponseComplete to reliably determine when body is completely
                            // received.
                            if (isLast)
                            {
                                //_LIT(KBodyReceived,"Body received"); iObserver.ClientEvent(KBodyReceived);
                                //this->iPublicS60QHttp->finished();
                            }

                            // Always remember to release the body data.
                            body->ReleaseData();
                        }
                        break;

                case THTTPEvent::EResponseComplete:
                        {
                        // Indicates that header & body of response is completely received.
                        // No further action here needed.
                        //_LIT(KTransactionComplete, "Transaction Complete"); iObserver.ClientEvent(KTransactionComplete);
                           // this->iPublicS60QHttp->done();
                            //emit this->iPublicS60QHttp->done(false);
                            //this->iPublicS60QHttp->finished(requestId);
                            this->iPublicS60QHttp->finished(iTransaction.Id());

                        }
                        break;

                case THTTPEvent::ESucceeded:
                        {
                            // Indicates that transaction succeeded.
                            //_LIT(KTransactionSuccessful, "Transaction Successful");                        iObserver.ClientEvent(KTransactionSuccessful);

                            // Transaction can be closed now. It's not needed anymore.
                            aTransaction.Close();
                            iRunning = EFalse;
                            if( this->to && this->data ) {
                                if( this->to->write(*this->data, data->size()) != -1 )
                                    emit this->iPublicS60QHttp->done(false);
                                else {
                                    //emit this->iPublicS60QHttp->done(true);
                                    this->iPublicS60QHttp->setError(-1, S60QHttp::UnknownError, "Data size "+QString::number(data->size())+" bytes");
                                }

                               /* if( this->to->write(this->data, data.size()) == -1 ) {
                                    this->iPublicS60QHttp->setError(-1, S60QHttp::UnknownError, "Data size "+QString::number(data.size())+" bytes");
                                } // Altrimenti aspetta il segnale di scrittura ed invia un altro segnale per il done
*/
                                this->to = NULL;
                            } else
                            if( this->data ) {
                                emit this->iPublicS60QHttp->done(false);
                            }
                            //emit this->iPublicS60QHttp->done(false);
                        }
                        break;

                case THTTPEvent::EFailed:
                        {
                            // Transaction completed with failure.
                            //_LIT(KTransactionFailed, "Transaction Failed");                            iObserver.ClientEvent(KTransactionFailed);
                            iRunning = EFalse;
                            // emit this->iPublicS60QHttp->done(true);
                            int id = iTransaction.Id();
                            aTransaction.Close();
                            this->iPublicS60QHttp->setError(id, S60QHttp::UnknownError, "Transaction Failed");
                        }
                        break;

                default:
                        // There are more events in THTTPEvent, but they are not usually
                        // needed. However, event status smaller than zero should be handled
                        // correctly since it's error.
                        {
                            TBuf<KInfotextBufferSize> text;
                            if (aEvent.iStatus < 0)
                            {
                                if( aEvent.iStatus == KErrNotReady ) {
                                    iRunning = EFalse;
                                    iTransaction.Close();
                                    SetupConnectionL();
                                   // iTransaction.SubmitL();
                                    /*_LIT(KErrorStr, "KErrNotReady\nError: %d");
                                    text.Format(KErrorStr, aEvent.iStatus);
                                    this->iPublicS60QHttp->setError(-1, S60QHttp::UnknownError, QString((QChar*)text.Ptr(), text.Length()));*/
                                } else {
                                    _LIT(KErrorStr, "Error: %d");
                                    text.Format(KErrorStr, aEvent.iStatus);
                                    int id = iTransaction.Id();
                                    // Just close the transaction on errors
                                    aTransaction.Close();
                                    iRunning = EFalse;
                                    this->iPublicS60QHttp->setError(id, S60QHttp::UnknownError, QString((QChar*)text.Ptr(), text.Length()));
                                }
                            }
                            else
                            {
                                // Other events are not errors (e.g. permanent and temporary
                                // redirections)
                                _LIT(KUnrecognisedEvent, "Unrecognised event: %d");
                                text.Format(KUnrecognisedEvent, aEvent.iStatus);
                                this->iPublicS60QHttp->setError(iTransaction.Id(), S60QHttp::UnknownError, QString((QChar*)text.Ptr(), text.Length()));
                            }
                            // iObserver.ClientEvent(text);
                        }
                        break;
                }
        }


// ----------------------------------------------------------------------------
// S60QHttpPrivate::MHFRunError()
//
// Inherited from MHTTPTransactionCallback
// Called by framework when *leave* occurs in handling of transaction event.
// These errors must be handled, or otherwise HTTP-CORE 6 panic is thrown.
// ----------------------------------------------------------------------------
TInt S60QHttpPrivate::MHFRunError(TInt aError,
                                                          RHTTPTransaction /*aTransaction*/,
                                                          const THTTPEvent& /*aEvent*/)
{
    // Just notify about the error and return KErrNone.
    TBuf<KInfotextBufferSize>	text;
    _LIT(KRunError, "MHFRunError: %d");
    text.Format(KRunError, aError);
    //iObserver.ClientEvent(text);

    this->iPublicS60QHttp->setError(iTransaction.Id(), S60QHttp::UnknownError, QString((QChar*)text.Ptr(), text.Length()));

    return KErrNone;
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::GetNextDataPart()
//
// Inherited from MHTTPDataSupplier
// Called by framework when next part of the body is needed. In this
// this provides data for HTTP post.
// ----------------------------------------------------------------------------
TBool S60QHttpPrivate::GetNextDataPart(TPtrC8& aDataPart)
{
    if(iPostData)
            {
            // Provide pointer to next chunk of data (return ETrue, if last chunk)
            // Usually only one chunk is needed, but sending big file could require
            // loading the file in small parts.
            aDataPart.Set(iPostData->Des());
            }
    return ETrue;
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::ReleaseData()
//
// Inherited from MHTTPDataSupplier
// Called by framework. Allows us to release resources needed for previous
// chunk. (e.g. free buffers)
// ----------------------------------------------------------------------------
void S60QHttpPrivate::ReleaseData()
{
    // It's safe to delete iPostData now.
    delete iPostData;
    iPostData = NULL;
}

// ----------------------------------------------------------------------------
// S60QHttpPrivate::Reset()
//
// Inherited from MHTTPDataSupplier
// Called by framework to reset the data supplier. Indicates to the data
// supplier that it should return to the first part of the data.
// In practise an error has occured while sending data, and framework needs to
// resend data.
// ----------------------------------------------------------------------------
TInt S60QHttpPrivate::Reset()
{
    // Nothing needed since iPostData still exists and contains all the data.
    // (If a file is used and read in small parts we should seek to beginning
    // of file and provide the first chunk again in GetNextDataPart() )
    return KErrNone;
}


// ----------------------------------------------------------------------------
// S60QHttpPrivate::OverallDataSize()
//
// Inherited from MHTTPDataSupplier
// Called by framework. We should return the expected size of data to be sent.
// If it's not know we can return KErrNotFound (it's allowed and does not cause
// problems, since HTTP protocol allows multipart bodys without exact content
// length in header).
// ----------------------------------------------------------------------------
TInt S60QHttpPrivate::OverallDataSize()
{
    if(iPostData)
            return iPostData->Length();
    else
            return KErrNotFound ;
}

// ----------------------------------------------------------------------------
// S60QHttpPrivate::GetCredentialsL()
//
// Inherited from MHTTPAuthenticationCallback
// Called by framework when we requested authenticated page and framework
// needs to know username and password.
// ----------------------------------------------------------------------------
TBool S60QHttpPrivate::GetCredentialsL(const TUriC8& aURI,
                                                                   RString aRealm,
                                                                   RStringF aAuthenticationType,
                                                                   RString& aUsername,
                                                                   RString& aPassword)
        {
        // aURI, aReal and aAuthenticationType are informational only. We only need
        // to set aUsername and aPassword and return ETrue, if aUsername and
        // aPassword are provided by user.

        // Informational only
        TBuf<KURIBufferSize> authType;
        TBuf<KURIBufferSize> uri;
        TBuf<KDefaultBufferSize> authText;
        authType.Copy(aAuthenticationType.DesC());
        uri.Copy(aURI.UriDes());
        _LIT(KAuthRequired, "%S requires %S authentication.");
        authText.Format(KAuthRequired, &uri, &authType);
        _LIT(KAuthNote, "Authentication required.");
        CEikonEnv::Static()->InfoWinL(KAuthNote, authText);

        // Query user name and password
        TBuf<KDefaultBufferSize> userName;
        TBuf<KDefaultBufferSize> password;
        //CAknMultiLineDataQueryDialog* dlg = CAknMultiLineDataQueryDialog::NewL(userName, password);

        /*if (!dlg->ExecuteLD(R_DIALOG_USER_PASSWORD_QUERY))
                return EFalse; // No credentials given; must return EFalse
*/
        // Set aUsername and aPassword
        TBuf8<KDefaultBufferSize> temp;
        temp.Copy(userName);
        TRAPD(err, aUsername = aRealm.Pool().OpenStringL(temp));
        if (!err)
                {
                temp.Copy(password);
                TRAP(err, aPassword = aRealm.Pool().OpenStringL(temp));
                if (!err) return ETrue;
                }

        // Return ETrue if user has given credentials (username and password),
        // otherwise EFlase
        return EFalse;
        }

// ----------------------------------------------------------------------------
// S60QHttpPrivate::SetupConnectionL()
//
// The method set the internet access point and connection setups.
// ----------------------------------------------------------------------------
void S60QHttpPrivate::SetupConnectionL()
{
    if( iConnectionSetupDone )    return;

    //iConnectionSetupDone = ETrue;
    iConnectionSetupDone = true;

    //open socket server and start the connection
    User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iConnection.Open(iSocketServ));

    // open the IAP communications database
    CCommsDatabase* commDB = CCommsDatabase::NewL(EDatabaseTypeIAP);
    CleanupStack::PushL(commDB);

    // initialize a view
    CCommsDbConnectionPrefTableView* commDBView =
    commDB->OpenConnectionPrefTableInRankOrderLC(ECommDbConnectionDirectionUnknown);

    // go to the first record
    User::LeaveIfError(commDBView->GotoFirstRecord());

    // Declare a prefTableView Object.
    CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;

    // read the connection preferences
    commDBView->ReadConnectionPreferenceL(pref);
    TUint32 iapID = pref.iBearer.iIapId;

    // pop and destroy the IAP View
    CleanupStack::PopAndDestroy(commDBView);

    // pop and destroy the database object
    CleanupStack::PopAndDestroy(commDB);

    // Now we have the iap Id. Use it to connect for the connection.
    // Create a connection preference variable.
    TCommDbConnPref connectPref;

    // setup preferences
    //connectPref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
    connectPref.SetDialogPreference(ECommDbDialogPrefPrompt);
    connectPref.SetDirection(ECommDbConnectionDirectionUnknown);
    connectPref.SetBearerSet(ECommDbBearerWLAN | ECommDbBearerGPRS);
    //Sets the CommDb ID of the IAP to use for this connection
    connectPref.SetIapId(iapID);

    User::LeaveIfError(iConnection.Start(connectPref));

    //set the sessions connection info
    RStringPool strPool = iSession.StringPool();
    RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();

    //to use our socket server and connection
    connInfo.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketServ,
    RHTTPSession::GetTable() ), THTTPHdrVal (iSocketServ.Handle()) );

    connInfo.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketConnection,
    RHTTPSession::GetTable() ),
    THTTPHdrVal (REINTERPRET_CAST(TInt, &(iConnection))) );

    emit this->iPublicS60QHttp->connectionSetup(true);
}

