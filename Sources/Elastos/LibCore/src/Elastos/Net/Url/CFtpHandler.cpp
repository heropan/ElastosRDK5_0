
#include "CFtpHandler.h"

namespace Elastos {
namespace Net {
namespace Url {

ECode CFtpHandler::constructor()
{
    return NOERROR;
}

ECode CFtpHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [out] */ IURLConnection** urlConnection)
{
    return FtpHandler::OpenConnection(u, urlConnection);
}

ECode CFtpHandler::OpenConnectionEx(
    /* [in] */ IURL* u,
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** urlConnection)
{
    return FtpHandler::OpenConnectionEx(u, proxy, urlConnection);
}

ECode CFtpHandler::ParseURL(
    /* [in] */ IURL* url,
    /* [in] */ const String& spec,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return FtpHandler::ParseURL(url, spec, start, end);
}

ECode CFtpHandler::SetURL(
    /* [in] */ IURL* u,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ const String& ref)
{
    return FtpHandler::SetURL(u, protocol, host, port, file, ref);
}

ECode CFtpHandler::SetURLEx(
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
    return FtpHandler::SetURLEx(u, protocol, host, port, authority, userInfo, file, query, ref);
}

ECode CFtpHandler::ToExternalFormEx(
    /* [in] */ IURL* url,
    /* [in] */ Boolean escapeIllegalCharacters,
    /* [out] */ String* s)
{
    return FtpHandler::ToExternalFormEx(url, escapeIllegalCharacters, s);
}

ECode CFtpHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [out] */ String* s)
{
    return FtpHandler::ToExternalForm(url, s);
}

ECode CFtpHandler::EqualsEx(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isEquals)
{
    return FtpHandler::EqualsEx(url1, url2, isEquals);
}

ECode CFtpHandler::GetDefaultPort(
    /* [out] */ Int32* portNum)
{
    return FtpHandler::GetDefaultPort(portNum);
}

ECode CFtpHandler::GetHostAddress(
    /* [in] */ IURL* url,
    /* [out] */ IInetAddress** hostAddress)
{
    return FtpHandler::GetHostAddress(url, hostAddress);
}

ECode CFtpHandler::GetHashCode(
    /* [in] */ IURL* url,
    /* [out] */ Int32* code)
{
    return FtpHandler::GetHashCode(url, code);
}

ECode CFtpHandler::HostsEqual(
    /* [in] */ IURL* a,
    /* [in] */ IURL* b,
    /* [out] */ Boolean* isEqual)
{
    return FtpHandler::HostsEqual(a, b, isEqual);
}

ECode CFtpHandler::SameFile(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isSame)
{
    return FtpHandler::SameFile(url1, url2, isSame);
}

} // namespace Url
} // namespace Net
} // namespace Elastos
