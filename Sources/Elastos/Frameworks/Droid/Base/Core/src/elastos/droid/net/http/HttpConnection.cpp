
#include "elastos/droid/net/http/HttpConnection.h"

using Elastos::Net::ISocket;
using Org::Apache::Http::Params::IBasicHttpParams;
using Org::Apache::Http::Params::IHttpConnectionParams;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

HttpConnection::HttpConnection()
{}

ECode HttpConnection::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHttpHost* host,
    /* [in] */ IRequestFeeder* requestFeeder)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(context, host, requestFeeder);
#endif
}

ECode HttpConnection::OpenConnection(
    /* [in] */ IRequest* req,
    /* [out] */ IElastosHttpClientConnection** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    // Update the certificate info (connection not secure - set to null)
    AutoPtr<IEventHandler> eventHandler = req->GetEventHandler();
    mCertificate = NULL;
    eventHandler->Certificate(mCertificate);

    AutoPtr<IElastosHttpClientConnection> conn;
    // TODO:
    // CCAndroidHttpClientConnection::New((IAndroidHttpClientConnection**)&conn);
    AutoPtr<IBasicHttpParams> params;
    // CBasicHttpParams::New((IBasicHttpParams**)&params);
    AutoPtr<ISocket> sock;
    String hostName;
    Int32 port;
    // mHost->GetHostName(&hostName);
    // mHost->GetPort(&port);
    // CSocket::New(hostName, port);
    // params->SetIntParameter(IHttpConnectionParams::SOCKET_BUFFER_SIZE, 8192);
    // conn->Bind(sock, params);

    *connection = conn;
    REFCOUNT_ADD(*connection);
    return NOERROR;
#endif
}

ECode HttpConnection::CloseConnection()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    ECode ec = NOERROR;
    // TODO:
    if (mHttpClientConnection != NULL/* && mHttpClientConnection->IsOpen()*/) {
        // ec = mHttpClientConnection->Close();
        if (FAILED(ec)) {
            if (HttpLog::LOGV) {
                String shost;
                // mHost->ToString(&shost);
                HttpLog::V(String("closeConnection(): failed closing connection ") + shost);
            }
        }
    }

    return NOERROR;
#endif
}

ECode HttpConnection::RestartConnection(
    /* [in] */ Boolean abort)
{
    // not required for plain http connections
    return NOERROR;
}

ECode HttpConnection::GetScheme(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(scheme);
    *scheme = String("http");
    return NOERROR;
#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
