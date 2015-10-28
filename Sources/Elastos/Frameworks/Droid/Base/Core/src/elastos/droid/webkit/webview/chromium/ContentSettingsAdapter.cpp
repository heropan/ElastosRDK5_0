
#include "elastos/droid/webkit/webview/chromium/ContentSettingsAdapter.h"
#include "elastos/core/AutoLock.h"
#include "elastos/droid/webkit/DebugFlags.h"

using Elastos::Core::AutoLock;
using Elastos::Droid::Webkit::DebugFlags;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//                        ContentSettingsAdapter
//=====================================================================
const String ContentSettingsAdapter::LOGTAG;// = ContentSettingsAdapter.class.getSimpleName();

ContentSettingsAdapter::ContentSettingsAdapter(
    /* [in] */ AwSettings* awSettings)
    //: mAwSettings(awSettings)
{
    // ==================before translated======================
    // mAwSettings = awSettings;
}

AutoPtr<AwSettings> ContentSettingsAdapter::GetAwSettings()
{
    // ==================before translated======================
    // return mAwSettings;

    return mAwSettings;
}

ECode ContentSettingsAdapter::SetNavDump(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

Boolean ContentSettingsAdapter::GetNavDump()
{
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    return FALSE;
}

ECode ContentSettingsAdapter::SetSupportZoom(
    /* [in] */ Boolean support)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setSupportZoom=" + support);
    // mAwSettings.setSupportZoom(support);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setSupportZoom=" + support);
    }
    //mAwSettings->SetSupportZoom(support);
    return NOERROR;
}

Boolean ContentSettingsAdapter::SupportZoom()
{
    // ==================before translated======================
    // return mAwSettings.supportZoom();

    assert(0);
    //return mAwSettings->SupportZoom();
    return FALSE;
}

ECode ContentSettingsAdapter::SetBuiltInZoomControls(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setBuiltInZoomControls=" + enabled);
    // mAwSettings.setBuiltInZoomControls(enabled);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setBuiltInZoomControls=" + enabled);
    }
    //mAwSettings->SetBuiltInZoomControls(enabled);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetBuiltInZoomControls()
{
    // ==================before translated======================
    // return mAwSettings.getBuiltInZoomControls();

    assert(0);
    //return mAwSettings->GetBuiltInZoomControls();
    return FALSE;
}

ECode ContentSettingsAdapter::SetDisplayZoomControls(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setDisplayZoomControls=" + enabled);
    // mAwSettings.setDisplayZoomControls(enabled);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setDisplayZoomControls=" + enabled);
    }
    //mAwSettings->SetDisplayZoomControls(enabled);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetDisplayZoomControls()
{
    // ==================before translated======================
    // return mAwSettings.getDisplayZoomControls();

    assert(0);
    //return mAwSettings->GetDisplayZoomControls();
    return FALSE;
}

ECode ContentSettingsAdapter::SetAllowFileAccess(
    /* [in] */ Boolean allow)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAllowFileAccess=" + allow);
    // mAwSettings.setAllowFileAccess(allow);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setAllowFileAccess=" + allow);
    }
    //mAwSettings->SetAllowFileAccess(allow);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetAllowFileAccess()
{
    // ==================before translated======================
    // return mAwSettings.getAllowFileAccess();

    assert(0);
    //return mAwSettings->GetAllowFileAccess();
    return FALSE;
}

ECode ContentSettingsAdapter::SetAllowContentAccess(
    /* [in] */ Boolean allow)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAllowContentAccess=" + allow);
    // mAwSettings.setAllowContentAccess(allow);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setAllowContentAccess=" + allow);
    }
    //mAwSettings->SetAllowContentAccess(allow);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetAllowContentAccess()
{
    // ==================before translated======================
    // return mAwSettings.getAllowContentAccess();

    assert(0);
    //return mAwSettings->GetAllowContentAccess();
    return FALSE;
}

ECode ContentSettingsAdapter::SetLoadWithOverviewMode(
    /* [in] */ Boolean overview)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setLoadWithOverviewMode=" + overview);
    // mAwSettings.setLoadWithOverviewMode(overview);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setLoadWithOverviewMode=" + overview);
    }
    //mAwSettings->SetLoadWithOverviewMode(overview);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetLoadWithOverviewMode()
{
    // ==================before translated======================
    // return mAwSettings.getLoadWithOverviewMode();

    assert(0);
    //return mAwSettings->GetLoadWithOverviewMode();
    return FALSE;
}

ECode ContentSettingsAdapter::SetAcceptThirdPartyCookies(
    /* [in] */ Boolean accept)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAcceptThirdPartyCookies=" + accept);
    // mAwSettings.setAcceptThirdPartyCookies(accept);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setAcceptThirdPartyCookies=" + accept);
    }
    //mAwSettings->SetAcceptThirdPartyCookies(accept);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetAcceptThirdPartyCookies()
{
    // ==================before translated======================
    // return mAwSettings.getAcceptThirdPartyCookies();

    assert(0);
    //return mAwSettings->GetAcceptThirdPartyCookies();
    return FALSE;
}

ECode ContentSettingsAdapter::SetEnableSmoothTransition(
    /* [in] */ Boolean enable)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

Boolean ContentSettingsAdapter::EnableSmoothTransition()
{
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    return FALSE;
}

ECode ContentSettingsAdapter::SetUseWebViewBackgroundForOverscrollBackground(
    /* [in] */ Boolean view)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

Boolean ContentSettingsAdapter::GetUseWebViewBackgroundForOverscrollBackground()
{
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    return FALSE;
}

ECode ContentSettingsAdapter::SetSaveFormData(
    /* [in] */ Boolean save)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setSaveFormData=" + save);
    // mAwSettings.setSaveFormData(save);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setSaveFormData=" + save);
    }
    //mAwSettings->SetSaveFormData(save);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetSaveFormData()
{
    // ==================before translated======================
    // return mAwSettings.getSaveFormData();

    assert(0);
    //return mAwSettings->GetSaveFormData();
    return FALSE;
}

ECode ContentSettingsAdapter::SetSavePassword(
    /* [in] */ Boolean save)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

Boolean ContentSettingsAdapter::GetSavePassword()
{
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetTextZoom(
    /* [in] */ Int32 textZoom)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setTextZoom=" + textZoom);
    // mAwSettings.setTextZoom(textZoom);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setTextZoom=" + textZoom);
    }
    //mAwSettings->SetTextZoom(textZoom);
    return NOERROR;
}

// synchronized
Int32 ContentSettingsAdapter::GetTextZoom()
{
    // ==================before translated======================
    // return mAwSettings.getTextZoom();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetTextZoom();
    return 0;
}

ECode ContentSettingsAdapter::SetDefaultZoom(
    /* [in] */ Int32/*WebSettings::ZoomDensity*/ zoom)
{
    // ==================before translated======================
    // if (zoom != ZoomDensity.MEDIUM) {
    //     Log.w(LOGTAG, "setDefaultZoom not supported, zoom=" + zoom);
    // }

    assert(0);
    if (zoom != -1/*ZoomDensity.MEDIUM*/) {
        //Log.w(LOGTAG, "setDefaultZoom not supported, zoom=" + zoom);
    }
    return NOERROR;
}

Int32/*WebSettings::ZoomDensity*/ ContentSettingsAdapter::GetDefaultZoom()
{
    // ==================before translated======================
    // // Intentional no-op.
    // return ZoomDensity.MEDIUM;

    assert(0);
    //return WebSettings::ZoomDensity::MEDIUM;
    return -1;
}

ECode ContentSettingsAdapter::SetLightTouchEnabled(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

Boolean ContentSettingsAdapter::GetLightTouchEnabled()
{
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetUserAgent(
    /* [in] */ Int32 ua)
{
    // ==================before translated======================
    // // Minimal implementation for backwards compatibility: just supports resetting to default.
    // if (ua == 0) {
    //     setUserAgentString(null);
    // } else {
    //     Log.w(LOGTAG, "setUserAgent not supported, ua=" + ua);
    // }

    assert(0);
    AutoLock lock(this);
    // Minimal implementation for backwards compatibility: just supports resetting to default.
    if (ua == 0) {
        SetUserAgentString(String(""));
    }
    else {
        //Log.w(LOGTAG, "setUserAgent not supported, ua=" + ua);
    }
    return NOERROR;
}

// synchronized
Int32 ContentSettingsAdapter::GetUserAgent()
{
    // ==================before translated======================
    // // Minimal implementation for backwards compatibility: just identifies default vs custom.
    // return AwSettings.getDefaultUserAgent().equals(getUserAgentString()) ? 0 : -1;

    assert(0);
    AutoLock lock(this);
    // Minimal implementation for backwards compatibility: just identifies default vs custom.
    //return AwSettings::GetDefaultUserAgent()->Equals(GetUserAgentString()) ? 0 : -1;
    return 0;
}

// synchronized
ECode ContentSettingsAdapter::SetUseWideViewPort(
    /* [in] */ Boolean use)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setUseWideViewPort=" + use);
    // mAwSettings.setUseWideViewPort(use);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setUseWideViewPort=" + use);
    }
    //mAwSettings->SetUseWideViewPort(use);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::GetUseWideViewPort()
{
    // ==================before translated======================
    // return mAwSettings.getUseWideViewPort();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetUseWideViewPort();
    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetSupportMultipleWindows(
    /* [in] */ Boolean support)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setSupportMultipleWindows=" + support);
    // mAwSettings.setSupportMultipleWindows(support);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setSupportMultipleWindows=" + support);
    }
    //mAwSettings->SetSupportMultipleWindows(support);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::SupportMultipleWindows()
{
    // ==================before translated======================
    // return mAwSettings.supportMultipleWindows();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->SupportMultipleWindows();
    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetLayoutAlgorithm(
    /* [in] */ Int32/*AwSettings::LayoutAlgorithm*/ la)
{
    // ==================before translated======================
    // // TODO: Remove the upstream enum and mapping once the new value is in the public API.
    // final AwSettings.LayoutAlgorithm[] chromiumValues = {
    //     AwSettings.LayoutAlgorithm.NORMAL,
    //     AwSettings.LayoutAlgorithm.SINGLE_COLUMN,
    //     AwSettings.LayoutAlgorithm.NARROW_COLUMNS,
    //     AwSettings.LayoutAlgorithm.TEXT_AUTOSIZING
    // };
    // mAwSettings.setLayoutAlgorithm(chromiumValues[l.ordinal()]);

    assert(0);
    AutoLock lock(this);
    /*const AwSettings::LayoutAlgorithm chromiumValues[] = {
        AwSettings::LayoutAlgorithm::NORMAL,
        AwSettings::LayoutAlgorithm::SINGLE_COLUMN,
        AwSettings::LayoutAlgorithm::NARROW_COLUMNS,
        AwSettings::LayoutAlgorithm::TEXT_AUTOSIZING
    };

    Int32 index = la - -1;//AwSettings::LayoutAlgorithm::NORMAL;
    mAwSettings->SetLayoutAlgorithm(index]);*/
    return NOERROR;
}

// synchronized
Int32/*AwSettings::LayoutAlgorithm*/ ContentSettingsAdapter::GetLayoutAlgorithm()
{
    // ==================before translated======================
    // // TODO: Remove the upstream enum and mapping once the new value is in the public API.
    // final LayoutAlgorithm[] webViewValues = {
    //     LayoutAlgorithm.NORMAL,
    //     LayoutAlgorithm.SINGLE_COLUMN,
    //     LayoutAlgorithm.NARROW_COLUMNS,
    //     LayoutAlgorithm.TEXT_AUTOSIZING
    // };
    // return webViewValues[mAwSettings.getLayoutAlgorithm().ordinal()];

    assert(0);
    AutoLock lock(this);
    /*const AwSettings::LayoutAlgorithm webViewValues[] = {
        AwSettings::LayoutAlgorithm::NORMAL,
        AwSettings::LayoutAlgorithm::SINGLE_COLUMN,
        AwSettings::LayoutAlgorithm::NARROW_COLUMNS,
        AwSettings::LayoutAlgorithm::TEXT_AUTOSIZING
    };

    Int32 index = mAwSettings->GetLayoutAlgorithm() - -1;//AwSettings::LayoutAlgorithm::NORMAL;
    return webViewValues[index];*/
    return -1;
}

// synchronized
ECode ContentSettingsAdapter::SetStandardFontFamily(
    /* [in] */ const String& font)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setStandardFontFamily=" + font);
    // mAwSettings.setStandardFontFamily(font);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setStandardFontFamily=" + font);
    }
    //mAwSettings->SetStandardFontFamily(font);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetStandardFontFamily()
{
    // ==================before translated======================
    // return mAwSettings.getStandardFontFamily();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetStandardFontFamily();
    return String("");
}

// synchronized
ECode ContentSettingsAdapter::SetFixedFontFamily(
    /* [in] */ const String& font)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setFixedFontFamily=" + font);
    // mAwSettings.setFixedFontFamily(font);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setFixedFontFamily=" + font);
    }
    //mAwSettings->SetFixedFontFamily(font);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetFixedFontFamily()
{
    // ==================before translated======================
    // return mAwSettings.getFixedFontFamily();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetFixedFontFamily();
    return String("");
}

