
#include "HttpConnection.h"
#include "Request.h"
#include "HttpLog.h"
#include "ext/frameworkext.h"

using Elastos::Net::ISocket;
using Org::Apache::Http::Params::IBasicHttpParams;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(HttpConnection, IConnection);

ECode HttpConnection::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpConnection::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpConnection::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode HttpConnection::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IConnection * o = IConnection::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IConnection));
    }
    return NOERROR;
}

ECode HttpConnection::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IConnection);
    return NOERROR;
}

ECode HttpConnection::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("HttpConnection:(");
    sb += (Int32)THIS_PROBE(IConnection);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

HttpConnection::HttpConnection(
    /* [in] */ IContext* context,
    /* [in] */ IHttpHost* host,
    /* [in] */ RequestFeeder* requestFeeder)
    : Connection(context, host, requestFeeder)
{}

ECode HttpConnection::OpenConnection(
    /* [in] */ Request* req,
    /* [out] */ IAndroidHttpClientConnection** connection)
{
    // Update the certificate info (connection not secure - set to null)
    AutoPtr<IEventHandler> eventHandler = req->GetEventHandler();
    mCertificate = NULL;
    eventHandler->Certificate(mCertificate);

    AutoPtr<IAndroidHttpClientConnection> conn;
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
}

ECode HttpConnection::CloseConnection()
{
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
}

ECode HttpConnection::RestartConnection(
    /* [in] */ Boolean abort)
{
    // not required for plain http connections
    return NOERROR;
}

ECode HttpConnection::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = String("http");
    return NOERROR;
}

ECode HttpConnection::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}


}
}
}
}
