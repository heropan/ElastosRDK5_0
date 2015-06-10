
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//@CalledByNative
AutoPtr<AwWebResourceResponse> AwContentsIoThreadClient::ShouldInterceptRequest(
    /* [in] */ String url,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Boolean hasUserGesture,
    /* [in] */ String method,
    /* [in] */ ArrayOf<String>* requestHeaderNames,
    /* [in] */ ArrayOf<String>* requestHeaderValues)
{
    AutoPtr<AwContentsClient::ShouldInterceptRequestParams> params =
        new AwContentsClient.ShouldInterceptRequestParams();
    params->url = url;
    params->isMainFrame = isMainFrame;
    params->hasUserGesture = hasUserGesture;
    params->method = method;
    params.requestHeaders = new ArrayMap<String, String>(requestHeaderNames.length);
    for (Int32 i = 0; i < requestHeaderNames.length; ++i) {
        params.requestHeaders.put(requestHeaderNames[i], requestHeaderValues[i]);
    }
    return ShouldInterceptRequest(params);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos