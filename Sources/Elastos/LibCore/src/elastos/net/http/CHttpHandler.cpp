
#include "CHttpHandler.h"
#include "HttpURLConnectionImpl.h"

namespace Elastos {
namespace Net {
namespace Http {

ECode CHttpHandler::constructor()
{
    return NOERROR;
}

ECode CHttpHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [out] */ IURLConnection** urlConnection)
{
    VALIDATE_NOT_NULL(urlConnection)

    Int32 portnum = 0;
    GetDefaultPort(&portnum);
    AutoPtr<IHttpURLConnectionImpl> outres = (IHttpURLConnectionImpl*) new HttpURLConnectionImpl(u, portnum);
    *urlConnection = IURLConnection::Probe(outres);
    REFCOUNT_ADD(*urlConnection);
    return NOERROR;
}

ECode CHttpHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** urlConnection)
{
    VALIDATE_NOT_NULL(urlConnection)

    if (u == NULL || proxy == NULL) {
        // throw new IllegalArgumentException("url == null || proxy == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 portnum = 0;
    GetDefaultPort(&portnum);
    AutoPtr<IHttpURLConnectionImpl> outres = (IHttpURLConnectionImpl*) new HttpURLConnectionImpl(u, portnum, proxy);
    *urlConnection = IURLConnection::Probe(outres);
    REFCOUNT_ADD(*urlConnection);
    return NOERROR;
}

ECode CHttpHandler::ParseURL(
    /* [in] */ IURL* url,
    /* [in] */ const String& spec,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return URLStreamHandler::ParseURL(url, spec, start, end);
}

ECode CHttpHandler::SetURL(
    /* [in] */ IURL* u,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ const String& ref)
{
    return URLStreamHandler::SetURL(u, protocol, host, port, file, ref);
}

ECode CHttpHandler::SetURL(
    /* [in] */ IURL* u,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& authority,
    /* [in] */ const String& userInfo,
    /* [in] */ const String& file,
    /* [in] */ const String& query,
    /* [in] */ const String& ref)
{
    return URLStreamHandler::SetURL(u, protocol, host, port, authority, userInfo, file, query, ref);
}

ECode CHttpHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [in] */ Boolean escapeIllegalCharacters,
    /* [out] */ String* s)
{
    return URLStreamHandler::ToExternalForm(url, escapeIllegalCharacters, s);
}

ECode CHttpHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [out] */ String* s)
{
    return URLStreamHandler::ToExternalForm(url, s);
}

ECode CHttpHandler::Equals(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isEquals)
{
    return URLStreamHandler::Equals(url1, url2, isEquals);
}

ECode CHttpHandler::GetDefaultPort(
    /* [out] */ Int32* portNum)
{
    VALIDATE_NOT_NULL(portNum)

    *portNum = 80;
    return NOERROR;
}

ECode CHttpHandler::GetHostAddress(
    /* [in] */ IURL* url,
    /* [out] */ IInetAddress** hostAddress)
{
    return URLStreamHandler::GetHostAddress(url, hostAddress);
}

ECode CHttpHandler::GetHashCode(
    /* [in] */ IURL* url,
    /* [out] */ Int32* code)
{
    return URLStreamHandler::GetHashCode(url, code);
}

ECode CHttpHandler::HostsEqual(
    /* [in] */ IURL* a,
    /* [in] */ IURL* b,
    /* [out] */ Boolean* isEqual)
{
    return URLStreamHandler::HostsEqual(a, b, isEqual);
}

ECode CHttpHandler::SameFile(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isSame)
{
    return URLStreamHandler::SameFile(url1, url2, isSame);
}

} // namespace Http
} // namespace Net
} // namespace Elastos
