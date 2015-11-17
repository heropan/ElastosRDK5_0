
#include "elastos/droid/webkit/webview/chromium/ContentSettingsAdapter.h"
#include "elastos/droid/webkit/webview/chromium/DebugFlags.h"
#include "elastos/core/AutoLock.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Webkit::Webview::Chromium::DebugFlags;
using Elastos::Core::AutoLock;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//                        ContentSettingsAdapter
//=====================================================================
const String ContentSettingsAdapter::LOGTAG("ContentSettingsAdapter");// = ContentSettingsAdapter.class.getSimpleName();

ContentSettingsAdapter::ContentSettingsAdapter()
    //: mAwSettings(NULL)
{
}

ContentSettingsAdapter::ContentSettingsAdapter(
    /* [in] */ AwSettings* awSettings)
    //: mAwSettings(awSettings)
{
    // ==================before translated======================
    // mAwSettings = awSettings;
}

ECode ContentSettingsAdapter::GetAwSettings(
    /* [out] */ AwSettings** awSettings)
{
    VALIDATE_NOT_NULL(awSettings);
    // ==================before translated======================
    // return mAwSettings;

    *awSettings = mAwSettings;
    REFCOUNT_ADD(*awSettings);
    return NOERROR;
}

ECode ContentSettingsAdapter::SetNavDump(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

ECode ContentSettingsAdapter::GetNavDump(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    *result = FALSE;
    return NOERROR;
}

ECode ContentSettingsAdapter::SetSupportZoom(
    /* [in] */ Boolean support)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setSupportZoom=" + support);
    // mAwSettings.setSupportZoom(support);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String suppportStr = (TRUE == support) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setSupportZoom=") + suppportStr);
    }
    mAwSettings->SetSupportZoom(support);
    return NOERROR;
}

