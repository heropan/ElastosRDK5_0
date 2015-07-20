
#include "HttpsConnection.h"
#include "Request.h"
#include "HttpLog.h"
#include <elastos/utility/logging/Logger.h>

using namespace Org::Apache::Harmony::Xnet::Provider::Jsse;
using namespace Org::Apache::Http::Params;

using Elastos::Utility::Logging::Logger;
using Elastos::Net::ISocket;
using Org::Apache::Http::Message::IBasicHttpRequest;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {


AutoPtr<ISSLSocketFactory> HttpsConnection::mSslSocketFactory;
ECode HttpsConnection::InitializeEngine(
        /* [in] */ IFile* sessionDir)
{
    AutoPtr<ISSLClientSessionCache> cache;
    if (sessionDir != NULL) {
        String sSessionDir;
        // sessionDir->ToString(&sSessionDir);
        Logger::D(String("HttpsConnection"), String("Caching SSL sessions in ")
                + sSessionDir + String("."));
                AutoPtr<IFileClientSessionCacheHelper> helper;

        // TODO:
        // CFileClientSessionCacheHelper::AcquireSingleton((IFileClientSessionCacheHelper**)&helper);
        // FAIL_RETURN(helper->UsingDirectory(sessionDir, &cache));
    }

    AutoPtr<IOpenSSLContextImpl> sslContext;
    // COpenSSLContextImpl::New((IOpenSSLContextImpl**)&sslContext);

    // here, trust managers is a single trust-all manager
    AutoPtr<ArrayOf<ITrustManager*> > trustManagers = ArrayOf<ITrustManager*>::Alloc(1);
    AutoPtr<ITrustManager> manager;
    LocalCX509TrustManager* cmanager = new LocalCX509TrustManager();
    manager = (ITrustManager*)cmanager->Probe(Elastos::Net::Ssl::EIID_ITrustManager);
    (*trustManagers)[0] = manager;

    // FAIL_RETURN(sslContext->EngineInit(NULL, trustManagers, NULL));

    AutoPtr<IClientSessionContext> context;
    // FAIL_RETURN(sslContext->EngineGetClientSessionContext((IClientSessionContext**)&context));
    // FAIL_RETURN(context->SetPersistentCache(cache));

    // {
    //     Object mLock;
    //     AutoLock lock(mLock);
    //     FAIL_RETURN(sslContext->EngineGetSocketFactory((ISSLSocketFactory**)&mSslSocketFactory));
    // }

    return NOERROR;
}

CAR_INTERFACE_IMPL(HttpsConnection::LocalCX509TrustManager, IX509TrustManager);

ECode HttpsConnection::LocalCX509TrustManager::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpsConnection::LocalCX509TrustManager::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpsConnection::LocalCX509TrustManager::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpsConnection::LocalCX509TrustManager::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IHttpsConnection * o = IHttpsConnection::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IHttpsConnection));
    }
    return NOERROR;
}

ECode HttpsConnection::LocalCX509TrustManager::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IHttpsConnection);
    return NOERROR;
}

ECode HttpsConnection::LocalCX509TrustManager::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("LocalCX509TrustManager:(");
    sb += (Int32)THIS_PROBE(IHttpsConnection);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

ECode HttpsConnection::LocalCX509TrustManager::GetAcceptedIssuers(
    /* [out, callee] */ ArrayOf<IX509Certificate>** certs)
{
    return NOERROR;
}

ECode HttpsConnection::LocalCX509TrustManager::CheckClientTrusted(
    /* [in] */ ArrayOf<IX509Certificate>* certs,
    /* [in] */ const String& authType)
{
    return NOERROR;
}

ECode HttpsConnection::LocalCX509TrustManager::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate>* certs,
    /* [in] */ const String& authType)
{
    return NOERROR;
}

CAR_INTERFACE_IMPL(HttpsConnection, IHttpsConnection);

ECode HttpsConnection::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpsConnection::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpsConnection::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpsConnection::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IHttpsConnection * o = IHttpsConnection::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IHttpsConnection));
    }
    return NOERROR;
}

ECode HttpsConnection::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IHttpsConnection);
    return NOERROR;
}

