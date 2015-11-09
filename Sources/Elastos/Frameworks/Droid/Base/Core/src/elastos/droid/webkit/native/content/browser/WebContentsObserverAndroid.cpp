
#include "elastos/droid/webkit/native/content/browser/WebContentsObserverAndroid.h"
#include "elastos/droid/webkit/native/content/api/WebContentsObserverAndroid_dec.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewCore.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Webkit::Base::ThreadUtils;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                      WebContentsObserverAndroid
//=====================================================================

WebContentsObserverAndroid::WebContentsObserverAndroid(
    /* [in] */ ContentViewCore* contentViewCore)
{
    ThreadUtils::AssertOnUiThread();
    mNativeWebContentsObserverAndroid = NativeInit(contentViewCore->GetWebContents());
}

WebContentsObserverAndroid::WebContentsObserverAndroid(
    /* [in] */ WebContents* webContents)
{
    ThreadUtils::AssertOnUiThread();
    mNativeWebContentsObserverAndroid = NativeInit(webContents);
}

ECode WebContentsObserverAndroid::RenderProcessGone(
    /* [in] */ Boolean wasOomProtected)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStartLoading(
    /* [in] */ const String& url)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStopLoading(
    /* [in] */ const String& url)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFailLoad(
    /* [in] */ Boolean isProvisionalLoad,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidNavigateMainFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isNavigationToDifferentPage,
    /* [in] */ Boolean isFragmentNavigation)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFirstVisuallyNonEmptyPaint()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidNavigateAnyFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isReload)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidStartProvisionalLoadForFrame(
    /* [in] */ Int64 frameId,
    /* [in] */ Int64 parentFrameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ const String& validatedUrl,
    /* [in] */ Boolean isErrorPage,
    /* [in] */ Boolean isIframeSrcdoc)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidCommitProvisionalLoadForFrame(
    /* [in] */ Int64 frameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ const String& url,
    /* [in] */ Int32 transitionType)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidFinishLoad(
    /* [in] */ Int64 frameId,
    /* [in] */ const String& validatedUrl,
    /* [in] */ Boolean isMainFrame)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DocumentLoadedInFrame(
    /* [in] */ Int64 frameId)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::NavigationEntryCommitted()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidAttachInterstitialPage()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidDetachInterstitialPage()
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DidChangeThemeColor(
    /* [in] */ Int32 color)
{
    return NOERROR;
}

ECode WebContentsObserverAndroid::DetachFromWebContents()
{
    if (mNativeWebContentsObserverAndroid != 0) {
        NativeDestroy(mNativeWebContentsObserverAndroid);
        mNativeWebContentsObserverAndroid = 0;
    }

    return NOERROR;
}

Int64 WebContentsObserverAndroid::NativeInit(
    /* [in] */ WebContents* webContents)
{
    return Elastos_WebContentsObserverAndroid_nativeInit(THIS_PROBE(IInterface), TO_IINTERFACE(webContents));
}

ECode WebContentsObserverAndroid::NativeDestroy(
    /* [in] */ Int64 nativeWebContentsObserverAndroid)
{
    Elastos_WebContentsObserverAndroid_nativeDestroy(THIS_PROBE(IInterface),
            (Handle32)nativeWebContentsObserverAndroid);
    return NOERROR;
}

void WebContentsObserverAndroid::RenderProcessGone(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean wasOomProtected)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::RenderProcessGone, mObj is NULL");
        return;
    }
    mObj->RenderProcessGone(wasOomProtected);
}

void WebContentsObserverAndroid::DidStartLoading(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidStartLoading, mObj is NULL");
        return;
    }
    mObj->DidStartLoading(url);
}

void WebContentsObserverAndroid::DidStopLoading(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidStopLoading, mObj is NULL");
        return;
    }
    mObj->DidStopLoading(url);
}

void WebContentsObserverAndroid::DidFailLoad(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean isProvisionalLoad,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidFailLoad, mObj is NULL");
        return;
    }
    mObj->DidFailLoad(isProvisionalLoad, isMainFrame, errorCode, description, failingUrl);
}

void WebContentsObserverAndroid::DidNavigateMainFrame(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isNavigationToDifferentPage,
    /* [in] */ Boolean isFragmentNavigation)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidNavigateMainFrame, mObj is NULL");
        return;
    }
    mObj->DidNavigateMainFrame(url, baseUrl, isNavigationToDifferentPage, isFragmentNavigation);
}

void WebContentsObserverAndroid::DidFirstVisuallyNonEmptyPaint(
    /* [in] */ IInterface* obj)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidFirstVisuallyNonEmptyPaint, mObj is NULL");
        return;
    }
    mObj->DidFirstVisuallyNonEmptyPaint();
}

void WebContentsObserverAndroid::DidNavigateAnyFrame(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isReload)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidNavigateAnyFrame, mObj is NULL");
        return;
    }
    mObj->DidNavigateAnyFrame(url, baseUrl, isReload);
}

void WebContentsObserverAndroid::DidStartProvisionalLoadForFrame(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 frameId,
    /* [in] */ Int64 parentFrameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ const String& validatedUrl,
    /* [in] */ Boolean isErrorPage,
    /* [in] */ Boolean isIframeSrcdoc)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidStartProvisionalLoadForFrame, mObj is NULL");
        return;
    }
    mObj->DidStartProvisionalLoadForFrame(frameId, parentFrameId, isMainFrame, validatedUrl, isErrorPage, isIframeSrcdoc);
}

void WebContentsObserverAndroid::DidCommitProvisionalLoadForFrame(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 frameId,
    /* [in] */ Boolean isMainFrame,
    /* [in] */ const String& url,
    /* [in] */ Int32 transitionType)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidCommitProvisionalLoadForFrame, mObj is NULL");
        return;
    }
    mObj->DidCommitProvisionalLoadForFrame(frameId, isMainFrame, url, transitionType);
}

void WebContentsObserverAndroid::DidFinishLoad(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 frameId,
    /* [in] */ const String& validatedUrl,
    /* [in] */ Boolean isMainFrame)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidFinishLoad, mObj is NULL");
        return;
    }
    mObj->DidFinishLoad(frameId, validatedUrl, isMainFrame);
}

void WebContentsObserverAndroid::DocumentLoadedInFrame(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 frameId)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DocumentLoadedInFrame, mObj is NULL");
        return;
    }
    mObj->DocumentLoadedInFrame(frameId);
}

void WebContentsObserverAndroid::NavigationEntryCommitted(
    /* [in] */ IInterface* obj)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::NavigationEntryCommitted, mObj is NULL");
        return;
    }
    mObj->NavigationEntryCommitted();
}

void WebContentsObserverAndroid::DidAttachInterstitialPage(
    /* [in] */ IInterface* obj)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidAttachInterstitialPage, mObj is NULL");
        return;
    }
    mObj->DidAttachInterstitialPage();
}

void WebContentsObserverAndroid::DidDetachInterstitialPage(
    /* [in] */ IInterface* obj)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidDetachInterstitialPage, mObj is NULL");
        return;
    }
    mObj->DidDetachInterstitialPage();
}

void WebContentsObserverAndroid::DidChangeThemeColor(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 color)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DidChangeThemeColor, mObj is NULL");
        return;
    }
    mObj->DidChangeThemeColor(color);
}

void WebContentsObserverAndroid::DetachFromWebContents(
    /* [in] */ IInterface* obj)
{
    AutoPtr<WebContentsObserverAndroid> mObj = (WebContentsObserverAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("WebContentsObserverAndroid", "WebContentsObserverAndroid::DetachFromWebContents, mObj is NULL");
        return;
    }
    mObj->DetachFromWebContents();
}


} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
