
#ifndef __CURLUTIL_H__
#define __CURLUTIL_H__

#include "_CURLUtil.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CURLUtil)
{
public:
    CARAPI GuessUrl(
        /* [in] */ const String& inUrl,
        /* [out] */ String* url);

    CARAPI ComposeSearchUrl(
        /* [in] */ const String& inQuery,
        /* [in] */ const String& tmp,
        /* [in] */ const String& queryPlaceHolder,
        /* [out] */ String* url);

    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* url,
        /* [out, callee] */ ArrayOf<Byte>** outUrl);

    CARAPI IsAssetUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsResourceUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsCookielessProxyUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsFileUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsAboutUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsDataUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsJavaScriptUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsHttpUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsHttpsUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsNetworkUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsContentUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI IsValidUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI StripAnchor(
        /* [in] */ const String& url,
        /* [out] */ String* result);

    CARAPI GuessFileName(
        /* [in] */ const String& url,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [out] */ String* fileName);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CURLUTIL_H__
