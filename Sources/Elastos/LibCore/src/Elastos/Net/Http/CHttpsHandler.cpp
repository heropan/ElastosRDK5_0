
#include "cmdef.h"
#include "CHttpsHandler.h"

namespace Elastos {
namespace Net {
namespace Http {

ECode CHttpsHandler::constructor()
{
    return NOERROR;
}

ECode CHttpsHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [out] */ IURLConnection** urlConnection)
{
    assert(0 && "TODO");
    // return new HttpsURLConnectionImpl(url, getDefaultPort());
    return NOERROR;
}

ECode CHttpsHandler::OpenConnectionEx(
    /* [in] */ IURL* u,
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** urlConnection)
{
    if (u == NULL || proxy == NULL) {
        // throw new IllegalArgumentException("url == null || proxy == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    assert(0 && "TODO");
    // return new HttpsURLConnectionImpl(url, getDefaultPort(), proxy);
    return NOERROR;
}

ECode CHttpsHandler::ParseURL(
    /* [in] */ IURL* url,
    /* [in] */ const String& spec,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return URLStreamHandler::ParseURL(url, spec, start, end);
}

ECode CHttpsHandler::SetURL(
    /* [in] */ IURL* u,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ const String& ref)
{
    return URLStreamHandler::SetURL(u, protocol, host, port, file, ref);
}

ECode CHttpsHandler::SetURLEx(
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
    return URLStreamHandler::SetURLEx(u, protocol, host, port, authority, userInfo, file, query, ref);
}

ECode CHttpsHandler::ToExternalFormEx(
    /* [in] */ IURL* url,
    /* [in] */ Boolean escapeIllegalCharacters,
    /* [out] */ String* s)
{
    return URLStreamHandler::ToExternalFormEx(url, escapeIllegalCharacters, s);
}

ECode CHttpsHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [out] */ String* s)
{
    return URLStreamHandler::ToExternalForm(url, s);
}

ECode CHttpsHandler::EqualsEx(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isEquals)
{
    return URLStreamHandler::EqualsEx(url1, url2, isEquals);
}

ECode CHttpsHandler::GetDefaultPort(
    /* [out] */ Int32* portNum)
{
    VALIDATE_NOT_NULL(portNum)

    *portNum = 443;
    return NOERROR;
}

ECode CHttpsHandler::GetHostAddress(
    /* [in] */ IURL* url,
    /* [out] */ IInetAddress** hostAddress)
{
    return URLStreamHandler::GetHostAddress(url, hostAddress);
}

ECode CHttpsHandler::GetHashCode(
    /* [in] */ IURL* url,
    /* [out] */ Int32* code)
{
    return URLStreamHandler::GetHashCode(url, code);
}

ECode CHttpsHandler::HostsEqual(
    /* [in] */ IURL* a,
    /* [in] */ IURL* b,
    /* [out] */ Boolean* isEqual)
{
    return URLStreamHandler::HostsEqual(a, b, isEqual);
}

ECode CHttpsHandler::SameFile(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isSame)
{
    return URLStreamHandler::SameFile(url1, url2, isSame);
}

} // namespace Http
} // namespace Net
} // namespace Elastos
