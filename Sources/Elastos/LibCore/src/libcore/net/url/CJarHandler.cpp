
#include "CJarHandler.h"
#include "UrlUtils.h"
#include "CURL.h"
#include "StringBuilder.h"
#include "CJarURLConnectionImpl.h"

using Elastos::Core::StringBuilder;
using Elastos::Net::CURL;

namespace Libcore {
namespace Net {
namespace Url {

// 6b5a0016-9e17-4dc5-80d7-397ff4118c2f
extern "C" const InterfaceID EIID_JarHandler =
    { 0x6b5a0016, 0x9e17, 0x4dc5, { 0x80, 0xd7, 0x39, 0x7f, 0xf4, 0x11, 0x8c, 0x2f } };

CAR_INTERFACE_IMPL(CJarHandler, URLStreamHandler, IJarHandler)

ECode CJarHandler::constructor()
{
    return NOERROR;
}

ECode CJarHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [out] */ IURLConnection** urlConnection)
{
    VALIDATE_NOT_NULL(urlConnection)

    return CJarURLConnectionImpl::New(u, urlConnection);
}

ECode CJarHandler::ParseURL(
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

ECode CJarHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s)

    StringBuilder sb;
    sb.Append(String("jar:"));
    String file;
    url->GetFile(&file);
    sb.Append(file);
    String ref;
    url->GetRef(&ref);
    if (!ref.IsNull()) {
        sb.Append(ref);
    }
    *s = sb.ToString();
    return NOERROR;
}

} // namespace Url
} // namespace Net
} // namespace Libcore