// synchronized
ECode ContentSettingsAdapter::SetSansSerifFontFamily(
    /* [in] */ const String& font)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setSansSerifFontFamily=" + font);
    // mAwSettings.setSansSerifFontFamily(font);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setSansSerifFontFamily=" + font);
    }
    //mAwSettings->SetSansSerifFontFamily(font);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetSansSerifFontFamily()
{
    // ==================before translated======================
    // return mAwSettings.getSansSerifFontFamily();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetSansSerifFontFamily();
    return String("");
}

// synchronized
ECode ContentSettingsAdapter::SetSerifFontFamily(
    /* [in] */ const String& font)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setSerifFontFamily=" + font);
    // mAwSettings.setSerifFontFamily(font);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setSerifFontFamily=" + font);
    }
    //mAwSettings->SetSerifFontFamily(font);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetSerifFontFamily()
{
    // ==================before translated======================
    // return mAwSettings.getSerifFontFamily();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetSerifFontFamily();
    return String("");
}

// synchronized
ECode ContentSettingsAdapter::SetCursiveFontFamily(
    /* [in] */ const String& font)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setCursiveFontFamily=" + font);
    // mAwSettings.setCursiveFontFamily(font);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setCursiveFontFamily=" + font);
    }
    //mAwSettings->SetCursiveFontFamily(font);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetCursiveFontFamily()
{
    // ==================before translated======================
    // return mAwSettings.getCursiveFontFamily();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetCursiveFontFamily();
    return String("");
}

// synchronized
ECode ContentSettingsAdapter::SetFantasyFontFamily(
    /* [in] */ const String& font)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setFantasyFontFamily=" + font);
    // mAwSettings.setFantasyFontFamily(font);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setFantasyFontFamily=" + font);
    }
    //mAwSettings->SetFantasyFontFamily(font);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetFantasyFontFamily()
{
    // ==================before translated======================
    // return mAwSettings.getFantasyFontFamily();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetFantasyFontFamily();
    return String("");
}

// synchronized
ECode ContentSettingsAdapter::SetMinimumFontSize(
    /* [in] */ Int32 size)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setMinimumFontSize=" + size);
    // mAwSettings.setMinimumFontSize(size);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setMinimumFontSize=" + size);
    }
    //mAwSettings->SetMinimumFontSize(size);
    return NOERROR;
}

// synchronized
Int32 ContentSettingsAdapter::GetMinimumFontSize()
{
    // ==================before translated======================
    // return mAwSettings.getMinimumFontSize();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetMinimumFontSize();
    return 0;
}

// synchronized
ECode ContentSettingsAdapter::SetMinimumLogicalFontSize(
    /* [in] */ Int32 size)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setMinimumLogicalFontSize=" + size);
    // mAwSettings.setMinimumLogicalFontSize(size);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setMinimumLogicalFontSize=" + size);
    }
    //mAwSettings->SetMinimumLogicalFontSize(size);
    return NOERROR;
}

// synchronized
Int32 ContentSettingsAdapter::GetMinimumLogicalFontSize()
{
    // ==================before translated======================
    // return mAwSettings.getMinimumLogicalFontSize();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetMinimumLogicalFontSize();
    return 0;
}