ECode HttpsConnection::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("HttpsConnection:(");
    sb += (Int32)THIS_PROBE(IHttpsConnection);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

HttpsConnection::HttpsConnection(
    /* [in] */ IContext* context,
    /* [in] */ IHttpHost* host,
    /* [in] */ IHttpHost* proxy,
    /* [in] */ RequestFeeder* requestFeeder)
    : Connection(context, host, requestFeeder)
    , mProxyHost(proxy)
    , mSuspended(FALSE)
    , mAborted(FALSE)
{}

HttpsConnection::~HttpsConnection()
{}

ISSLSocketFactory* HttpsConnection::GetSocketFactory()
{
    return mSslSocketFactory;
}

ECode HttpsConnection::SetCertificate(
        /* [in] */ ISslCertificate* certificate)
{
    mCertificate = certificate;
    return NOERROR;
}

ECode HttpsConnection::OpenConnection(
    /* [in] */ Request* req,
    /* [out] */ IAndroidHttpClientConnection** connection)
{
    AutoPtr<ISSLSocket> sslSock;

    if (mProxyHost != NULL) {
        // If we have a proxy set, we first send a CONNECT request
        // to the proxy; if the proxy returns 200 OK, we negotiate
        // a secure connection to the target server via the proxy.
        // If the request fails, we drop it, but provide the event
        // handler with the response status and headers. The event
        // handler is then responsible for cancelling the load or
        // issueing a new request.
        AutoPtr<IAndroidHttpClientConnection> proxyConnection;
        AutoPtr<ISocket> proxySock;

        String name;
        Int32 port;
        // TODO:
        // mProxyHost->GetHostName(&name);
        // mProxyHost->GetPort(&port);
        // CSocket::New(name, port, (ISocket**)&proxySock);

        ECode ec;
        // ec = proxySock->SetSoTimeout(60 * 1000);
        FAIL_RETURN(CheckErrorAndClose(ec, proxyConnection));

        CAndroidHttpClientConnection::New((IAndroidHttpClientConnection**)&proxyConnection);
        AutoPtr<IHttpParams> params;
        // CBasicHttpParams::New((IHttpParams**)&params);
        AutoPtr<IHttpConnectionParamsHelper> helper;
        // CHttpConnectionParamsHelper::AcquireSingleton((IHttpConnectionParamsHelper**)&helper);
        // ec = helper->SetSocketBufferSize(params, 8192);
        FAIL_RETURN(CheckErrorAndClose(ec, proxyConnection));

        // ec = proxyConnection->Bind(proxySock, params);
        FAIL_RETURN(CheckErrorAndClose(ec, proxyConnection));

        AutoPtr<IStatusLine> statusLine;
        Int32 statusCode = 0;
        AutoPtr<IHeaders> headers;
        CHeaders::New((IHeaders**)&headers);

        AutoPtr<IBasicHttpRequest> proxyReq;
        String shost;
        // mHost->ToHostString(&shost);
        // CBasicHttpRequest::New(String("CONNECT"), shost, &proxyReq);

        // add all 'proxy' headers from the original request, we also need
        // to add 'host' header unless we want proxy to answer us with a
        // 400 Bad Request
        AutoPtr<ArrayOf<IHeader*> > reqHeaders;
        // req->mHttpRequest->GetAllHeaders((ArrayOf<IHeader*>**)&reqHeaders);
        if (reqHeaders != NULL) {
            for (Int32 i = 0; i < reqHeaders->GetLength(); ++i) {
                AutoPtr<IHeader> h;
                h = (*reqHeaders)[i];
                String headerName;
                // h->GetName(&headerName);
                // h->ToLowerCase();

                if (headerName.StartWith("proxy")
                        || headerName.Equals("keep-alive")
                        || headerName.Equals("host")) {
                    // proxyReq->AddHeader(h);
                }
            }
        }

        AutoPtr<IHttpRequest> httpReq = (IHttpRequest*)proxyReq->Probe(EIID_IHttpRequest);
        ec = proxyConnection->SendRequestHeader(httpReq);
        // if (ec == E_PARSE_EXCEPTION
        //         || ec == E_HTTP_EXCEPTION
        //         || ec == E_IO_EXCEPTION ) {
        //     Logger::E(TAG, String("failed to send a CONNECT request"));
        //     return ec;
        // }
        ec = proxyConnection->Flush();
        // if (ec == E_PARSE_EXCEPTION
        //         || ec == E_HTTP_EXCEPTION
        //         || ec == E_IO_EXCEPTION ) {
        //     Logger::E(TAG, String("failed to send a CONNECT request"));
        //     return ec;
        // }

        // it is possible to receive informational status
        // codes prior to receiving actual headers;
        // all those status codes are smaller than OK 200
        // a loop is a standard way of dealing with them
        do {
            statusLine = NULL;
            ec = proxyConnection->ParseResponseHeader(headers, (IStatusLine**)&statusLine);
            // if (ec == E_PARSE_EXCEPTION
            //         || ec == E_HTTP_EXCEPTION
            //         || ec == E_IO_EXCEPTION ) {
            //     Logger::E(TAG, String("failed to send a CONNECT request"));
            //     return ec;
            // }
            // statusLine->GetStatusCode(&statusCode);
        } while (statusCode < IHttpStatus::SC_OK);


        if (statusCode == IHttpStatus::SC_OK) {
            String hName;
            Int32 hPort;
            // mHost->GetHostName(&hName);
            // mHost->GetPort(&hName);
            // ec = GetSocketFactory()->CreateSocket(proxySock, hName, hName, TRUE, &sslSock);
            if (FAILED(ec)) {
                if (sslSock != NULL) {
                    // sslSock->Close();
                }

                Logger::E("HttpsConnection", String("failed to create an SSL socket"));
            }
        } else {
            // if the code is not OK, inform the event handler
            AutoPtr<IProtocolVersion> version;
            // statusLine->GetProtocolVersion((IProtocolVersion**)&version);

            Int32 major;
            Int32 minor;
            // version->HetMajor(&major);
            // version->HetMinor(&minor);
            String phrase;
            // statusLine->GetReasonPhrase(&phrase);
            req->mEventHandler->Status(major, minor, statusCode, phrase);
            req->mEventHandler->Headers(headers);
            req->mEventHandler->EndData();

            // proxyConnection->Close();

            // here, we return null to indicate that the original
            // request needs to be dropped
            *connection = NULL;
            return NOERROR;
        }
    } else {
        // if we do not have a proxy, we simply connect to the host
        String hName;
        Int32 hPort;
        // mHost->GetHostName(&hName);
        // mHost->GetPort(&hName);
        ECode ec;
        // ec = GetSocketFactory()->CreateSocket(proxySock, hName, hName, TRUE, &sslSock);
        if (FAILED(ec)) {
            if (sslSock != NULL) {
                // sslSock->Close();
            }

            Logger::E("HttpsConnection", String("failed to create an SSL socket"));
        }
        // ec = sslSock->SetSoTimeout(SOCKET_TIMEOUT);
    }

    // do handshake and validate server certificates
    AutoPtr<ICertificateChainValidatorHelper> helper;
    CCertificateChainValidatorHelper::AcquireSingleton((ICertificateChainValidatorHelper**)&helper);
    AutoPtr<ICertificateChainValidator> validator;
    helper->GetInstance((ICertificateChainValidator**)&validator);
    String hName;
    // mHost->GetHostName(&hName);
    AutoPtr<ISslError> error;
    AutoPtr<IHttpsConnection> iThis = (IHttpsConnection*)this->Probe(EIID_IHttpsConnection);
    validator->DoHandshakeAndValidateServerCertificates(iThis, sslSock, hName, (ISslError**)&error);

    // Inform the user if there is a problem
    if (error != NULL) {
        // handleSslErrorRequest may immediately unsuspend if it wants to
        // allow the certificate anyway.
        // So we mark the connection as suspended, call handleSslErrorRequest
        // then check if we're still suspended and only wait if we actually
        // need to.
        {
            AutoLock lock(mSuspendLock);
            mSuspended = true;
        }
        // don't hold the lock while calling out to the event handler
        Boolean canHandle;
        AutoPtr<IEventHandler> handler = req->GetEventHandler();
        handler->HandleSslErrorRequest(error, &canHandle);
        if(!canHandle) {
            String serr;
            error->ToString(&serr);
            Logger::E("HttpsConnection", String("failed to handle ") + serr);
            return E_IO_EXCEPTION;
        }

        {
            AutoLock lock(mSuspendLock);
            if (mSuspended) {
                // Put a limit on how long we are waiting; if the timeout
                // expires (which should never happen unless you choose
                // to ignore the SSL error dialog for a very long time),
                // we wake up the thread and abort the request. This is
                // to prevent us from stalling the network if things go
                // very bad.
                // TODO:
                // mSuspendLock.Wait(10 * 60 * 1000);
                if (mSuspended) {
                    // mSuspended is true if we have not had a chance to
                    // restart the connection yet (ie, the wait timeout
                    // has expired)
                    mSuspended = FALSE;
                    mAborted = TRUE;
                    if (HttpLog::LOGV) {
                        HttpLog::V(String("HttpsConnection.openConnection(): SSL timeout expired and request was cancelled!!!"));
                    }
                }
            }
            if (mAborted) {
                // The user decided not to use this unverified connection
                // so close it immediately.
                // sslSock->Close();
                Logger::E("HttpsConnection", String("connection closed by the user"));
                // return E_SSL_CONNECTION_CLOSE_BY_USER_EXCEPTION;
                return E_RUNTIME_EXCEPTION;
            }
        }
    }

    // All went well, we have an open, verified connection.
    AutoPtr<IAndroidHttpClientConnection> conn;
    CAndroidHttpClientConnection::New((IAndroidHttpClientConnection**)&conn);
    AutoPtr<IBasicHttpParams> params;
    // CBasicHttpParams::New((IBasicHttpParams**)&params);
    // params->SetIntParameter(IHttpConnectionParams::SOCKET_BUFFER_SIZE, 8192);
    AutoPtr<ISocket> sslBaseSock = (ISocket*)sslSock->Probe(Elastos::Net::EIID_ISocket);
    AutoPtr<IHttpParams> baseParams = (IHttpParams*)sslSock->Probe(Org::Apache::Http::Params::EIID_IHttpParams);
    conn->Bind(sslBaseSock, baseParams);

    *connection = conn;
    REFCOUNT_ADD(*connection);
    return NOERROR;
}