ECode ContentSettingsAdapter::SupportZoom(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.supportZoom();

    assert(0);
    *result = mAwSettings->SupportZoom();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetBuiltInZoomControls(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setBuiltInZoomControls=" + enabled);
    // mAwSettings.setBuiltInZoomControls(enabled);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String enabledStr = (TRUE == enabled) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setBuiltInZoomControls=") + enabledStr);
    }
    mAwSettings->SetBuiltInZoomControls(enabled);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetBuiltInZoomControls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getBuiltInZoomControls();

    assert(0);
    *result = mAwSettings->GetBuiltInZoomControls();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetDisplayZoomControls(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setDisplayZoomControls=" + enabled);
    // mAwSettings.setDisplayZoomControls(enabled);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String enabledStr = (TRUE == enabled) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setDisplayZoomControls=") + enabledStr);
    }
    mAwSettings->SetDisplayZoomControls(enabled);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetDisplayZoomControls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getDisplayZoomControls();

    assert(0);
    *result = mAwSettings->GetDisplayZoomControls();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetAllowFileAccess(
    /* [in] */ Boolean allow)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAllowFileAccess=" + allow);
    // mAwSettings.setAllowFileAccess(allow);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String allowStr = (TRUE == allow) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setAllowFileAccess=") + allowStr);
    }
    mAwSettings->SetAllowFileAccess(allow);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetAllowFileAccess(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getAllowFileAccess();

    assert(0);
    *result = mAwSettings->GetAllowFileAccess();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetAllowContentAccess(
    /* [in] */ Boolean allow)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAllowContentAccess=" + allow);
    // mAwSettings.setAllowContentAccess(allow);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String allowStr = (TRUE == allow) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setAllowContentAccess=") + allowStr);
    }
    mAwSettings->SetAllowContentAccess(allow);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetAllowContentAccess(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getAllowContentAccess();

    assert(0);
    *result = mAwSettings->GetAllowContentAccess();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetLoadWithOverviewMode(
    /* [in] */ Boolean overview)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setLoadWithOverviewMode=" + overview);
    // mAwSettings.setLoadWithOverviewMode(overview);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String overviewStr = (TRUE == overview) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setLoadWithOverviewMode=") + overviewStr);
    }
    mAwSettings->SetLoadWithOverviewMode(overview);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetLoadWithOverviewMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getLoadWithOverviewMode();

    assert(0);
    *result = mAwSettings->GetLoadWithOverviewMode();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetAcceptThirdPartyCookies(
    /* [in] */ Boolean accept)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setAcceptThirdPartyCookies=" + accept);
    // mAwSettings.setAcceptThirdPartyCookies(accept);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String acceptStr = (TRUE == accept) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setAcceptThirdPartyCookies=") + acceptStr);
    }
    mAwSettings->SetAcceptThirdPartyCookies(accept);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetAcceptThirdPartyCookies(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getAcceptThirdPartyCookies();

    assert(0);
    *result = mAwSettings->GetAcceptThirdPartyCookies();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetEnableSmoothTransition(
    /* [in] */ Boolean enable)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

ECode ContentSettingsAdapter::EnableSmoothTransition(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    *result = FALSE;
    return NOERROR;
}

ECode ContentSettingsAdapter::SetUseWebViewBackgroundForOverscrollBackground(
    /* [in] */ Boolean view)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

ECode ContentSettingsAdapter::GetUseWebViewBackgroundForOverscrollBackground(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    *result = FALSE;
    return NOERROR;
}

ECode ContentSettingsAdapter::SetSaveFormData(
    /* [in] */ Boolean save)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setSaveFormData=" + save);
    // mAwSettings.setSaveFormData(save);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String saveStr = (TRUE == save) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setSaveFormData=") + saveStr);
    }
    mAwSettings->SetSaveFormData(save);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetSaveFormData(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getSaveFormData();

    assert(0);
    *result = mAwSettings->GetSaveFormData();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetSavePassword(
    /* [in] */ Boolean save)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

ECode ContentSettingsAdapter::GetSavePassword(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    *result = FALSE;
    return NOERROR;
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
        Logger::D(LOGTAG, "setTextZoom=%d", textZoom);
    }
    mAwSettings->SetTextZoom(textZoom);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetTextZoom(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getTextZoom();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetTextZoom();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetDefaultZoom(
    /* [in] */ ZoomDensity zoom)
{
    // ==================before translated======================
    // if (zoom != ZoomDensity.MEDIUM) {
    //     Log.w(LOGTAG, "setDefaultZoom not supported, zoom=" + zoom);
    // }

    assert(0);
    if (zoom != ZoomDensity_MEDIUM) {
        Logger::W(LOGTAG, "setDefaultZoom not supported, zoom=%d", zoom);
    }
    return NOERROR;
}

ECode ContentSettingsAdapter::GetDefaultZoom(
    /* [out] */ ZoomDensity* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Intentional no-op.
    // return ZoomDensity.MEDIUM;

    assert(0);
    *result = ZoomDensity_MEDIUM;
    return NOERROR;
}

ECode ContentSettingsAdapter::SetLightTouchEnabled(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // // Intentional no-op.

    return NOERROR;
}

ECode ContentSettingsAdapter::GetLightTouchEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Intentional no-op.
    // return false;

    *result = FALSE;
    return NOERROR;
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
        Logger::W(LOGTAG, "setUserAgent not supported, ua=%d", ua);
    }
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetUserAgent(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Minimal implementation for backwards compatibility: just identifies default vs custom.
    // return AwSettings.getDefaultUserAgent().equals(getUserAgentString()) ? 0 : -1;

    assert(0);
    AutoLock lock(this);
    // Minimal implementation for backwards compatibility: just identifies default vs custom.

    String userAgent;
    GetUserAgentString(&userAgent);
    *result = AwSettings::GetDefaultUserAgent().Equals(userAgent) ? 0 : -1;
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
        String useStr = (TRUE == use) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setUseWideViewPort=") + useStr);
    }
    mAwSettings->SetUseWideViewPort(use);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetUseWideViewPort(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getUseWideViewPort();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetUseWideViewPort();
    return NOERROR;
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
        String supportStr = (TRUE == support) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setSupportMultipleWindows=") + supportStr);
    }
    mAwSettings->SetSupportMultipleWindows(support);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SupportMultipleWindows(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.supportMultipleWindows();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->SupportMultipleWindows();
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetLayoutAlgorithm(
    /* [in] */ LayoutAlgorithm la)
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
    AwSettings::LayoutAlgorithm index = (AwSettings::LayoutAlgorithm)(la - LayoutAlgorithm_NORMAL);
    mAwSettings->SetLayoutAlgorithm(index);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetLayoutAlgorithm(
    /* [out] */ LayoutAlgorithm* result)
{
    VALIDATE_NOT_NULL(result);
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
    const LayoutAlgorithm webViewValues[] = {
        LayoutAlgorithm_NORMAL,
        LayoutAlgorithm_SINGLE_COLUMN,
        LayoutAlgorithm_NARROW_COLUMNS,
        LayoutAlgorithm_TEXT_AUTOSIZING
    };

    Int32 index = mAwSettings->GetLayoutAlgorithm() - AwSettings::NORMAL;
    *result = webViewValues[index];
    return NOERROR;
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
        Logger::D(LOGTAG, String("setStandardFontFamily=") + font);
    }
    mAwSettings->SetStandardFontFamily(font);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetStandardFontFamily(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getStandardFontFamily();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetStandardFontFamily();
    return NOERROR;
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
        Logger::D(LOGTAG, String("setFixedFontFamily=") + font);
    }
    mAwSettings->SetFixedFontFamily(font);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetFixedFontFamily(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getFixedFontFamily();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetFixedFontFamily();
    return NOERROR;
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
        Logger::D(LOGTAG, String("setSansSerifFontFamily=") + font);
    }
    mAwSettings->SetSansSerifFontFamily(font);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetSansSerifFontFamily(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getSansSerifFontFamily();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetSansSerifFontFamily();
    return NOERROR;
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
        Logger::D(LOGTAG, String("setSerifFontFamily=") + font);
    }
    mAwSettings->SetSerifFontFamily(font);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetSerifFontFamily(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getSerifFontFamily();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetSerifFontFamily();
    return NOERROR;
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
        Logger::D(LOGTAG, String("setCursiveFontFamily=") + font);
    }
    mAwSettings->SetCursiveFontFamily(font);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetCursiveFontFamily(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getCursiveFontFamily();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetCursiveFontFamily();
    return NOERROR;
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
        Logger::D(LOGTAG, String("setFantasyFontFamily=") + font);
    }
    mAwSettings->SetFantasyFontFamily(font);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetFantasyFontFamily(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getFantasyFontFamily();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetFantasyFontFamily();
    return NOERROR;
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
        Logger::D(LOGTAG, "setMinimumFontSize=%d", size);
    }
    mAwSettings->SetMinimumFontSize(size);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetMinimumFontSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getMinimumFontSize();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetMinimumFontSize();
    return NOERROR;
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
        Logger::D(LOGTAG, "setMinimumLogicalFontSize=%d", size);
    }
    mAwSettings->SetMinimumLogicalFontSize(size);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetMinimumLogicalFontSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getMinimumLogicalFontSize();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetMinimumLogicalFontSize();
    return NOERROR;
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
        Logger::D(LOGTAG, "setDefaultFontSize=%d", size);
    }
    mAwSettings->SetDefaultFontSize(size);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetDefaultFontSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getDefaultFontSize();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetDefaultFontSize();
    return NOERROR;
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
        Logger::D(LOGTAG, "setDefaultFixedFontSize=%d", size);
    }
    mAwSettings->SetDefaultFixedFontSize(size);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetDefaultFixedFontSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getDefaultFixedFontSize();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetDefaultFixedFontSize();
    return NOERROR;
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setLoadsImagesAutomatically=") + flagStr);
    }
    mAwSettings->SetLoadsImagesAutomatically(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetLoadsImagesAutomatically(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getLoadsImagesAutomatically();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetLoadsImagesAutomatically();
    return NOERROR;
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setBlockNetworkImage=") + flagStr);
    }
    mAwSettings->SetImagesEnabled(!flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetBlockNetworkImage(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return !mAwSettings.getImagesEnabled();

    assert(0);
    AutoLock lock(this);
    *result = !mAwSettings->GetImagesEnabled();
    return NOERROR;
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setBlockNetworkLoads=") + flagStr);
    }
    mAwSettings->SetBlockNetworkLoads(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetBlockNetworkLoads(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getBlockNetworkLoads();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetBlockNetworkLoads();
    return NOERROR;
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setJavaScriptEnabled=") + flagStr);
    }
    mAwSettings->SetJavaScriptEnabled(flag);
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setAllowUniversalAccessFromFileURLs=") + flagStr);
    }
    mAwSettings->SetAllowUniversalAccessFromFileURLs(flag);
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setAllowFileAccessFromFileURLs=") + flagStr);
    }
    mAwSettings->SetAllowFileAccessFromFileURLs(flag);
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setPluginsEnabled=") + flagStr);
    }
    mAwSettings->SetPluginsEnabled(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetPluginState(
    /* [in] */ PluginState state)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setPluginState=" + state);
    // mAwSettings.setPluginState(state);

    assert(0);
    AutoLock lock(this);
    if (DebugFlags::TRACE_API) {
        Logger::D(LOGTAG, "setPluginState=%d", (Int32)state);
    }
    mAwSettings->SetPluginState(state);
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setAppCacheEnabled=") + flagStr);
    }
    mAwSettings->SetAppCacheEnabled(flag);
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
        Logger::D(LOGTAG, String("setAppCachePath=") + appCachePath);
    }
    mAwSettings->SetAppCachePath(appCachePath);
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setDatabaseEnabled=") + flagStr);
    }
    mAwSettings->SetDatabaseEnabled(flag);
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setDomStorageEnabled=") + flagStr);
    }
    mAwSettings->SetDomStorageEnabled(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetDomStorageEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getDomStorageEnabled();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetDomStorageEnabled();
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetDatabasePath(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Intentional no-op.
    // return "";

    AutoLock lock(this);
    *result = String("");
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetDatabaseEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getDatabaseEnabled();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetDatabaseEnabled();
    return NOERROR;
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setGeolocationEnabled=") + flagStr);
    }
    mAwSettings->SetGeolocationEnabled(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetJavaScriptEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getJavaScriptEnabled();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetJavaScriptEnabled();
    return NOERROR;
}

ECode ContentSettingsAdapter::GetAllowUniversalAccessFromFileURLs(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getAllowUniversalAccessFromFileURLs();

    assert(0);
    *result = mAwSettings->GetAllowUniversalAccessFromFileURLs();
    return NOERROR;
}

ECode ContentSettingsAdapter::GetAllowFileAccessFromFileURLs(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getAllowFileAccessFromFileURLs();

    assert(0);
    *result = mAwSettings->GetAllowFileAccessFromFileURLs();
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetPluginsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getPluginsEnabled();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetPluginsEnabled();
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetPluginState(
    /* [out] */ PluginState* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getPluginState();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetPluginState();
    return NOERROR;
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
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setJavaScriptCanOpenWindowsAutomatically=") + flagStr);
    }
    mAwSettings->SetJavaScriptCanOpenWindowsAutomatically(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetJavaScriptCanOpenWindowsAutomatically(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getJavaScriptCanOpenWindowsAutomatically();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetJavaScriptCanOpenWindowsAutomatically();
    return NOERROR;
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
        Logger::D(LOGTAG, String("setDefaultTextEncodingName=") + encoding);
    }
    mAwSettings->SetDefaultTextEncodingName(encoding);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetDefaultTextEncodingName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getDefaultTextEncodingName();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetDefaultTextEncodingName();
    return NOERROR;
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
        Logger::D(LOGTAG, String("setUserAgentString=") + ua);
    }
    mAwSettings->SetUserAgentString(ua);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::GetUserAgentString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getUserAgentString();

    assert(0);
    AutoLock lock(this);
    *result = mAwSettings->GetUserAgentString();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetNeedInitialFocus(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setNeedInitialFocus=" + flag);
    // mAwSettings.setShouldFocusFirstNode(flag);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String flagStr = (TRUE == flag) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setNeedInitialFocus=") + flagStr);
    }
    mAwSettings->SetShouldFocusFirstNode(flag);
    return NOERROR;
}

// synchronized
ECode ContentSettingsAdapter::SetRenderPriority(
    /* [in] */ RenderPriority priority)
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
        Logger::D(LOGTAG, "setCacheMode=%d", mode);
    }
    mAwSettings->SetCacheMode(mode);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetCacheMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getCacheMode();

    assert(0);
    *result = mAwSettings->GetCacheMode();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetMediaPlaybackRequiresUserGesture(
    /* [in] */ Boolean require)
{
    // ==================before translated======================
    // if (DebugFlags.TRACE_API) Log.d(LOGTAG, "setMediaPlaybackRequiresUserGesture=" + require);
    // mAwSettings.setMediaPlaybackRequiresUserGesture(require);

    assert(0);
    if (DebugFlags::TRACE_API) {
        String requireStr = (TRUE == require) ? String("TRUE") : String("FALSE");
        Logger::D(LOGTAG, String("setMediaPlaybackRequiresUserGesture=") + requireStr);
    }
    mAwSettings->SetMediaPlaybackRequiresUserGesture(require);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetMediaPlaybackRequiresUserGesture(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getMediaPlaybackRequiresUserGesture();

    assert(0);
    *result = mAwSettings->GetMediaPlaybackRequiresUserGesture();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetMixedContentMode(
    /* [in] */ Int32 mode)
{
    // ==================before translated======================
    // mAwSettings.setMixedContentMode(mode);

    assert(0);
    mAwSettings->SetMixedContentMode(mode);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetMixedContentMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getMixedContentMode();

    assert(0);
    *result = mAwSettings->GetMixedContentMode();
    return NOERROR;
}

ECode ContentSettingsAdapter::SetVideoOverlayForEmbeddedEncryptedVideoEnabled(
    /* [in] */ Boolean flag)
{
    // ==================before translated======================
    // mAwSettings.setVideoOverlayForEmbeddedVideoEnabled(flag);

    assert(0);
    mAwSettings->SetVideoOverlayForEmbeddedVideoEnabled(flag);
    return NOERROR;
}

ECode ContentSettingsAdapter::GetVideoOverlayForEmbeddedEncryptedVideoEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAwSettings.getVideoOverlayForEmbeddedVideoEnabled();

    assert(0);
    *result = mAwSettings->GetVideoOverlayForEmbeddedVideoEnabled();
    return NOERROR;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