// synchronized
ECode ContentSettingsAdapter::SetDefaultFontSize(
    /* [in] */ Int32 size)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setDefaultFontSize=" + size);
    // mAwSettings.setDefaultFontSize(size);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setDefaultFontSize=" + size);
    }
    //mAwSettings->SetDefaultFontSize(size);
    return NOERROR;
}

// synchronized
Int32 ContentSettingsAdapter::GetDefaultFontSize()
{
    // ==================before translated======================
    // return mAwSettings.getDefaultFontSize();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetDefaultFontSize();
    return 0;
}

// synchronized
ECode ContentSettingsAdapter::SetDefaultFixedFontSize(
    /* [in] */ Int32 size)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setDefaultFixedFontSize=" + size);
    // mAwSettings.setDefaultFixedFontSize(size);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setDefaultFixedFontSize=" + size);
    }
    //mAwSettings->SetDefaultFixedFontSize(size);
    return NOERROR;
}

// synchronized
Int32 ContentSettingsAdapter::GetDefaultFixedFontSize()
{
    // ==================before translated======================
    // return mAwSettings.getDefaultFixedFontSize();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetDefaultFixedFontSize();
    return 0;
}

// synchronized
ECode ContentSettingsAdapter::SetLoadsImagesAutomatically(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setLoadsImagesAutomatically=" + flag);
    // mAwSettings.setLoadsImagesAutomatically(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setLoadsImagesAutomatically=" + flag);
    }
    //mAwSettings->SetLoadsImagesAutomatically(flag);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::GetLoadsImagesAutomatically()
{
    // ==================before translated======================
    // return mAwSettings.getLoadsImagesAutomatically();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetLoadsImagesAutomatically();
    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetBlockNetworkImage(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setBlockNetworkImage=" + flag);
    // mAwSettings.setImagesEnabled(!flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setBlockNetworkImage=" + flag);
    }
    //mAwSettings->SetImagesEnabled(!flag);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::GetBlockNetworkImage()
{
    // ==================before translated======================
    // return !mAwSettings.getImagesEnabled();

    assert(0);
    AutoLock lock(this);
    //return !mAwSettings->GetImagesEnabled();
    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetBlockNetworkLoads(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setBlockNetworkLoads=" + flag);
    // mAwSettings.setBlockNetworkLoads(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setBlockNetworkLoads=" + flag);
    }
    //mAwSettings->SetBlockNetworkLoads(flag);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::GetBlockNetworkLoads()
{
    // ==================before translated======================
    // return mAwSettings.getBlockNetworkLoads();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetBlockNetworkLoads();
    return FALSE;
}

ECode ContentSettingsAdapter::SetJavaScriptEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setJavaScriptEnabled=" + flag);
    // mAwSettings.setJavaScriptEnabled(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setJavaScriptEnabled=" + flag);
    }
    //mAwSettings->SetJavaScriptEnabled(flag);
    return NOERROR;
}

ECode ContentSettingsAdapter::SetAllowUniversalAccessFromFileURLs(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAllowUniversalAccessFromFileURLs=" + flag);
    // mAwSettings.setAllowUniversalAccessFromFileURLs(flag);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setAllowUniversalAccessFromFileURLs=" + flag);
    }
    //mAwSettings->SetAllowUniversalAccessFromFileURLs(flag);
    return NOERROR;
}

