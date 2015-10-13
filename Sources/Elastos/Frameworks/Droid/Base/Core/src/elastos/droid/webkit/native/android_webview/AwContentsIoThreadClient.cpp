#include "elastos/droid/webkit/native/android_webview/AwContentsIoThreadClient.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//@CalledByNative
AutoPtr<AwWebResourceResponse> AwContentsIoThreadClient::ShouldInterceptRequest(
    /* [in] */ const String& url,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Boolean hasUserGesture,
    /* [in] */ const String& method,
    /* [in] */ ArrayOf<String>* requestHeaderNames,
    /* [in] */ ArrayOf<String>* requestHeaderValues)
{
    AutoPtr<AwContentsClient::ShouldInterceptRequestParams> params =
        new AwContentsClient::ShouldInterceptRequestParams();
    params->url = url;
    params->isMainFrame = isMainFrame;
    params->hasUserGesture = hasUserGesture;
    params->method = method;
    //params->requestHeaders = new ArrayMap<String, String>(requestHeaderNames.length);
    for (Int32 i = 0; i < requestHeaderNames->GetLength(); ++i) {
        //params->requestHeaders.put(requestHeaderNames[i], requestHeaderValues[i]);
        params->requestHeaders[(*requestHeaderNames)[i]] = (*requestHeaderValues)[i];
    }
    return ShouldInterceptRequest(params);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
