// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                            LoadUrlParams
//=====================================================================
const Int32 LoadUrlParams::LOAD_TYPE_DEFAULT;
const Int32 LoadUrlParams::LOAD_TYPE_BROWSER_INITIATED_HTTP_POST;
const Int32 LoadUrlParams::LOAD_TYPE_DATA;
const Int32 LoadUrlParams::UA_OVERRIDE_INHERIT;
const Int32 LoadUrlParams::UA_OVERRIDE_FALSE;
const Int32 LoadUrlParams::UA_OVERRIDE_TRUE;

LoadUrlParams::LoadUrlParams(
    /* [in] */ String url)
{
    /*
    this(url, PageTransitionTypes.PAGE_TRANSITION_LINK);
    */
}

LoadUrlParams::LoadUrlParams(
    /* [in] */ String url,
    /* [in] */ Int32 transitionType)
    : :LoadUrlParams.
         mLoadUrlType = LOAD_TYPE_DEFAULT;
         mUaOverrideOption = UA_OVERRIDE_INHERIT;
         mPostData = null;
         mBaseUrlForDataUrl = null;
         mVirtualUrlForDataUrl = null;
     }
{
    /*
    mUrl = url;
    mTransitionType = transitionType;


    // Initialize other fields to defaults matching defaults of the native
    // NavigationController::LoadUrlParams.
    mLoadUrlType = LOAD_TYPE_DEFAULT;
    mUaOverrideOption = UA_OVERRIDE_INHERIT;
    mPostData = null;
    mBaseUrlForDataUrl = null;
    mVirtualUrlForDataUrl = null;
    */
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParams(
    /* [in] */ String data,
    /* [in] */ String mimeType,
    /* [in] */ Boolean isBase64Encoded)
{
    /*
    return createLoadDataParams(data, mimeType, isBase64Encoded, null);
    */
    return AutoPtr<LoadUrlParams>(this);
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParams(
    /* [in] */ String data,
    /* [in] */ String mimeType,
    /* [in] */ Boolean isBase64Encoded,
    /* [in] */ String charset)
{
    /*
    StringBuilder dataUrl = new StringBuilder("data:");
    dataUrl.append(mimeType);
    if (charset != null && !charset.isEmpty()) {
        dataUrl.append(";charset=" + charset);
    }
    if (isBase64Encoded) {
        dataUrl.append(";base64");
    }
    dataUrl.append(",");
    dataUrl.append(data);


    LoadUrlParams params = new LoadUrlParams(dataUrl.toString());
    params.setLoadType(LoadUrlParams.LOAD_TYPE_DATA);
    params.setTransitionType(PageTransitionTypes.PAGE_TRANSITION_TYPED);
    return params;
    */
    return AutoPtr<LoadUrlParams>(this);
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParamsWithBaseUrl(
    /* [in] */ String data,
    /* [in] */ String mimeType,
    /* [in] */ Boolean isBase64Encoded,
    /* [in] */ String baseUrl,
    /* [in] */ String historyUrl)
{
    /*
    return createLoadDataParamsWithBaseUrl(data, mimeType, isBase64Encoded,
            baseUrl, historyUrl, null);
    */
    return AutoPtr<LoadUrlParams>(this);
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParamsWithBaseUrl(
    /* [in] */ String data,
    /* [in] */ String mimeType,
    /* [in] */ Boolean isBase64Encoded,
    /* [in] */ String baseUrl,
    /* [in] */ String historyUrl,
    /* [in] */ String charset)
{
    /*
    LoadUrlParams params = createLoadDataParams(data, mimeType, isBase64Encoded, charset);
    // For WebView compatibility, when the base URL has the 'data:'
    // scheme, we treat it as a regular data URL load and skip setting
    // baseUrl and historyUrl.
    // TODO(joth): we should just append baseURL and historyURL here, and move the
    // WebView specific transform up to a wrapper factory function in android_webview/.
    if (baseUrl == null || !baseUrl.toLowerCase(Locale.US).startsWith("data:")) {
        params.setBaseUrlForDataUrl(baseUrl != null ? baseUrl : "about:blank");
        params.setVirtualUrlForDataUrl(historyUrl != null ? historyUrl : "about:blank");
    }
    return params;
    */
    return AutoPtr<LoadUrlParams>(this);
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadHttpPostParams(
    /* [in] */ String url,
    /* [in] */ ArrayOf<Byte>* postData)
{
    /*
    LoadUrlParams params = new LoadUrlParams(url);
    params.setLoadType(LOAD_TYPE_BROWSER_INITIATED_HTTP_POST);
    params.setTransitionType(PageTransitionTypes.PAGE_TRANSITION_TYPED);
    params.setPostData(postData);
    return params;
    */
    return AutoPtr<LoadUrlParams>(this);
}

ECode LoadUrlParams::SetUrl(
    /* [in] */ String url)
{
    /*
    mUrl = url;
    */
    return NOERROR;
}

String LoadUrlParams::GetUrl()
{
    /*
    return mUrl;
    */
    return String("");
}

String LoadUrlParams::GetBaseUrl()
{
    /*
    return mBaseUrlForDataUrl;
    */
    return String("");
}

ECode LoadUrlParams::SetLoadType(
    /* [in] */ Int32 loadType)
{
    /*
    mLoadUrlType = loadType;
    */
    return NOERROR;
}

ECode LoadUrlParams::SetTransitionType(
    /* [in] */ Int32 transitionType)
{
    /*
    mTransitionType = transitionType;
    */
    return NOERROR;
}

Int32 LoadUrlParams::GetTransitionType()
{
    /*
    return mTransitionType;
    */
    return 0;
}

ECode LoadUrlParams::SetReferrer(
    /* [in] */ Referrer* referrer)
{
    VALIDATE_NOT_NULL(referrer);
    /*
    mReferrer = referrer;
    */
    return NOERROR;
}

AutoPtr<Referrer> LoadUrlParams::GetReferrer()
{
    /*
    return mReferrer;
    */
    return AutoPtr<Referrer>(this);
}

ECode LoadUrlParams::SetExtraHeaders(
    /* [in] */ IMap<String, String>* extraHeaders)
{
    VALIDATE_NOT_NULL(extraHeaders);
    /*
    mExtraHeaders = extraHeaders;
    */
    return NOERROR;
}

AutoPtr< IMap<String, String> > LoadUrlParams::GetExtraHeaders()
{
    /*
    return mExtraHeaders;
    */
    return AutoPtr< IMap<String, String> >(this);
}

String LoadUrlParams::GetExtraHeadersString()
{
    /*
    return getExtraHeadersString("\n", false);
    */
    return String("");
}

String LoadUrlParams::GetExtraHttpRequestHeadersString()
{
    /*
    return getExtraHeadersString("\r\n", true);
    */
    return String("");
}

ECode LoadUrlParams::SetVerbatimHeaders(
    /* [in] */ String headers)
{
    /*
    mVerbatimHeaders = headers;
    */
    return NOERROR;
}

String LoadUrlParams::GetVerbatimHeaders()
{
    /*
    return mVerbatimHeaders;
    */
    return String("");
}

ECode LoadUrlParams::SetOverrideUserAgent(
    /* [in] */ Int32 uaOption)
{
    /*
    mUaOverrideOption = uaOption;
    */
    return NOERROR;
}

ECode LoadUrlParams::SetPostData(
    /* [in] */ ArrayOf<Byte>* postData)
{
    VALIDATE_NOT_NULL(postData);
    /*
    mPostData = postData;
    */
    return NOERROR;
}

AutoPtr< ArrayOf<Byte> > LoadUrlParams::GetPostData()
{
    /*
    return mPostData;
    */
    return AutoPtr< ArrayOf<Byte> >(this);
}

ECode LoadUrlParams::SetBaseUrlForDataUrl(
    /* [in] */ String baseUrl)
{
    /*
    mBaseUrlForDataUrl = baseUrl;
    */
    return NOERROR;
}

ECode LoadUrlParams::SetVirtualUrlForDataUrl(
    /* [in] */ String virtualUrl)
{
    /*
    mVirtualUrlForDataUrl = virtualUrl;
    */
    return NOERROR;
}

ECode LoadUrlParams::SetCanLoadLocalResources(
    /* [in] */ Boolean canLoad)
{
    /*
    mCanLoadLocalResources = canLoad;
    */
    return NOERROR;
}

Int32 LoadUrlParams::GetLoadUrlType()
{
    /*
    return mLoadUrlType;
    */
    return 0;
}

ECode LoadUrlParams::SetIsRendererInitiated(
    /* [in] */ Boolean rendererInitiated)
{
    /*
    mIsRendererInitiated = rendererInitiated;
    */
    return NOERROR;
}

Boolean LoadUrlParams::GetIsRendererInitiated()
{
    /*
    return mIsRendererInitiated;
    */
    return FALSE;
}

Boolean LoadUrlParams::IsBaseUrlDataScheme()
{
    /*
    // If there's no base url set, but this is a data load then
    // treat the scheme as data:.
    if (mBaseUrlForDataUrl == null && mLoadUrlType == LOAD_TYPE_DATA) {
        return true;
    }
    return nativeIsDataScheme(mBaseUrlForDataUrl);
    */
    return FALSE;
}

String LoadUrlParams::GetExtraHeadersString(
    /* [in] */ String delimiter,
    /* [in] */ Boolean addTerminator)
{
    /*
    if (mExtraHeaders == null) return null;


    StringBuilder headerBuilder = new StringBuilder();
    for (Map.Entry<String, String> header : mExtraHeaders.entrySet()) {
        if (headerBuilder.length() > 0) headerBuilder.append(delimiter);


        // Header name should be lower case.
        headerBuilder.append(header.getKey().toLowerCase(Locale.US));
        headerBuilder.append(":");
        headerBuilder.append(header.getValue());
    }
    if (addTerminator)
        headerBuilder.append(delimiter);


    return headerBuilder.toString();
    */
    return String("");
}

ECode LoadUrlParams::InitializeConstants(
    /* [in] */ Int32 load_type_default,
    /* [in] */ Int32 load_type_browser_initiated_http_post,
    /* [in] */ Int32 load_type_data,
    /* [in] */ Int32 ua_override_inherit,
    /* [in] */ Int32 ua_override_false,
    /* [in] */ Int32 ua_override_true)
{
    /*
    assert LOAD_TYPE_DEFAULT == load_type_default;
    assert LOAD_TYPE_BROWSER_INITIATED_HTTP_POST == load_type_browser_initiated_http_post;
    assert LOAD_TYPE_DATA == load_type_data;
    assert UA_OVERRIDE_INHERIT == ua_override_inherit;
    assert UA_OVERRIDE_FALSE == ua_override_false;
    assert UA_OVERRIDE_TRUE == ua_override_true;
    */
    return NOERROR;
}

Boolean LoadUrlParams::NativeIsDataScheme(
    /* [in] */ String url)
{
    return FALSE;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