ECode ContentSettingsAdapter::SetAllowFileAccessFromFileURLs(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAllowFileAccessFromFileURLs=" + flag);
    // mAwSettings.setAllowFileAccessFromFileURLs(flag);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setAllowFileAccessFromFileURLs=" + flag);
    }
    //mAwSettings->SetAllowFileAccessFromFileURLs(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetPluginsEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setPluginsEnabled=" + flag);
    // mAwSettings.setPluginsEnabled(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setPluginsEnabled=" + flag);
    }
    //mAwSettings->SetPluginsEnabled(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetPluginState(
    /* [in] */ Int32/*WebSettings::PluginState*/ state)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setPluginState=" + state);
    // mAwSettings.setPluginState(state);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setPluginState=" + state);
    }
    //mAwSettings->SetPluginState(state);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetDatabasePath(
    /* [in] */ const String& databasePath)
{
    // ==================before translated======================
    // // Intentional no-op.

    AutoLock lock(this);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetGeolocationDatabasePath(
    /* [in] */ const String& databasePath)
{
    // ==================before translated======================
    // // Intentional no-op.

    AutoLock lock(this);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetAppCacheEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAppCacheEnabled=" + flag);
    // mAwSettings.setAppCacheEnabled(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setAppCacheEnabled=" + flag);
    }
    //mAwSettings->SetAppCacheEnabled(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetAppCachePath(
    /* [in] */ const String& appCachePath)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAppCachePath=" + appCachePath);
    // mAwSettings.setAppCachePath(appCachePath);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setAppCachePath=" + appCachePath);
    }
    //mAwSettings->SetAppCachePath(appCachePath);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetAppCacheMaxSize(
    /* [in] */ Int64 appCacheMaxSize)
{
    // ==================before translated======================
    // // Intentional no-op.

    AutoLock lock(this);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetDatabaseEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setDatabaseEnabled=" + flag);
    // mAwSettings.setDatabaseEnabled(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setDatabaseEnabled=" + flag);
    }
    //mAwSettings->SetDatabaseEnabled(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetDomStorageEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setDomStorageEnabled=" + flag);
    // mAwSettings.setDomStorageEnabled(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setDomStorageEnabled=" + flag);
    }
    //mAwSettings.setDomStorageEnabled(flag);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::GetDomStorageEnabled()
{
    // ==================before translated======================
    // return mAwSettings.getDomStorageEnabled();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetDomStorageEnabled();
    return FALSE;
}

// synchronized
String ContentSettingsAdapter::GetDatabasePath()
{
    // ==================before translated======================
    // // Intentional no-op.
    // return "";

    AutoLock lock(this);
    return String("");
}

// synchronized
Boolean ContentSettingsAdapter::GetDatabaseEnabled()
{
    // ==================before translated======================
    // return mAwSettings.getDatabaseEnabled();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetDatabaseEnabled();
    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetGeolocationEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setGeolocationEnabled=" + flag);
    // mAwSettings.setGeolocationEnabled(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setGeolocationEnabled=" + flag);
    }
    //mAwSettings->SetGeolocationEnabled(flag);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::GetJavaScriptEnabled()
{
    // ==================before translated======================
    // return mAwSettings.getJavaScriptEnabled();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetJavaScriptEnabled();
    return FALSE;
}

Boolean ContentSettingsAdapter::GetAllowUniversalAccessFromFileURLs()
{
    // ==================before translated======================
    // return mAwSettings.getAllowUniversalAccessFromFileURLs();

    assert(0);
    //return mAwSettings->GetAllowUniversalAccessFromFileURLs();
    return FALSE;
}

Boolean ContentSettingsAdapter::GetAllowFileAccessFromFileURLs()
{
    // ==================before translated======================
    // return mAwSettings.getAllowFileAccessFromFileURLs();

    assert(0);
    //return mAwSettings->GetAllowFileAccessFromFileURLs();
    return FALSE;
}

// synchronized
Boolean ContentSettingsAdapter::GetPluginsEnabled()
{
    // ==================before translated======================
    // return mAwSettings.getPluginsEnabled();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetPluginsEnabled();
    return FALSE;
}

// synchronized
Int32/*WebSettings::PluginState*/ ContentSettingsAdapter::GetPluginState()
{
    // ==================before translated======================
    // return mAwSettings.getPluginState();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetPluginState();
    return -1;
}

// synchronized
ECode ContentSettingsAdapter::SetJavaScriptCanOpenWindowsAutomatically(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setJavaScriptCanOpenWindowsAutomatically=" + flag);
    // mAwSettings.setJavaScriptCanOpenWindowsAutomatically(flag);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setJavaScriptCanOpenWindowsAutomatically=" + flag);
    }
    //mAwSettings->SetJavaScriptCanOpenWindowsAutomatically(flag);
    return NOERROR;
}

// synchronized
Boolean ContentSettingsAdapter::GetJavaScriptCanOpenWindowsAutomatically()
{
    // ==================before translated======================
    // return mAwSettings.getJavaScriptCanOpenWindowsAutomatically();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetJavaScriptCanOpenWindowsAutomatically();
    return FALSE;
}

