
#include "JarHandler.h"
#include "UrlUtils.h"
#include "CURL.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Libcore {
namespace Net {
namespace Url {

// CAR_INTERFACE_IMPL(JarHandler, IJarHandler);

ECode JarHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [out] */ IURLConnection** urlConnection)
{
    VALIDATE_NOT_NULL(urlConnection)

    assert(0 && "TODO");
    // return new JarURLConnectionImpl(u);
    return NOERROR;
}

ECode JarHandler::ParseURL(
    /* [in] */ IURL* url,
    /* [in] */ const String& spec,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String tmpspec;
    String file;
    url->GetFile(&file);
    if (file.IsNull()) {
        file = "";
    }
    if (end > start) {
        tmpspec = spec.Substring(start, end);
    }
    else {
        tmpspec = "";
    }
    if (tmpspec.IndexOf("!/") == -1 && (file.IndexOf("!/") == -1)) {
        // throw new NullPointerException("Cannot find \"!/\"");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (file.IsEmpty()) {
        file = tmpspec;
    }
    else if (tmpspec.GetChar(0) == '/') {
        file = file.Substring(0, file.IndexOf('!') + 1) + tmpspec;
    }
    else {
        Int32 idx = file.IndexOf('!');
        String tmpFile = file.Substring(idx + 1, file.LastIndexOf('/') + 1) + tmpspec;
        tmpFile = UrlUtils::CanonicalizePath(tmpFile, TRUE);
        file = file.Substring(0, idx + 1) + tmpFile;
    }
    // try {
        // check that the embedded url is valid
    AutoPtr<IURL> outurl;
    FAIL_RETURN(CURL::New(file, (IURL**)&outurl));
    // } catch (MalformedURLException e) {
    //     throw new NullPointerException(e.toString());
    // }
    return SetURL(url, String("jar"), String(""), -1, String(NULL), String(NULL), file, String(NULL), String(NULL));
}

ECode JarHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s)

    StringBuilder sb;
    sb.AppendString(String("jar:"));
    String file;
    url->GetFile(&file);
    sb.AppendString(file);
    String ref;
    url->GetRef(&ref);
    if (!ref.IsNull()) {
        sb.AppendString(ref);
    }
    *s = sb.ToString();
    return NOERROR;
}


ECode JarHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** urlConnection)
{
    return URLStreamHandler::OpenConnection(u, proxy, urlConnection);
}

ECode JarHandler::SetURL(
    /* [in] */ IURL* u,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ const String& ref)
{
    return URLStreamHandler::SetURL(u, protocol, host, port, file, ref);
}

ECode JarHandler::SetURL(
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

ECode JarHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [in] */ Boolean escapeIllegalCharacters,
    /* [out] */ String* s)
{
    return URLStreamHandler::ToExternalForm(url, escapeIllegalCharacters, s);
}

ECode JarHandler::Equals(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isEquals)
{
    return URLStreamHandler::Equals(url1, url2, isEquals);
}

ECode JarHandler::GetDefaultPort(
    /* [out] */ Int32* portNum)
{
    return URLStreamHandler::GetDefaultPort(portNum);
}

ECode JarHandler::GetHostAddress(
    /* [in] */ IURL* url,
    /* [out] */ IInetAddress** hostAddress)
{
    return URLStreamHandler::GetHostAddress(url, hostAddress);
}

ECode JarHandler::GetHashCode(
    /* [in] */ IURL* url,
    /* [out] */ Int32* code)
{
    return URLStreamHandler::GetHashCode(url, code);
}

ECode JarHandler::HostsEqual(
    /* [in] */ IURL* a,
    /* [in] */ IURL* b,
    /* [out] */ Boolean* isEqual)
{
    return URLStreamHandler::HostsEqual(a, b, isEqual);
}

ECode JarHandler::SameFile(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isSame)
{
    return URLStreamHandler::SameFile(url1, url2, isSame);
}

} // namespace Url
} // namespace Net
} // namespace Libcore