ECode HttpsConnection::CloseConnection()
{
    // if the connection has been suspended due to an SSL error
    if (mSuspended) {
        // wake up the network thread
        RestartConnection(FALSE);
    }

    ECode ec;
    Boolean isOpen;
    // TODO:
    if (mHttpClientConnection != NULL &&/* (mHttpClientConnection->IsOpen(&isOpen), */isOpen) {
        // ec = mHttpClientConnection->Close();
    }
    if (FAILED(ec)) {
        if (HttpLog::LOGV) {
            String smHost;
            // mHost->ToString(&smHost);
            HttpLog::V(String("HttpsConnection.closeConnection(): failed closing connection ") + smHost);
        }
    }

    return ec;
}

ECode HttpsConnection::RestartConnection(
    /* [in] */ Boolean proceed)
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("HttpsConnection.restartConnection(): proceed: ") + StringUtils::BooleanToString(proceed));
    }

    AutoLock lock(mSuspendLock);

    if (mSuspended) {
        mSuspended = FALSE;
        mAborted = !proceed;
        // TODO:
        // mSuspendLock->Notify();
    }

    return NOERROR;
}

ECode HttpsConnection::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = String("https");
    return NOERROR;
}

ECode HttpsConnection::CheckErrorAndClose(
        /* [in] */ ECode ec,
        /* [in] */ IAndroidHttpClientConnection* connection)
{
    if (FAILED(ec))    {
        if (connection != NULL) {
            // TODO:
            // connection->Close();
        }
        Logger::E("HttpsConnection", "failed to establish a connection to the proxy");
    }

    return ec;
}

}
}
}
}
