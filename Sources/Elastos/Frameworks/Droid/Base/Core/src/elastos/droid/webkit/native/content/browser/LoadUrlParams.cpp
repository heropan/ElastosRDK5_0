// wuweizuo automatic build .cpp file from .java file.

#include "LoadUrlParams.h"

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
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // this(url, PageTransitionTypes.PAGE_TRANSITION_LINK);
}

LoadUrlParams::LoadUrlParams(
    /* [in] */ const String& url,
    /* [in] */ Int32 transitionType)
    : :LoadUrlParams.
         mLoadUrlType = LOAD_TYPE_DEFAULT;
         mUaOverrideOption = UA_OVERRIDE_INHERIT;
         mPostData = null;
         mBaseUrlForDataUrl = null;
         mVirtualUrlForDataUrl = null;
     }
{
    // ==================before translated======================
    // mUrl = url;
    // mTransitionType = transitionType;
    //
    // // Initialize other fields to defaults matching defaults of the native
    // // NavigationController::LoadUrlParams.
    // mLoadUrlType = LOAD_TYPE_DEFAULT;
    // mUaOverrideOption = UA_OVERRIDE_INHERIT;
    // mPostData = null;
    // mBaseUrlForDataUrl = null;
    // mVirtualUrlForDataUrl = null;
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParams(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ Boolean isBase64Encoded)
{
    // ==================before translated======================
    // return createLoadDataParams(data, mimeType, isBase64Encoded, null);
    assert(0);
    AutoPtr<LoadUrlParams> empty;
    return empty;
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParams(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ Boolean isBase64Encoded,
    /* [in] */ const String& charset)
{
    // ==================before translated======================
    // StringBuilder dataUrl = new StringBuilder("data:");
    // dataUrl.append(mimeType);
    // if (charset != null && !charset.isEmpty()) {
    //     dataUrl.append(";charset=" + charset);
    // }
    // if (isBase64Encoded) {
    //     dataUrl.append(";base64");
    // }
    // dataUrl.append(",");
    // dataUrl.append(data);
    //
    // LoadUrlParams params = new LoadUrlParams(dataUrl.toString());
    // params.setLoadType(LoadUrlParams.LOAD_TYPE_DATA);
    // params.setTransitionType(PageTransitionTypes.PAGE_TRANSITION_TYPED);
    // return params;
    assert(0);
    AutoPtr<LoadUrlParams> empty;
    return empty;
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParamsWithBaseUrl(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ Boolean isBase64Encoded,
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& historyUrl)
{
    // ==================before translated======================
    // return createLoadDataParamsWithBaseUrl(data, mimeType, isBase64Encoded,
    //         baseUrl, historyUrl, null);
    assert(0);
    AutoPtr<LoadUrlParams> empty;
    return empty;
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadDataParamsWithBaseUrl(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ Boolean isBase64Encoded,
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& historyUrl,
    /* [in] */ const String& charset)
{
    // ==================before translated======================
    // LoadUrlParams params = createLoadDataParams(data, mimeType, isBase64Encoded, charset);
    // // For WebView compatibility, when the base URL has the 'data:'
    // // scheme, we treat it as a regular data URL load and skip setting
    // // baseUrl and historyUrl.
    // // TODO(joth): we should just append baseURL and historyURL here, and move the
    // // WebView specific transform up to a wrapper factory function in android_webview/.
    // if (baseUrl == null || !baseUrl.toLowerCase(Locale.US).startsWith("data:")) {
    //     params.setBaseUrlForDataUrl(baseUrl != null ? baseUrl : "about:blank");
    //     params.setVirtualUrlForDataUrl(historyUrl != null ? historyUrl : "about:blank");
    // }
    // return params;
    assert(0);
    AutoPtr<LoadUrlParams> empty;
    return empty;
}

AutoPtr<LoadUrlParams> LoadUrlParams::CreateLoadHttpPostParams(
    /* [in] */ const String& url,
    /* [in] */ ArrayOf<Byte>* postData)
{
    // ==================before translated======================
    // LoadUrlParams params = new LoadUrlParams(url);
    // params.setLoadType(LOAD_TYPE_BROWSER_INITIATED_HTTP_POST);
    // params.setTransitionType(PageTransitionTypes.PAGE_TRANSITION_TYPED);
    // params.setPostData(postData);
    // return params;
    assert(0);
    AutoPtr<LoadUrlParams> empty;
    return empty;
}

ECode LoadUrlParams::SetUrl(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // mUrl = url;
    assert(0);
    return NOERROR;
}

String LoadUrlParams::GetUrl()
{
    // ==================before translated======================
    // return mUrl;
    assert(0);
    return String("");
}

String LoadUrlParams::GetBaseUrl()
{
    // ==================before translated======================
    // return mBaseUrlForDataUrl;
    assert(0);
    return String("");
}

ECode LoadUrlParams::SetLoadType(
    /* [in] */ Int32 loadType)
{
    // ==================before translated======================
    // mLoadUrlType = loadType;
    assert(0);
    return NOERROR;
}

ECode LoadUrlParams::SetTransitionType(
    /* [in] */ Int32 transitionType)
{
    // ==================before translated======================
    // mTransitionType = transitionType;
    assert(0);
    return NOERROR;
}

Int32 LoadUrlParams::GetTransitionType()
{
    // ==================before translated======================
    // return mTransitionType;
    assert(0);
    return 0;
}

ECode LoadUrlParams::SetReferrer(
    /* [in] */ Referrer* referrer)
{
    VALIDATE_NOT_NULL(referrer);
    // ==================before translated======================
    // mReferrer = referrer;
    assert(0);
    return NOERROR;
}

AutoPtr<Referrer> LoadUrlParams::GetReferrer()
{
    // ==================before translated======================
    // return mReferrer;
    assert(0);
    AutoPtr<Referrer> empty;
    return empty;
}

ECode LoadUrlParams::SetExtraHeaders(
    /* [in] */ IMap<String, String>* extraHeaders)
{
    VALIDATE_NOT_NULL(extraHeaders);
    // ==================before translated======================
    // mExtraHeaders = extraHeaders;
    assert(0);
    return NOERROR;
}

AutoPtr< IMap<String, String> > LoadUrlParams::GetExtraHeaders()
{
    // ==================before translated======================
    // return mExtraHeaders;
    assert(0);
    AutoPtr< IMap<String, String> > empty;
    return empty;
}

String LoadUrlParams::GetExtraHeadersString()
{
    // ==================before translated======================
    // return getExtraHeadersString("\n", false);
    assert(0);
    return String("");
}

String LoadUrlParams::GetExtraHttpRequestHeadersString()
{
    // ==================before translated======================
    // return getExtraHeadersString("\r\n", true);
    assert(0);
    return String("");
}

ECode LoadUrlParams::SetVerbatimHeaders(
    /* [in] */ const String& headers)
{
    // ==================before translated======================
    // mVerbatimHeaders = headers;
    assert(0);
    return NOERROR;
}

String LoadUrlParams::GetVerbatimHeaders()
{
    // ==================before translated======================
    // return mVerbatimHeaders;
    assert(0);
    return String("");
}

ECode LoadUrlParams::SetOverrideUserAgent(
    /* [in] */ Int32 uaOption)
{
    // ==================before translated======================
    // mUaOverrideOption = uaOption;
    assert(0);
    return NOERROR;
}

ECode LoadUrlParams::SetPostData(
    /* [in] */ ArrayOf<Byte>* postData)
{
    VALIDATE_NOT_NULL(postData);
    // ==================before translated======================
    // mPostData = postData;
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<Byte> > LoadUrlParams::GetPostData()
{
    // ==================before translated======================
    // return mPostData;
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

ECode LoadUrlParams::SetBaseUrlForDataUrl(
    /* [in] */ const String& baseUrl)
{
    // ==================before translated======================
    // mBaseUrlForDataUrl = baseUrl;
    assert(0);
    return NOERROR;
}

ECode LoadUrlParams::SetVirtualUrlForDataUrl(
    /* [in] */ const String& virtualUrl)
{
    // ==================before translated======================
    // mVirtualUrlForDataUrl = virtualUrl;
    assert(0);
    return NOERROR;
}

ECode LoadUrlParams::SetCanLoadLocalResources(
    /* [in] */ Boolean canLoad)
{
    // ==================before translated======================
    // mCanLoadLocalResources = canLoad;
    assert(0);
    return NOERROR;
}

Int32 LoadUrlParams::GetLoadUrlType()
{
    // ==================before translated======================
    // return mLoadUrlType;
    assert(0);
    return 0;
}

ECode LoadUrlParams::SetIsRendererInitiated(
    /* [in] */ Boolean rendererInitiated)
{
    // ==================before translated======================
    // mIsRendererInitiated = rendererInitiated;
    assert(0);
    return NOERROR;
}

Boolean LoadUrlParams::GetIsRendererInitiated()
{
    // ==================before translated======================
    // return mIsRendererInitiated;
    assert(0);
    return FALSE;
}

Boolean LoadUrlParams::IsBaseUrlDataScheme()
{
    // ==================before translated======================
    // // If there's no base url set, but this is a data load then
    // // treat the scheme as data:.
    // if (mBaseUrlForDataUrl == null && mLoadUrlType == LOAD_TYPE_DATA) {
    //     return true;
    // }
    // return nativeIsDataScheme(mBaseUrlForDataUrl);
    assert(0);
    return FALSE;
}

String LoadUrlParams::GetExtraHeadersString(
    /* [in] */ const String& delimiter,
    /* [in] */ Boolean addTerminator)
{
    // ==================before translated======================
    // if (mExtraHeaders == null) return null;
    //
    // StringBuilder headerBuilder = new StringBuilder();
    // for (Map.Entry<String, String> header : mExtraHeaders.entrySet()) {
    //     if (headerBuilder.length() > 0) headerBuilder.append(delimiter);
    //
    //     // Header name should be lower case.
    //     headerBuilder.append(header.getKey().toLowerCase(Locale.US));
    //     headerBuilder.append(":");
    //     headerBuilder.append(header.getValue());
    // }
    // if (addTerminator)
    //     headerBuilder.append(delimiter);
    //
    // return headerBuilder.toString();
    assert(0);
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
    // ==================before translated======================
    // assert LOAD_TYPE_DEFAULT == load_type_default;
    // assert LOAD_TYPE_BROWSER_INITIATED_HTTP_POST == load_type_browser_initiated_http_post;
    // assert LOAD_TYPE_DATA == load_type_data;
    // assert UA_OVERRIDE_INHERIT == ua_override_inherit;
    // assert UA_OVERRIDE_FALSE == ua_override_false;
    // assert UA_OVERRIDE_TRUE == ua_override_true;
    assert(0);
    return NOERROR;
}

Boolean LoadUrlParams::NativeIsDataScheme(
    /* [in] */ const String& url)
{
    assert(0);
    return FALSE;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


