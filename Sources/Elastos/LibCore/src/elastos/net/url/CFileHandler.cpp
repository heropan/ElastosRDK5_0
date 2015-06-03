
#include "CFileHandler.h"

namespace Elastos {
namespace Net {
namespace Url {

ECode CFileHandler::constructor()
{
    return NOERROR;
}

ECode CFileHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [out] */ IURLConnection** urlConnection)
{
    return FileHandler::OpenConnection(u, urlConnection);
}

ECode CFileHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** urlConnection)
{
    return FileHandler::OpenConnection(u, proxy, urlConnection);
}

ECode CFileHandler::ParseURL(
    /* [in] */ IURL* url,
    /* [in] */ const String& spec,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return FileHandler::ParseURL(url, spec, start, end);
}

ECode CFileHandler::SetURL(
    /* [in] */ IURL* u,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ const String& ref)
{
    return FileHandler::SetURL(u, protocol, host, port, file, ref);
}

ECode CFileHandler::SetURL(
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
    return FileHandler::SetURL(u, protocol, host, port, authority, userInfo, file, query, ref);
}

ECode CFileHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [in] */ Boolean escapeIllegalCharacters,
    /* [out] */ String* s)
{
    return FileHandler::ToExternalForm(url,escapeIllegalCharacters, s);
}

ECode CFileHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [out] */ String* s)
{
    return FileHandler::ToExternalForm(url, s);
}

ECode CFileHandler::Equals(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isEquals)
{
    return FileHandler::Equals(url1, url2, isEquals);
}

ECode CFileHandler::GetDefaultPort(
    /* [out] */ Int32* portNum)
{
    return FileHandler::GetDefaultPort(portNum);
}

ECode CFileHandler::GetHostAddress(
    /* [in] */ IURL* url,
    /* [out] */ IInetAddress** hostAddress)
{
    return FileHandler::GetHostAddress(url, hostAddress);
}

ECode CFileHandler::GetHashCode(
    /* [in] */ IURL* url,
    /* [out] */ Int32* code)
{
    return FileHandler::GetHashCode(url, code);
}

ECode CFileHandler::HostsEqual(
    /* [in] */ IURL* a,
    /* [in] */ IURL* b,
    /* [out] */ Boolean* isEqual)
{
    return FileHandler::HostsEqual(a, b, isEqual);
}

ECode CFileHandler::SameFile(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isSame)
{
    return FileHandler::SameFile(url1, url2, isSame);
}

} // namespace Url
} // namespace Net
} // namespace Elastos