// synchronized
ECode ContentSettingsAdapter::SetDefaultTextEncodingName(
    /* [in] */ const String& encoding)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setDefaultTextEncodingName=" + encoding);
    // mAwSettings.setDefaultTextEncodingName(encoding);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setDefaultTextEncodingName=" + encoding);
    }
    //mAwSettings->SetDefaultTextEncodingName(encoding);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetDefaultTextEncodingName()
{
    // ==================before translated======================
    // return mAwSettings.getDefaultTextEncodingName();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetDefaultTextEncodingName();
    return String("");
}

// synchronized
ECode ContentSettingsAdapter::SetUserAgentString(
    /* [in] */ const String& ua)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setUserAgentString=" + ua);
    // mAwSettings.setUserAgentString(ua);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setUserAgentString=" + ua);
    }
    //mAwSettings->SetUserAgentString(ua);
    return NOERROR;
}

// synchronized
String ContentSettingsAdapter::GetUserAgentString()
{
    // ==================before translated======================
    // return mAwSettings.getUserAgentString();

    assert(0);
    AutoLock lock(this);
    //return mAwSettings->GetUserAgentString();
    return String("");
}

ECode ContentSettingsAdapter::SetNeedInitialFocus(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setNeedInitialFocus=" + flag);
    // mAwSettings.setShouldFocusFirstNode(flag);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setNeedInitialFocus=" + flag);
    }
    //mAwSettings->SetShouldFocusFirstNode(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetRenderPriority(
    /* [in] */ Int32/*RenderPriority*/ priority)
{
    // ==================before translated======================
    // // Intentional no-op.

    assert(0);
    AutoLock lock(this);
    return NOERROR;
}

ECode ContentSettingsAdapter::SetCacheMode(
    /* [in] */ Int32 mode)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setCacheMode=" + mode);
    // mAwSettings.setCacheMode(mode);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setCacheMode=" + mode);
    }
    //mAwSettings->SetCacheMode(mode);
    return NOERROR;
}

Int32 ContentSettingsAdapter::GetCacheMode()
{
    // ==================before translated======================
    // return mAwSettings.getCacheMode();

    assert(0);
    //return mAwSettings->GetCacheMode();
    return 0;
}

ECode ContentSettingsAdapter::SetMediaPlaybackRequiresUserGesture(
    /* [in] */ Boolean require)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setMediaPlaybackRequiresUserGesture=" + require);
    // mAwSettings.setMediaPlaybackRequiresUserGesture(require);

    assert(0);
    if (DebugFlags::TRACE_API) {
        //Log.d(LOGTAG, "setMediaPlaybackRequiresUserGesture=" + require);
    }
    //mAwSettings->SetMediaPlaybackRequiresUserGesture(require);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetMediaPlaybackRequiresUserGesture()
{
    // ==================before translated======================
    // return mAwSettings.getMediaPlaybackRequiresUserGesture();

    assert(0);
    //return mAwSettings->GetMediaPlaybackRequiresUserGesture();
    return FALSE;
}

ECode ContentSettingsAdapter::SetMixedContentMode(
    /* [in] */ Int32 mode)
{
    // ==================before translated======================
    // mAwSettings.setMixedContentMode(mode);

    assert(0);
    //mAwSettings->SetMixedContentMode(mode);
    return NOERROR;
}

Int32 ContentSettingsAdapter::GetMixedContentMode()
{
    // ==================before translated======================
    // return mAwSettings.getMixedContentMode();

    assert(0);
    //return mAwSettings->GetMixedContentMode();
    return 0;
}

ECode ContentSettingsAdapter::SetVideoOverlayForEmbeddedEncryptedVideoEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // mAwSettings.setVideoOverlayForEmbeddedVideoEnabled(flag);

    assert(0);
    //mAwSettings->SetVideoOverlayForEmbeddedVideoEnabled(flag);
    return NOERROR;
}

Boolean ContentSettingsAdapter::GetVideoOverlayForEmbeddedEncryptedVideoEnabled()
{
    // ==================before translated======================
    // return mAwSettings.getVideoOverlayForEmbeddedVideoEnabled();

    assert(0);
    //return mAwSettings->GetVideoOverlayForEmbeddedVideoEnabled();
    return FALSE;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


