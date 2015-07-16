
#include "os/Build.h"
#include "os/Process.h"
#include "os/CMessageHelper.h"
#include "provider/CSettingsSystem.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/DebugFlags.h"
#include "webkit/WebSettingsClassic.h"
#include "webkit/CAutoFillProfile.h"
#include "R.h"
#include "Manifest.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuffer.h>
#include <elastos/core/Character.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elastos/utility/logging/Logger.h>
#include "webkit/DebugFlags.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuilder;
using Elastos::Core::Character;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::ISharedPreferencesEditor;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Provider::CSettingsSystem;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

ElaWebSettingsCallback* WebSettingsClassic::Init_WebSettingsCallback()
{
    if (DebugFlags::WEB_SETTINGS)
        Logger::V(LOGTAG, "enter: Init_WebSettingsCallback");
    static struct ElaWebSettingsCallback t =
    {
        &WebSettingsClassic::GetLayoutAlgorithm,
        &WebSettingsClassic::GetTextSize,
        &WebSettingsClassic::GetStandardFontFamily,
        &WebSettingsClassic::GetFixedFontFamily,
        &WebSettingsClassic::GetSansSerifFontFamily,
        &WebSettingsClassic::GetSerifFontFamily,
        &WebSettingsClassic::GetCursiveFontFamily,
        &WebSettingsClassic::GetFantasyFontFamily,
        &WebSettingsClassic::GetDefaultTextEncoding,
        &WebSettingsClassic::GetUserAgentString,
        &WebSettingsClassic::GetAcceptLanguage,
        &WebSettingsClassic::GetMinimumFontSize,
        &WebSettingsClassic::GetMinimumLogicalFontSize,
        &WebSettingsClassic::GetDefaultFontSize,
        &WebSettingsClassic::GetDefaultFixedFontSize,
        &WebSettingsClassic::GetLoadsImagesAutomatically,
        &WebSettingsClassic::GetBlockNetworkImage,

        &WebSettingsClassic::GetBlockNetworkLoads,
        &WebSettingsClassic::GetJavaScriptEnabled,
        &WebSettingsClassic::GetAllowUniversalAccessFromFileURLs,
        &WebSettingsClassic::GetAllowFileAccessFromFileURLs,
        &WebSettingsClassic::GetPluginState,

        &WebSettingsClassic::GetAppCacheEnabled,
        &WebSettingsClassic::GetAppCachePath,
        &WebSettingsClassic::GetAppCacheMaxSize,
        &WebSettingsClassic::GetWorkersEnabled,
        &WebSettingsClassic::GetJavaScriptCanOpenWindowsAutomatically,
        &WebSettingsClassic::GetUseWideViewport,
        &WebSettingsClassic::GetSupportMultipleWindows,
        &WebSettingsClassic::GetShrinksStandaloneImagesToFit,
        &WebSettingsClassic::GetMaximumDecodedImageSize,
        &WebSettingsClassic::GetPrivateBrowsingEnabled,
        &WebSettingsClassic::GetSyntheticLinksEnabled,
        &WebSettingsClassic::GetUseDoubleTree,
        &WebSettingsClassic::GetPageCacheCapacity,
        &WebSettingsClassic::GetDatabaseEnabled,
        &WebSettingsClassic::GetDomStorageEnabled,
        &WebSettingsClassic::GetGeolocationEnabled,
        &WebSettingsClassic::GetGeolocationDatabasePath,
        &WebSettingsClassic::GetXSSAuditorEnabled,
        &WebSettingsClassic::GetLinkPrefetchEnabled,
        &WebSettingsClassic::GetDatabasePath,
        &WebSettingsClassic::GetDatabasePathHasBeenSet,
        &WebSettingsClassic::GetAutoFillEnabled,
        &WebSettingsClassic::GetAutoFillProfile,
        &WebSettingsClassic::GetAutoFillProfileFullName,
        &WebSettingsClassic::GetAutoFillProfileEmailAddress,
        &WebSettingsClassic::GetAutoFillProfileCompanyName,
        &WebSettingsClassic::GetAutoFillProfileAddressLine1,
        &WebSettingsClassic::GetAutoFillProfileAddressLine2,
        &WebSettingsClassic::GetAutoFillProfileCity,
        &WebSettingsClassic::GetAutoFillProfileState,
        &WebSettingsClassic::GetAutoFillProfileZipCode,
        &WebSettingsClassic::GetAutoFillProfileCountry,
        &WebSettingsClassic::GetAutoFillProfilePhoneNumber,
        &WebSettingsClassic::GetOverrideCacheMode,
        &WebSettingsClassic::GetPasswordEchoEnabled,
        &WebSettingsClassic::GetMediaPlaybackRequiresUserGesture,
    };
    Elastos_WebSettings_InitCallback((Int32)&t);
    return &t;
}
struct ElaWebSettingsCallback* WebSettingsClassic::sElaWebSettingsCallback = WebSettingsClassic::Init_WebSettingsCallback();

//===============================================================
//         WebSettingsClassic::EventHandler::InnerHandler
//===============================================================
WebSettingsClassic::EventHandler::InnerHandler::InnerHandler(
    /* [in] */ EventHandler* owner)
    : mOwner(owner)
{}

ECode WebSettingsClassic::EventHandler::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case SYNC: {
                Mutex::Autolock lock(mOwner->mOwner->mLock);
                if (mOwner->mOwner->mBrowserFrame->mNativeFrame != 0) {
                    mOwner->mOwner->NativeSync(mOwner->mOwner->mBrowserFrame->mNativeFrame);
                }
                mOwner->mOwner->mSyncPending = FALSE;
            }
            break;

        case PRIORITY: {
            mOwner->SetRenderPriority();
            break;
        }

        case SET_DOUBLE_TAP_TOAST_COUNT: {
            AutoPtr<ISharedPreferences> sharedPreferences;
            mOwner->mOwner->mContext->GetSharedPreferences(PREF_FILE,
                            IContext::MODE_PRIVATE, (ISharedPreferences**)&sharedPreferences);
            AutoPtr<ISharedPreferencesEditor> editor;
            sharedPreferences->Edit((ISharedPreferencesEditor**)&editor);
            editor->PutInt32(DOUBLE_TAP_TOAST_COUNT,
                    sDoubleTapToastCount);
            Boolean result = FALSE;
            editor->Commit(&result);
            break;
        }
    }
    return NOERROR;
}

//===============================================================
//                WebSettingsClassic::EventHandler
//===============================================================
const Int32 WebSettingsClassic::EventHandler::SYNC;
const Int32 WebSettingsClassic::EventHandler::PRIORITY;
const Int32 WebSettingsClassic::EventHandler::SET_DOUBLE_TAP_TOAST_COUNT;

WebSettingsClassic::EventHandler::EventHandler(
    /* [in] */ WebSettingsClassic* owner)
    : mOwner(owner)
{}

void WebSettingsClassic::EventHandler::CreateHandler()
{
    Mutex::Autolock lock(mLock);

    // as mRenderPriority can be set before thread is running, sync up
    SetRenderPriority();

    mHandler = new InnerHandler(this);
}

void WebSettingsClassic::EventHandler::SetRenderPriority()
{
    Mutex::Autolock lock(mOwner->mLock);

    if (mOwner->mRenderPriority == RenderPriority_NORMAL) {
        Process::SetThreadPriority(
                IProcess::THREAD_PRIORITY_DEFAULT);
    }
    else if (mOwner->mRenderPriority == RenderPriority_HIGH) {
        Process::SetThreadPriority(
                IProcess::THREAD_PRIORITY_FOREGROUND +
                IProcess::THREAD_PRIORITY_LESS_FAVORABLE);
    }
    else if (mOwner->mRenderPriority == RenderPriority_LOW) {
        Process::SetThreadPriority(
                IProcess::THREAD_PRIORITY_BACKGROUND);
    }
}

/**
 * Send a message to the private queue or handler.
 */
Boolean WebSettingsClassic::EventHandler::SendMessage(
    /* [in] */ IMessage* msg)
{
    Mutex::Autolock lock(mLock);

    if (mHandler != NULL) {
          Boolean result = FALSE;
          mHandler->SendMessage(msg, &result);
          return TRUE;
      }
      else {
          return FALSE;
      }
}

//===============================================================
//                    WebSettingsClassic
//===============================================================
// TODO: Keep this up to date
const String WebSettingsClassic::PREVIOUS_VERSION("4.1.1");
const String WebSettingsClassic::LOGTAG("WebSettingsClassic");

// private WebSettings, not accessible by the host activity
Int32 WebSettingsClassic::sDoubleTapToastCount = 3;

const String WebSettingsClassic::PREF_FILE("WebViewSettings");
const String WebSettingsClassic::DOUBLE_TAP_TOAST_COUNT("double_tap_toast_count");

// User agent strings.
const String WebSettingsClassic::DESKTOP_USERAGENT("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.34 Safari/534.24");

const String WebSettingsClassic::IPHONE_USERAGENT("Mozilla/5.0 (iPhone; U; CPU iPhone OS 3_0 like Mac OS X; en-us) AppleWebKit/528.18 (KHTML, like Gecko) Version/4.0 Mobile/7A341 Safari/528.16");

AutoPtr<ILocale> WebSettingsClassic::sLocale;
Mutex WebSettingsClassic::sLockForLocaleSettings;

const String WebSettingsClassic::ACCEPT_LANG_FOR_US_LOCALE("en-US");

/**
 * Package constructor to prevent clients from creating a new settings
 * instance.
 */
WebSettingsClassic::WebSettingsClassic(
    /* [in] */ IContext* context,
    /* [in] */ CWebViewClassic* webview)
    : mWebView(webview)
    , mSyncPending(FALSE)
    , mLayoutAlgorithm(LayoutAlgorithm_NARROW_COLUMNS)
    , mContext(context)
    , mTextSize(100)
    , mStandardFontFamily("sans-serif")
    , mFixedFontFamily("monospace")
    , mSansSerifFontFamily("sans-serif")
    , mSerifFontFamily("serif")
    , mCursiveFontFamily("cursive")
    , mFantasyFontFamily("fantasy")
    , mUseDefaultUserAgent(FALSE)
    , mMinimumFontSize(8)
    , mMinimumLogicalFontSize(8)
    , mDefaultFontSize(16)
    , mDefaultFixedFontSize(13)
    , mPageCacheCapacity(0)
    , mLoadsImagesAutomatically(TRUE)
    , mBlockNetworkImage(FALSE)
    , mBlockNetworkLoads(FALSE)
    , mJavaScriptEnabled(FALSE)
    , mAllowUniversalAccessFromFileURLs(FALSE)
    , mAllowFileAccessFromFileURLs(FALSE)
    , mHardwareAccelSkia(FALSE)
    , mShowVisualIndicator(FALSE)
    , mPluginState(PluginState_OFF)
    , mJavaScriptCanOpenWindowsAutomatically(FALSE)
    , mUseDoubleTree(FALSE)
    , mUseWideViewport(FALSE)
    , mSupportMultipleWindows(FALSE)
    , mShrinksStandaloneImagesToFit(FALSE)
    , mMaximumDecodedImageSize(0)
    , mPrivateBrowsingEnabled(FALSE)
    , mSyntheticLinksEnabled(TRUE)
    , mAppCacheEnabled(FALSE)
    , mDatabaseEnabled(FALSE)
    , mDomStorageEnabled(FALSE)
    , mWorkersEnabled(FALSE)
    , mGeolocationEnabled(TRUE)
    , mXSSAuditorEnabled(FALSE)
    , mLinkPrefetchEnabled(FALSE)
    , mAppCacheMaxSize(Elastos::Core::Math::INT64_MAX_VALUE)
    , mDatabasePath("")
    , mDatabasePathHasBeenSet(FALSE)
    , mGeolocationDatabasePath("")
    , mDefaultZoom(ZoomDensity_MEDIUM)
    , mRenderPriority(RenderPriority_NORMAL)
    , mOverrideCacheMode(IWebSettings::LOAD_DEFAULT)
    , mDoubleTapZoom(100)
    , mSaveFormData(TRUE)
    , mAutoFillEnabled(FALSE)
    , mSavePassword(TRUE)
    , mLightTouchEnabled(FALSE)
    , mNeedInitialFocus(TRUE)
    , mNavDump(FALSE)
    , mSupportZoom(TRUE)
    , mMediaPlaybackRequiresUserGesture(TRUE)
    , mBuiltInZoomControls(FALSE)
    , mDisplayZoomControls(TRUE)
    , mAllowFileAccess(TRUE)
    , mAllowContentAccess(TRUE)
    , mLoadWithOverviewMode(FALSE)
    , mEnableSmoothTransition(FALSE)
    , mForceUserScalable(FALSE)
    , mPasswordEchoEnabled(TRUE)
    , mUseWebViewBackgroundForOverscroll(TRUE)
{
    mEventHandler = new EventHandler(this);
    context->GetString(R::string::default_text_encoding, &mDefaultTextEncoding);

    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    helper->GetDefault((ILocale**)&sLocale);
    mAcceptLanguage = GetCurrentAcceptLanguage();
    mUserAgent = GetCurrentUserAgent();
    mUseDefaultUserAgent = TRUE;

    Int32 permission;
    mContext->CheckPermission(
            Elastos::Droid::Manifest::Permission::INTERNET, Process::MyPid(),
            Process::MyUid(), &permission);
    mBlockNetworkLoads = permission != IPackageManager::PERMISSION_GRANTED;

    // SDK specific settings. See issue 6212665
    AutoPtr<IApplicationInfo> appInfo;
    mContext->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 sdkVersion;
    appInfo->GetTargetSdkVersion(&sdkVersion);
    if (sdkVersion < Build::VERSION_CODES::JELLY_BEAN) {
        mAllowUniversalAccessFromFileURLs = TRUE;
        mAllowFileAccessFromFileURLs = TRUE;
    }
    //try {
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsSystem> settingSystem;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingSystem);
    Int32 Int;
    settingSystem->GetInt32(resolver, ISettingsSystem::TEXT_SHOW_PASSWORD, &Int);
    mPasswordEchoEnabled = Int != 0;
    //} catch (SettingNotFoundException e) {
    //    mPasswordEchoEnabled = true;
    //}
}

CAR_INTERFACE_IMPL(WebSettingsClassic, IWebSettingsClassic);

/**
 * Looks at sLocale and returns current AcceptLanguage String.
 * @return Current AcceptLanguage String.
 */
String WebSettingsClassic::GetCurrentAcceptLanguage()
{
    AutoPtr<ILocale> locale;
    {
        Mutex::Autolock lock(sLockForLocaleSettings);
        locale = sLocale;
    }
    StringBuilder buffer;
    AddLocaleToHttpAcceptLanguage(buffer, locale);

    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> US;
    helper->GetUS((ILocale**)&US);
    Boolean result;
    if (US->Equals(locale, &result), !result) {
        if (buffer.GetLength() > 0) {
            buffer.AppendCStr(", ");
        }
        buffer.AppendString(ACCEPT_LANG_FOR_US_LOCALE);
    }

    return buffer.ToString();
}

/**
 * Convert obsolete language codes, including Hebrew/Indonesian/Yiddish,
 * to new standard.
 */
String WebSettingsClassic::ConvertObsoleteLanguageCodeToNew(
    /* [in] */ const String& langCode)
{
    if (langCode.IsNull()) {
        return String(NULL);
    }
    if (langCode.Equals("iw")) {
        // Hebrew
        return String("he");
    }
    else if (langCode.Equals("in")) {
        // Indonesian
        return String("id");
    }
    else if (langCode.Equals("ji")) {
        // Yiddish
        return String("yi");
    }
    return langCode;
}

void WebSettingsClassic::AddLocaleToHttpAcceptLanguage(
    /* [in] */ StringBuilder& builder,
    /* [in] */ ILocale* locale)
{
    String lang;
    locale->GetLanguage(&lang);
    String language = ConvertObsoleteLanguageCodeToNew(lang);
    if (!language.IsNull()) {
        builder.AppendString(language);
        String country;
        locale->GetCountry(&country);
        if (!country.IsNull()) {
            builder.AppendCStr("-");
            builder.AppendString(country);
        }
    }
}

/**
 * Looks at sLocale and mContext and returns current UserAgent String.
 * @return Current UserAgent String.
 */
String WebSettingsClassic::GetCurrentUserAgent()
{
    Mutex::Autolock lock(mLock);

    AutoPtr<ILocale> locale;
    {
        Mutex::Autolock lock(sLockForLocaleSettings);
        locale = sLocale;
    }
    return GetDefaultUserAgentForLocale(mContext, locale);
}

/**
 * Returns the default User-Agent used by a WebView.
 * An instance of WebView could use a different User-Agent if a call
 * is made to {@link WebSettings#setUserAgent(int)} or
 * {@link WebSettings#setUserAgentString(String)}.
 *
 * @param context a Context object used to access application assets
 * @param locale The Locale to use in the User-Agent string.
 * @see WebViewFactoryProvider#getDefaultUserAgent(Context)
 * @see WebView#getDefaultUserAgent(Context)
 */
String WebSettingsClassic::GetDefaultUserAgentForLocale(
    /* [in] */  IContext* context,
    /* [in] */  ILocale* locale)
{
    Elastos::Core::StringBuffer buffer;
    // Add version
    const String version = Build::VERSION::RELEASE;
    if (version.GetLength() > 0) {
        if (Character::IsDigit(version.GetChar(0))) {
            // Release is a version, eg "3.1"
            buffer.AppendString(version);
        }
        else {
            // Release is a codename, eg "Honeycomb"
            // In this case, use the previous release's version
            buffer.AppendString(WebSettingsClassic::PREVIOUS_VERSION);
        }
    }
    else {
        // default to "1.0"
        buffer.AppendCStr("1.0");
    }
    buffer.AppendCStr("; ");
    String language;
    locale->GetLanguage(&language);
    if (!language.IsNull()) {
        buffer.AppendString(ConvertObsoleteLanguageCodeToNew(language));
        String country;
        locale->GetCountry(&country);
        if (!country.IsNull()) {
            buffer.AppendCStr("-");
            buffer.AppendString(country.ToLowerCase());
        }
    }
    else {
        // default to "en"
        buffer.AppendCStr("en");
    }
    buffer.AppendCStr(";");
    // add the model for the release build
    if (CString("REL").Equals(Build::VERSION::CODENAME)) {
        const String model = Build::MODEL;
        if (model.GetLength() > 0) {
            buffer.AppendCStr(" ");
            buffer.AppendString(model);
        }
    }
    const String id = Build::ID;
    if (id.GetLength() > 0) {
        buffer.AppendCStr(" Build/");
        buffer.AppendString(id);
    }
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<ICharSequence> cs;
    res->GetText(
        R::string::web_user_agent_target_content, (ICharSequence**)&cs);
    String mobile;
    cs->ToString(&mobile);
    cs = NULL;
    res->GetText(
            R::string::web_user_agent, (ICharSequence**)&cs);
    String base;
    cs->ToString(&base);
    // TODO
//    return String.format(base, buffer, mobile);
    String result("");
    result.AppendFormat(base.string(), buffer.ToString().string(), mobile.string());
    return result;
}

/**
 * @see android.webkit.WebSettings#setNavDump(boolean)
 */
ECode WebSettingsClassic::SetNavDump(
    /* [in] */ Boolean enabled)
{
    mNavDump = enabled;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getNavDump()
 */
Boolean WebSettingsClassic::GetNavDump()
{
    return mNavDump;
}

/**
 * @see android.webkit.WebSettings#setSupportZoom(boolean)
 */
ECode WebSettingsClassic::SetSupportZoom(
    /* [in] */ Boolean support)
{
    mSupportZoom = support;
    mWebView->UpdateMultiTouchSupport(mContext);
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#supportZoom()
 */
Boolean WebSettingsClassic::SupportZoom()
{
    return mSupportZoom;
}

/**
 * @see android.webkit.WebSettings#setMediaPlaybackRequiresUserGesture(boolean)
 */
ECode WebSettingsClassic::SetMediaPlaybackRequiresUserGesture(
    /* [in] */ Boolean support)
{
    if (mMediaPlaybackRequiresUserGesture != support) {
        mMediaPlaybackRequiresUserGesture = support;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getMediaPlaybackRequiresUserGesture()
 */
Boolean WebSettingsClassic::GetMediaPlaybackRequiresUserGesture()
{
    return mMediaPlaybackRequiresUserGesture;
}

/**
 * @see android.webkit.WebSettings#setBuiltInZoomControls(boolean)
 */
ECode WebSettingsClassic::SetBuiltInZoomControls(
    /* [in] */ Boolean enabled)
{
    mBuiltInZoomControls = enabled;
    mWebView->UpdateMultiTouchSupport(mContext);
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getBuiltInZoomControls()
 */
Boolean WebSettingsClassic::GetBuiltInZoomControls()
{
    return mBuiltInZoomControls;
}

/**
 * @see android.webkit.WebSettings#setDisplayZoomControls(boolean)
 */
ECode WebSettingsClassic::SetDisplayZoomControls(
    /* [in] */ Boolean enabled)
{
    mDisplayZoomControls = enabled;
    mWebView->UpdateMultiTouchSupport(mContext);
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getDisplayZoomControls()
 */
Boolean WebSettingsClassic::GetDisplayZoomControls()
{
    return mDisplayZoomControls;
}

/**
 * @see android.webkit.WebSettings#setAllowFileAccess(boolean)
 */
ECode WebSettingsClassic::SetAllowFileAccess(
    /* [in] */ Boolean allow)
{
    mAllowFileAccess = allow;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getAllowFileAccess()
 */
Boolean WebSettingsClassic::GetAllowFileAccess()
{
    return mAllowFileAccess;
}

/**
 * @see android.webkit.WebSettings#setAllowContentAccess(boolean)
 */
ECode WebSettingsClassic::SetAllowContentAccess(
    /* [in] */ Boolean allow)
{
    mAllowContentAccess = allow;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getAllowContentAccess()
 */
Boolean WebSettingsClassic::GetAllowContentAccess()
{
    return mAllowContentAccess;
}

/**
 * @see android.webkit.WebSettings#setLoadWithOverviewMode(boolean)
 */
ECode WebSettingsClassic::SetLoadWithOverviewMode(
    /* [in] */ Boolean overview)
{
    mLoadWithOverviewMode = overview;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getLoadWithOverviewMode()
 */
Boolean WebSettingsClassic::GetLoadWithOverviewMode()
{
    return mLoadWithOverviewMode;
}

/**
 * @see android.webkit.WebSettings#setEnableSmoothTransition(boolean)
 */
ECode WebSettingsClassic::SetEnableSmoothTransition(
    /* [in] */ Boolean enable)
{
    mEnableSmoothTransition = enable;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#enableSmoothTransition()
 */
Boolean WebSettingsClassic::EnableSmoothTransition()
{
    return mEnableSmoothTransition;
}

/**
 * @see android.webkit.WebSettings#setUseWebViewBackgroundForOverscrollBackground(boolean)
 */
ECode WebSettingsClassic::SetUseWebViewBackgroundForOverscrollBackground(
    /* [in] */ Boolean view)
{
    mUseWebViewBackgroundForOverscroll = view;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getUseWebViewBackgroundForOverscrollBackground()
 */
Boolean WebSettingsClassic::GetUseWebViewBackgroundForOverscrollBackground()
{
    return mUseWebViewBackgroundForOverscroll;
}

/**
 * @see android.webkit.WebSettings#setSaveFormData(boolean)
 */
ECode WebSettingsClassic::SetSaveFormData(
    /* [in] */ Boolean save)
{
    mSaveFormData = save;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getSaveFormData()
 */
Boolean WebSettingsClassic::GetSaveFormData()
{
    return mSaveFormData && !mPrivateBrowsingEnabled;
}

/**
 * @see android.webkit.WebSettings#setSavePassword(boolean)
 */
ECode WebSettingsClassic::SetSavePassword(
    /* [in] */ Boolean save)
{
    mSavePassword = save;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getSavePassword()
 */
Boolean WebSettingsClassic::GetSavePassword()
{
    return mSavePassword;
}

/**
 * @see android.webkit.WebSettings#setTextZoom(int)
 */
ECode WebSettingsClassic::SetTextZoom(
    /* [in] */ Int32 textZoom)
{
    if (mTextSize != textZoom) {
        if (CWebViewClassic::mLogEvent) {
//            EventLog.writeEvent(EventLogTags.BROWSER_TEXT_SIZE_CHANGE,
//                    mTextSize, textZoom);
        }
        mTextSize = textZoom;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getTextZoom()
 */
Int32 WebSettingsClassic::GetTextZoom()
{
    return mTextSize;
}

/**
 * Set the double-tap zoom of the page in percent. Default is 100.
 * @param doubleTapZoom A percent value for increasing or decreasing the double-tap zoom.
 */
ECode WebSettingsClassic::SetDoubleTapZoom(
    /* [in] */ Int32 doubleTapZoom)
{
    if (mDoubleTapZoom != doubleTapZoom) {
        mDoubleTapZoom = doubleTapZoom;
        mWebView->UpdateDoubleTapZoom(doubleTapZoom);
    }
    return NOERROR;
}

/**
 * Get the double-tap zoom of the page in percent.
 * @return A percent value describing the double-tap zoom.
 */
ECode WebSettingsClassic::GetDoubleTapZoom(
    /* [out] */ Int32* zoom)
{
    VALIDATE_NOT_NULL(zoom);
    *zoom = mDoubleTapZoom;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setDefaultZoom(android.webkit.WebSettingsClassic.ZoomDensity)
 */
ECode WebSettingsClassic::SetDefaultZoom(
    /* [in] */ ZoomDensity zoom)
{
    if (mDefaultZoom != zoom) {
        mDefaultZoom = zoom;
        mWebView->AdjustDefaultZoomDensity(zoom);
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getDefaultZoom()
 */
ZoomDensity WebSettingsClassic::GetDefaultZoom()
{
    return mDefaultZoom;
}

/**
 * @see android.webkit.WebSettings#setLightTouchEnabled(boolean)
 */
ECode WebSettingsClassic::SetLightTouchEnabled(
    /* [in] */ Boolean enabled)
{
    mLightTouchEnabled = enabled;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getLightTouchEnabled()
 */
Boolean WebSettingsClassic::GetLightTouchEnabled()
{
    return mLightTouchEnabled;
}

/**
 * @see android.webkit.WebSettings#setUseDoubleTree(boolean)
 */
ECode WebSettingsClassic::SetUseDoubleTree(
    /* [in] */ Boolean use)
{
    Mutex::Autolock lock(mLock);
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getUseDoubleTree()
 */
Boolean WebSettingsClassic::GetUseDoubleTree()
{
    Mutex::Autolock lock(mLock);
    return FALSE;
}

/**
 * @see android.webkit.WebSettings#setUserAgent(int)
 */
ECode WebSettingsClassic::SetUserAgent(
    /* [in] */ Int32 ua)
{
    Mutex::Autolock lock(mLock);

    String uaString;
    if (ua == 1) {
        if (DESKTOP_USERAGENT.Equals(mUserAgent)) {
            return NOERROR; // do nothing
        }
        else {
            uaString = DESKTOP_USERAGENT;
        }
    }
    else if (ua == 2) {
        if (IPHONE_USERAGENT.Equals(mUserAgent)) {
            return NOERROR; // do nothing
        }
        else {
            uaString = IPHONE_USERAGENT;
        }
    }
    else if (ua != 0) {
        return NOERROR; // do nothing
    }
    return SetUserAgentString(uaString);
}

/**
 * @see android.webkit.WebSettings#getUserAgent()
 */
Int32 WebSettingsClassic::GetUserAgent()
{
    Mutex::Autolock lock(mLock);

    if (DESKTOP_USERAGENT.Equals(mUserAgent)) {
        return 1;
    }
    else if (IPHONE_USERAGENT.Equals(mUserAgent)) {
        return 2;
    }
    else if (mUseDefaultUserAgent) {
        return 0;
    }
    return -1;
}

/**
 * @see android.webkit.WebSettings#setUseWideViewPort(boolean)
 */
ECode WebSettingsClassic::SetUseWideViewPort(
    /* [in] */ Boolean use)
{
    Mutex::Autolock lock(mLock);

    if (mUseWideViewport != use) {
        mUseWideViewport = use;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getUseWideViewPort()
 */
Boolean WebSettingsClassic::GetUseWideViewPort()
{
    Mutex::Autolock lock(mLock);

    return mUseWideViewport;
}

/**
 * @see android.webkit.WebSettings#setSupportMultipleWindows(boolean)
 */
ECode WebSettingsClassic::SetSupportMultipleWindows(
    /* [in] */ Boolean support)
{
    Mutex::Autolock lock(mLock);

    if (mSupportMultipleWindows != support) {
        mSupportMultipleWindows = support;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#supportMultipleWindows()
 */
Boolean WebSettingsClassic::SupportMultipleWindows()
{
    Mutex::Autolock lock(mLock);

    return mSupportMultipleWindows;
}

/**
 * @see android.webkit.WebSettings#setLayoutAlgorithm(android.webkit.WebSettingsClassic.LayoutAlgorithm)
 */
ECode WebSettingsClassic::SetLayoutAlgorithm(
    /* [in] */ LayoutAlgorithm l)
{
    Mutex::Autolock lock(mLock);

    // XXX: This will only be affective if libwebcore was built with
    // ANDROID_LAYOUT defined.
    if (mLayoutAlgorithm != l) {
        mLayoutAlgorithm = l;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getLayoutAlgorithm()
 */
LayoutAlgorithm WebSettingsClassic::GetLayoutAlgorithm()
{
    Mutex::Autolock lock(mLock);

    return mLayoutAlgorithm;
}

/**
 * @see android.webkit.WebSettings#setStandardFontFamily(java.lang.String)
 */
ECode WebSettingsClassic::SetStandardFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mLock);

    if (!font.IsNull() && !font.Equals(mStandardFontFamily)) {
        mStandardFontFamily = font;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getStandardFontFamily()
 */
String WebSettingsClassic::GetStandardFontFamily()
{
    Mutex::Autolock lock(mLock);

    return mStandardFontFamily;
}

/**
 * @see android.webkit.WebSettings#setFixedFontFamily(java.lang.String)
 */
ECode WebSettingsClassic::SetFixedFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mLock);

    if (!font.IsNull() && !font.Equals(mFixedFontFamily)) {
        mFixedFontFamily = font;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getFixedFontFamily()
 */
String WebSettingsClassic::GetFixedFontFamily()
{
    Mutex::Autolock lock(mLock);

    return mFixedFontFamily;
}

/**
 * @see android.webkit.WebSettings#setSansSerifFontFamily(java.lang.String)
 */
ECode WebSettingsClassic::SetSansSerifFontFamily(
    /* [in] */  const String& font)
{
    Mutex::Autolock lock(mLock);

    if (!font.IsNull() && !font.Equals(mSansSerifFontFamily)) {
        mSansSerifFontFamily = font;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getSansSerifFontFamily()
 */
String WebSettingsClassic::GetSansSerifFontFamily()
{
    Mutex::Autolock lock(mLock);

    return mSansSerifFontFamily;
}

/**
 * @see android.webkit.WebSettings#setSerifFontFamily(java.lang.String)
 */
ECode WebSettingsClassic::SetSerifFontFamily(
    /* [in] */  const String& font)
{
    Mutex::Autolock lock(mLock);

    if (!font.IsNull() && !font.Equals(mSerifFontFamily)) {
        mSerifFontFamily = font;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getSerifFontFamily()
 */
String WebSettingsClassic::GetSerifFontFamily()
{
    Mutex::Autolock lock(mLock);

    return mSerifFontFamily;
}

/**
 * @see android.webkit.WebSettings#setCursiveFontFamily(java.lang.String)
 */
ECode WebSettingsClassic::SetCursiveFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mLock);

    if (!font.IsNull() && !font.Equals(mCursiveFontFamily)) {
        mCursiveFontFamily = font;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getCursiveFontFamily()
 */
String WebSettingsClassic::GetCursiveFontFamily()
{
    Mutex::Autolock lock(mLock);

    return mCursiveFontFamily;
}

/**
 * @see android.webkit.WebSettings#setFantasyFontFamily(java.lang.String)
 */
ECode WebSettingsClassic::SetFantasyFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mLock);

    if (font.IsNull() && !font.Equals(mFantasyFontFamily)) {
        mFantasyFontFamily = font;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getFantasyFontFamily()
 */
String WebSettingsClassic::GetFantasyFontFamily()
{
    Mutex::Autolock lock(mLock);

    return mFantasyFontFamily;
}

/**
 * @see android.webkit.WebSettings#setMinimumFontSize(int)
 */
ECode WebSettingsClassic::SetMinimumFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mLock);

    size = Pin(size);
    if (mMinimumFontSize != size) {
        mMinimumFontSize = size;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getMinimumFontSize()
 */
Int32 WebSettingsClassic::GetMinimumFontSize()
{
    Mutex::Autolock lock(mLock);

    return mMinimumFontSize;
}

/**
 * @see android.webkit.WebSettings#setMinimumLogicalFontSize(int)
 */
ECode WebSettingsClassic::SetMinimumLogicalFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mLock);

    size = Pin(size);
    if (mMinimumLogicalFontSize != size) {
        mMinimumLogicalFontSize = size;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getMinimumLogicalFontSize()
 */
Int32 WebSettingsClassic::GetMinimumLogicalFontSize()
{
    Mutex::Autolock lock(mLock);

    return mMinimumLogicalFontSize;
}

/**
 * @see android.webkit.WebSettings#setDefaultFontSize(int)
 */
ECode WebSettingsClassic::SetDefaultFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mLock);

    size = Pin(size);
    if (mDefaultFontSize != size) {
        mDefaultFontSize = size;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getDefaultFontSize()
 */
Int32 WebSettingsClassic::GetDefaultFontSize()
{
    Mutex::Autolock lock(mLock);

    return mDefaultFontSize;
}

/**
 * @see android.webkit.WebSettings#setDefaultFixedFontSize(int)
 */
ECode WebSettingsClassic::SetDefaultFixedFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mLock);

    size = Pin(size);
    if (mDefaultFixedFontSize != size) {
        mDefaultFixedFontSize = size;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getDefaultFixedFontSize()
 */
Int32 WebSettingsClassic::GetDefaultFixedFontSize()
{
    Mutex::Autolock lock(mLock);

    return mDefaultFixedFontSize;
}

/**
 * Set the number of pages cached by the WebKit for the history navigation.
 * @param size A non-negative integer between 0 (no cache) and 20 (max).
 */
ECode WebSettingsClassic::SetPageCacheCapacity(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mLock);

    if (size < 0) size = 0;
    if (size > 20) size = 20;
    if (mPageCacheCapacity != size) {
        mPageCacheCapacity = size;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setLoadsImagesAutomatically(boolean)
 */
ECode WebSettingsClassic::SetLoadsImagesAutomatically(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mLoadsImagesAutomatically != flag) {
        mLoadsImagesAutomatically = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getLoadsImagesAutomatically()
 */
Boolean WebSettingsClassic::GetLoadsImagesAutomatically()
{
    Mutex::Autolock lock(mLock);

    return mLoadsImagesAutomatically;
}

/**
 * @see android.webkit.WebSettings#setBlockNetworkImage(boolean)
 */
ECode WebSettingsClassic::SetBlockNetworkImage(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mBlockNetworkImage != flag) {
        mBlockNetworkImage = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getBlockNetworkImage()
 */
Boolean WebSettingsClassic::GetBlockNetworkImage()
{
    Mutex::Autolock lock(mLock);

    return mBlockNetworkImage;
}

/**
 * @see android.webkit.WebSettings#setBlockNetworkLoads(boolean)
 */
ECode WebSettingsClassic::SetBlockNetworkLoads(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mBlockNetworkLoads != flag) {
        mBlockNetworkLoads = flag;
        FAIL_RETURN(VerifyNetworkAccess());
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getBlockNetworkLoads()
 */
Boolean WebSettingsClassic::GetBlockNetworkLoads()
{
    Mutex::Autolock lock(mLock);

    return mBlockNetworkLoads;
}

ECode WebSettingsClassic::VerifyNetworkAccess()
{
    if (!mBlockNetworkLoads) {
        Int32 permission;
        mContext->CheckPermission(Elastos::Droid::Manifest::Permission::INTERNET,
                Process::MyPid(), Process::MyUid(), &permission);
        if (permission  != IPackageManager::PERMISSION_GRANTED) {
            //throw new SecurityException
            //        ("Permission denied - " +
            //                "application missing INTERNET permission");
            return E_SECURITY_EXCEPTION;
        }
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setJavaScriptEnabled(boolean)
 */
ECode WebSettingsClassic::SetJavaScriptEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mJavaScriptEnabled != flag) {
        mJavaScriptEnabled = flag;
        PostSync();
        mWebView->UpdateJavaScriptEnabled(flag);
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setAllowUniversalAccessFromFileURLs
 */
ECode WebSettingsClassic::SetAllowUniversalAccessFromFileURLs(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mAllowUniversalAccessFromFileURLs != flag) {
        mAllowUniversalAccessFromFileURLs = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setAllowFileAccessFromFileURLs
 */
ECode WebSettingsClassic::SetAllowFileAccessFromFileURLs(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mAllowFileAccessFromFileURLs != flag) {
        mAllowFileAccessFromFileURLs = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * Tell the WebView to use Skia's hardware accelerated rendering path
 * @param flag True if the WebView should use Skia's hw-accel path
 */
ECode WebSettingsClassic::SetHardwareAccelSkiaEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mHardwareAccelSkia != flag) {
        mHardwareAccelSkia = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @return True if the WebView is using hardware accelerated skia
 */
ECode WebSettingsClassic::GetHardwareAccelSkiaEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    Mutex::Autolock lock(mLock);
    *enabled = mHardwareAccelSkia;
    return NOERROR;
}

/**
 * Tell the WebView to show the visual indicator
 * @param flag True if the WebView should show the visual indicator
 */
ECode WebSettingsClassic::SetShowVisualIndicator(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mShowVisualIndicator != flag) {
        mShowVisualIndicator = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @return True if the WebView is showing the visual indicator
 */
ECode WebSettingsClassic::GetShowVisualIndicator(
    /* [out] */ Boolean* isShowing)
{
    VALIDATE_NOT_NULL(isShowing);
    Mutex::Autolock lock(mLock);
    *isShowing = mShowVisualIndicator;
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setPluginsEnabled(boolean)
 */
ECode WebSettingsClassic::SetPluginsEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    SetPluginState(flag ? PluginState_ON : PluginState_OFF);
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setPluginState(android.webkit.WebSettingsClassic.PluginState)
 */
ECode WebSettingsClassic::SetPluginState(
    /* [in] */ PluginState state)
{
    Mutex::Autolock lock(mLock);

    if (mPluginState != state) {
        mPluginState = state;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setPluginsPath(java.lang.String)
 */
ECode WebSettingsClassic::SetPluginsPath(
    /* [in] */ const String& pluginsPath)
{
    Mutex::Autolock lock(mLock);
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setDatabasePath(java.lang.String)
 */
ECode WebSettingsClassic::SetDatabasePath(
    /* [in] */ const String& databasePath)
{
    Mutex::Autolock lock(mLock);

    if (!databasePath.IsNull() && !mDatabasePathHasBeenSet) {
        mDatabasePath = databasePath;
        mDatabasePathHasBeenSet = TRUE;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setGeolocationDatabasePath(java.lang.String)
 */
ECode WebSettingsClassic::SetGeolocationDatabasePath(
    /* [in] */ const String& databasePath)
{
    Mutex::Autolock lock(mLock);

    if (!databasePath.IsNull()
            && !databasePath.Equals(mGeolocationDatabasePath)) {
        mGeolocationDatabasePath = databasePath;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setAppCacheEnabled(boolean)
 */
ECode WebSettingsClassic::SetAppCacheEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mAppCacheEnabled != flag) {
        mAppCacheEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setAppCachePath(java.lang.String)
 */
ECode WebSettingsClassic::SetAppCachePath(
    /* [in] */ const String& path)
{
    Mutex::Autolock lock(mLock);

    // We test for a valid path and for repeated setting on the native
    // side, but we can avoid syncing in some simple cases.
    if (mAppCachePath.IsNull() && !path.IsNullOrEmpty()) {
        mAppCachePath = path;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setAppCacheMaxSize(long)
 */
ECode WebSettingsClassic::SetAppCacheMaxSize(
    /* [in] */ Int64 appCacheMaxSize)
{
    Mutex::Autolock lock(mLock);

    if (appCacheMaxSize != mAppCacheMaxSize) {
        mAppCacheMaxSize = appCacheMaxSize;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setDatabaseEnabled(boolean)
 */
ECode WebSettingsClassic::SetDatabaseEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mDatabaseEnabled != flag) {
        mDatabaseEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setDomStorageEnabled(boolean)
 */
ECode WebSettingsClassic::SetDomStorageEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mDomStorageEnabled != flag) {
        mDomStorageEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getDomStorageEnabled()
 */
Boolean WebSettingsClassic::GetDomStorageEnabled()
{
    Mutex::Autolock lock(mLock);

    return mDomStorageEnabled;
}

/**
 * @see android.webkit.WebSettings#getDatabasePath()
 */
String WebSettingsClassic::GetDatabasePath()
{
    Mutex::Autolock lock(mLock);

    return mDatabasePath;
}

/**
 * @see android.webkit.WebSettings#getDatabaseEnabled()
 */
Boolean WebSettingsClassic::GetDatabaseEnabled()
{
    Mutex::Autolock lock(mLock);

    return mDatabaseEnabled;
}

/**
 * Tell the WebView to enable WebWorkers API.
 * @param flag True if the WebView should enable WebWorkers.
 * Note that this flag only affects V8. JSC does not have
 * an equivalent setting.
 */
ECode WebSettingsClassic::SetWorkersEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mWorkersEnabled != flag) {
        mWorkersEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setGeolocationEnabled(boolean)
 */
ECode WebSettingsClassic::SetGeolocationEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mGeolocationEnabled != flag) {
        mGeolocationEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * Sets whether XSS Auditor is enabled.
 * Only used by LayoutTestController.
 * @param flag Whether XSS Auditor should be enabled.
 */
ECode WebSettingsClassic::SetXSSAuditorEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mXSSAuditorEnabled != flag) {
        mXSSAuditorEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * Enables/disables HTML5 link "prefetch" parameter.
 */
ECode WebSettingsClassic::SetLinkPrefetchEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mLinkPrefetchEnabled != flag) {
        mLinkPrefetchEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getJavaScriptEnabled()
 */
Boolean WebSettingsClassic::GetJavaScriptEnabled()
{
    Mutex::Autolock lock(mLock);

    return mJavaScriptEnabled;
}

/**
 * @see android.webkit.WebSettings#getAllowUniversalFileAccessFromFileURLs
 */
Boolean WebSettingsClassic::GetAllowUniversalAccessFromFileURLs()
{
    Mutex::Autolock lock(mLock);

    return mAllowUniversalAccessFromFileURLs;
}

/**
 * @see android.webkit.WebSettings#getAllowFileAccessFromFileURLs
 */
Boolean WebSettingsClassic::GetAllowFileAccessFromFileURLs()
{
    Mutex::Autolock lock(mLock);

    return mAllowFileAccessFromFileURLs;
}

/**
 * @see android.webkit.WebSettings#getPluginsEnabled()
 */
Boolean WebSettingsClassic::GetPluginsEnabled()
{
    Mutex::Autolock lock(mLock);

    return mPluginState == PluginState_ON;
}

/**
 * @see android.webkit.WebSettings#getPluginState()
 */
PluginState WebSettingsClassic::GetPluginState()
{
    Mutex::Autolock lock(mLock);

    return mPluginState;
}

/**
 * @see android.webkit.WebSettings#getPluginsPath()
 */
String WebSettingsClassic::GetPluginsPath()
{
    Mutex::Autolock lock(mLock);

    return String("");
}

/**
 * @see android.webkit.WebSettings#setJavaScriptCanOpenWindowsAutomatically(boolean)
 */
ECode WebSettingsClassic::SetJavaScriptCanOpenWindowsAutomatically(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mJavaScriptCanOpenWindowsAutomatically != flag) {
        mJavaScriptCanOpenWindowsAutomatically = flag;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getJavaScriptCanOpenWindowsAutomatically()
 */
Boolean WebSettingsClassic::GetJavaScriptCanOpenWindowsAutomatically()
{
    Mutex::Autolock lock(mLock);

    return mJavaScriptCanOpenWindowsAutomatically;
}

/**
 * @see android.webkit.WebSettings#setDefaultTextEncodingName(java.lang.String)
 */
ECode WebSettingsClassic::SetDefaultTextEncodingName(
    /* [in] */ const String& encoding)
{
    Mutex::Autolock lock(mLock);

    if (!encoding.IsNull() && !encoding.Equals(mDefaultTextEncoding)) {
        mDefaultTextEncoding = encoding;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getDefaultTextEncodingName()
 */
String WebSettingsClassic::GetDefaultTextEncodingName()
{
    Mutex::Autolock lock(mLock);

    return mDefaultTextEncoding;
}

/**
 * @see android.webkit.WebSettings#setUserAgentString(java.lang.String)
 */
ECode WebSettingsClassic::SetUserAgentString(
    /* [in] */ const String& _ua)
{
    Mutex::Autolock lock(mLock);

    String ua = _ua;
    if (ua.IsNullOrEmpty()) {
        {
            Mutex::Autolock lock(sLockForLocaleSettings);

            AutoPtr<ILocaleHelper> helper;
            CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
            AutoPtr<ILocale> currentLocale;
            helper->GetDefault((ILocale**)&currentLocale);
            Boolean result = FALSE;
            sLocale->Equals(currentLocale, &result);
            if (!result) {
                sLocale = currentLocale;
                mAcceptLanguage = GetCurrentAcceptLanguage();
            }
        }
        ua = GetCurrentUserAgent();
        mUseDefaultUserAgent = TRUE;
    }
    else  {
        mUseDefaultUserAgent = FALSE;
    }

    if (!ua.Equals(mUserAgent)) {
        mUserAgent = ua;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getUserAgentString()
 */
String WebSettingsClassic::GetUserAgentString()
{
    Mutex::Autolock lock(mLock);

    if (DESKTOP_USERAGENT.Equals(mUserAgent) ||
            IPHONE_USERAGENT.Equals(mUserAgent) ||
            !mUseDefaultUserAgent) {
        return mUserAgent;
    }

    Boolean doPostSync = FALSE;
    {
        Mutex::Autolock lock(sLockForLocaleSettings);

        AutoPtr<ILocaleHelper> helper;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
        AutoPtr<ILocale> currentLocale;
        helper->GetDefault((ILocale**)&currentLocale);
        Boolean result = FALSE;
        sLocale->Equals(currentLocale, &result);
        if (!result) {
            sLocale = currentLocale;
            mUserAgent = GetCurrentUserAgent();
            mAcceptLanguage = GetCurrentAcceptLanguage();
            doPostSync = TRUE;
        }
    }
    if (doPostSync) {
        PostSync();
    }
    return mUserAgent;
}

/* package api to grab the Accept Language string. */
String WebSettingsClassic::GetAcceptLanguage()
{
    Mutex::Autolock lock(mLock);

    {
        Mutex::Autolock lock(sLockForLocaleSettings);

        AutoPtr<ILocaleHelper> helper;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
        AutoPtr<ILocale> currentLocale;
        helper->GetDefault((ILocale**)&currentLocale);
        Boolean result = FALSE;
        sLocale->Equals(currentLocale, &result);
        if (!result) {
            sLocale = currentLocale;
            mAcceptLanguage = GetCurrentAcceptLanguage();
        }
    }
    return mAcceptLanguage;
}

/* package */
Boolean WebSettingsClassic::IsNarrowColumnLayout()
{
    return GetLayoutAlgorithm() == LayoutAlgorithm_NARROW_COLUMNS;
}

/**
 * @see android.webkit.WebSettings#setNeedInitialFocus(boolean)
 */
ECode WebSettingsClassic::SetNeedInitialFocus(
    /* [in] */ Boolean flag)
{
    if (mNeedInitialFocus != flag) {
        mNeedInitialFocus = flag;
    }
    return NOERROR;
}

/* Package api to get the choice whether it needs to set initial focus. */
Boolean WebSettingsClassic::GetNeedInitialFocus()
{
    return mNeedInitialFocus;
}

/**
 * @see android.webkit.WebSettings#setRenderPriority(android.webkit.WebSettingsClassic.RenderPriority)
 */
ECode WebSettingsClassic::SetRenderPriority(
    /* [in] */ RenderPriority priority)
{
    Mutex::Autolock lock(mLock);

    if (mRenderPriority != priority) {
        mRenderPriority = priority;
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(NULL, EventHandler::PRIORITY, (IMessage**)&msg);
        mEventHandler->SendMessage(msg);
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#setCacheMode(int)
 */
ECode WebSettingsClassic::SetCacheMode(
    /* [in] */ Int32 mode)
{
    if (mode != mOverrideCacheMode) {
        mOverrideCacheMode = mode;
        PostSync();
    }
    return NOERROR;
}

/**
 * @see android.webkit.WebSettings#getCacheMode()
 */
Int32 WebSettingsClassic::GetCacheMode()
{
    return mOverrideCacheMode;
}

/**
 * If set, webkit alternately shrinks and expands images viewed outside
 * of an HTML page to fit the screen. This conflicts with attempts by
 * the UI to zoom in and out of an image, so it is set false by default.
 * @param shrink Set true to let webkit shrink the standalone image to fit.
 */
ECode WebSettingsClassic::SetShrinksStandaloneImagesToFit(
    /* [in] */ Boolean shrink)
{
    if (mShrinksStandaloneImagesToFit != shrink) {
        mShrinksStandaloneImagesToFit = shrink;
        PostSync();
    }
    return NOERROR;
}

/**
 * Specify the maximum decoded image size. The default is
 * 2 megs for small memory devices and 8 megs for large memory devices.
 * @param size The maximum decoded size, or zero to set to the default.
 */
ECode WebSettingsClassic::SetMaximumDecodedImageSize(
    /* [in] */ Int64 size)
{
    if (mMaximumDecodedImageSize != size) {
        mMaximumDecodedImageSize = size;
        PostSync();
    }
    return NOERROR;
}

/**
 * Returns whether to use fixed viewport.  Use fixed viewport
 * whenever wide viewport is on.
 */
Boolean WebSettingsClassic::GetUseFixedViewport()
{
    return GetUseWideViewPort();
}

/**
 * Returns whether private browsing is enabled.
 */
Boolean WebSettingsClassic::IsPrivateBrowsingEnabled()
{
    return mPrivateBrowsingEnabled;
}

/**
 * Sets whether private browsing is enabled.
 * @param flag Whether private browsing should be enabled.
 */
ECode WebSettingsClassic::SetPrivateBrowsingEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mPrivateBrowsingEnabled != flag) {
        mPrivateBrowsingEnabled = flag;

        // AutoFill is dependant on private browsing being enabled so
        // reset it to take account of the new value of mPrivateBrowsingEnabled.
        SetAutoFillEnabled(mAutoFillEnabled);

        PostSync();
    }
    return NOERROR;
}

/**
 * Returns whether the viewport metatag can disable zooming
 */
ECode WebSettingsClassic::ForceUserScalable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mForceUserScalable;
    return NOERROR;
}

/**
 * Sets whether viewport metatag can disable zooming.
 * @param flag Whether or not to forceably enable user scalable.
 */
ECode WebSettingsClassic::SetForceUserScalable(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    mForceUserScalable = flag;
    return NOERROR;
}

//synchronized
ECode WebSettingsClassic::SetSyntheticLinksEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mLock);

    if (mSyntheticLinksEnabled != flag) {
        mSyntheticLinksEnabled = flag;
        PostSync();
    }
    return NOERROR;
}

//synchronized
ECode WebSettingsClassic::SetAutoFillEnabled(
    /* [in] */ Boolean enabled)
{
    Mutex::Autolock lock(mLock);

    // AutoFill is always disabled in private browsing mode.
    Boolean autoFillEnabled = enabled && !mPrivateBrowsingEnabled;
    if (mAutoFillEnabled != autoFillEnabled) {
        mAutoFillEnabled = autoFillEnabled;
        PostSync();
    }
    return NOERROR;
}

ECode WebSettingsClassic::GetAutoFillEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    Mutex::Autolock lock(mLock);
    *enabled = mAutoFillEnabled;
    return NOERROR;
}

ECode WebSettingsClassic::SetAutoFillProfile(
    /* [in] */ IAutoFillProfile* profile)
{
    Mutex::Autolock lock(mLock);

    if (mAutoFillProfile.Get() != profile) {
        mAutoFillProfile = profile;
        PostSync();
    }
    return NOERROR;
}

ECode WebSettingsClassic::GetAutoFillProfile(
    /* [out] */ IAutoFillProfile** profile)
{
    VALIDATE_NOT_NULL(profile);
    Mutex::Autolock lock(mLock);
    *profile = mAutoFillProfile;
    REFCOUNT_ADD(*profile);
    return NOERROR;
}

ECode WebSettingsClassic::GetDoubleTapToastCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = sDoubleTapToastCount;
    return NOERROR;
}

ECode WebSettingsClassic::SetDoubleTapToastCount(
    /* [in] */ Int32 count)
{
    if (sDoubleTapToastCount != count) {
        sDoubleTapToastCount = count;
        // write the settings in the non-UI thread
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(NULL, EventHandler::SET_DOUBLE_TAP_TOAST_COUNT, (IMessage**)&msg);
        mEventHandler->SendMessage(msg);
    }
    return NOERROR;
}

ECode WebSettingsClassic::SetProperty(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    if (mWebView->NativeSetProperty(key, value)) {
        mWebView->Invalidate();
    }
    return NOERROR;
}

ECode WebSettingsClassic::GetProperty(
    /* [in] */ const String& key,
    /* [out] */ String* property)
{
    VALIDATE_NOT_NULL(property);
    *property = mWebView->NativeGetProperty(key);
    return NOERROR;
}

/**
 * Transfer messages from the queue to the new WebCoreThread. Called from
 * WebCore thread.
 */
void WebSettingsClassic::SyncSettingsAndCreateHandler(
    /* [in] */ BrowserFrame* frame)
{
    Mutex::Autolock lock(mLock);

    mBrowserFrame = frame;
    if (DebugFlags::WEB_SETTINGS) {
        assert(frame->mNativeFrame != 0);
    }

    AutoPtr<ISharedPreferences> sp;
    mContext->GetSharedPreferences(PREF_FILE,
            IContext::MODE_PRIVATE, (ISharedPreferences**)&sp);
    if (sDoubleTapToastCount > 0) {
        sp->GetInt32(DOUBLE_TAP_TOAST_COUNT,
                sDoubleTapToastCount, &sDoubleTapToastCount);
    }
    NativeSync(frame->mNativeFrame);
    mSyncPending = FALSE;
    mEventHandler->CreateHandler();
}

/**
 * Let the Settings object know that our owner is being destroyed.
 */
void WebSettingsClassic::OnDestroyed()
{
    Mutex::Autolock lock(mLock);
}

Int32 WebSettingsClassic::Pin(
    /* [in] */ Int32 size)
{
    // FIXME: 72 is just an arbitrary max text size value.
    if (size < 1) {
        return 1;
    }
    else if (size > 72) {
        return 72;
    }
    return size;
}

/* Post a SYNC message to handle syncing the native settings. */
void WebSettingsClassic::PostSync()
{
    Mutex::Autolock lock(mLock);

    // Only post if a sync is not pending
    if (!mSyncPending) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(NULL, EventHandler::SYNC, (IMessage**)&msg);
        mSyncPending = mEventHandler->SendMessage(msg);
    }
}

// Synchronize the native and java settings.
void WebSettingsClassic::NativeSync(
    /* [in] */ Int32 frame)
{
    Mutex::Autolock lock(mLock);
    Elastos_WebSettings_nativeSync(this, frame);
}

ECode WebSettingsClassic::GetNavDump(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetNavDump();
    return NOERROR;
}

ECode WebSettingsClassic::SupportZoom(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = SupportZoom();
    return NOERROR;
}

ECode WebSettingsClassic::GetMediaPlaybackRequiresUserGesture(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetMediaPlaybackRequiresUserGesture();
    return NOERROR;
}

ECode WebSettingsClassic::GetBuiltInZoomControls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetBuiltInZoomControls();
    return NOERROR;
}

ECode WebSettingsClassic::GetDisplayZoomControls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetDisplayZoomControls();
    return NOERROR;
}

ECode WebSettingsClassic::GetAllowFileAccess(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetAllowFileAccess();
    return NOERROR;
}

ECode WebSettingsClassic::GetAllowContentAccess(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetAllowContentAccess();
    return NOERROR;
}

ECode WebSettingsClassic::GetLoadWithOverviewMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetLoadWithOverviewMode();
    return NOERROR;
}

ECode WebSettingsClassic::EnableSmoothTransition(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = EnableSmoothTransition();
    return NOERROR;
}

ECode WebSettingsClassic::GetUseWebViewBackgroundForOverscrollBackground(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetUseWebViewBackgroundForOverscrollBackground();
    return NOERROR;
}

ECode WebSettingsClassic::GetSaveFormData(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetSaveFormData();
    return NOERROR;
}

ECode WebSettingsClassic::GetSavePassword(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetSavePassword();
    return NOERROR;
}

ECode WebSettingsClassic::GetTextZoom(
    /* [out] */ Int32* textZoom)
{
    VALIDATE_NOT_NULL(textZoom);
    *textZoom = GetTextZoom();
    return NOERROR;
}

ECode WebSettingsClassic::SetTextSize(
    /* [in] */ TextSize t)
{
    SetTextSize(t);
    return NOERROR;
}

ECode WebSettingsClassic::GetTextSize(
    /* [out] */ TextSize* textSize)
{
    VALIDATE_NOT_NULL(textSize);
    *textSize = WebSettings::GetTextSize();
    return NOERROR;
}

ECode WebSettingsClassic::GetDefaultZoom(
    /* [out] */ ZoomDensity* zoom)
{
    VALIDATE_NOT_NULL(zoom);
    *zoom = GetDefaultZoom();
    return NOERROR;
}

ECode WebSettingsClassic::GetLightTouchEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetLightTouchEnabled();
    return NOERROR;
}

ECode WebSettingsClassic::GetUseDoubleTree(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetUseDoubleTree();
    return NOERROR;
}

ECode WebSettingsClassic::GetUserAgent(
    /* [out] */ Int32* ua)
{
    VALIDATE_NOT_NULL(ua);
    *ua = GetUserAgent();
    return NOERROR;
}

ECode WebSettingsClassic::GetUseWideViewPort(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetUseWideViewPort();
    return NOERROR;
}

ECode WebSettingsClassic::SupportMultipleWindows(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = SupportMultipleWindows();
    return NOERROR;
}

ECode WebSettingsClassic::GetLayoutAlgorithm(
    /* [out] */ LayoutAlgorithm* pL)
{
    VALIDATE_NOT_NULL(pL);
    *pL = GetLayoutAlgorithm();
    return NOERROR;
}

ECode WebSettingsClassic::GetStandardFontFamily(
    /* [out] */ String* font)
{
    VALIDATE_NOT_NULL(font);
    *font = GetStandardFontFamily();
    return NOERROR;
}

ECode WebSettingsClassic::GetFixedFontFamily(
    /* [out] */ String* font)
{
    VALIDATE_NOT_NULL(font);
    *font = GetFixedFontFamily();
    return NOERROR;
}

ECode WebSettingsClassic::GetSansSerifFontFamily(
    /* [out] */ String* font)
{
    VALIDATE_NOT_NULL(font);
    *font = GetSansSerifFontFamily();
    return NOERROR;
}

ECode WebSettingsClassic::GetSerifFontFamily(
    /* [out] */ String* font)
{
    VALIDATE_NOT_NULL(font);
    *font = GetSerifFontFamily();
    return NOERROR;
}

ECode WebSettingsClassic::GetCursiveFontFamily(
    /* [out] */ String* font)
{
    VALIDATE_NOT_NULL(font);
    *font = GetCursiveFontFamily();
    return NOERROR;
}

ECode WebSettingsClassic::GetFantasyFontFamily(
    /* [out] */ String* font)
{
    VALIDATE_NOT_NULL(font);
    *font = GetFantasyFontFamily();
    return NOERROR;
}

ECode WebSettingsClassic::GetMinimumFontSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = GetMinimumFontSize();
    return NOERROR;
}

ECode WebSettingsClassic::GetMinimumLogicalFontSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = GetMinimumLogicalFontSize();
    return NOERROR;
}

ECode WebSettingsClassic::GetDefaultFontSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = GetDefaultFontSize();
    return NOERROR;
}

ECode WebSettingsClassic::GetDefaultFixedFontSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = GetDefaultFixedFontSize();
    return NOERROR;
}

ECode WebSettingsClassic::GetLoadsImagesAutomatically(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = GetLoadsImagesAutomatically();
    return NOERROR;
}

ECode WebSettingsClassic::GetBlockNetworkImage(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = GetBlockNetworkImage();
    return NOERROR;
}

ECode WebSettingsClassic::GetBlockNetworkLoads(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = GetBlockNetworkLoads();
    return NOERROR;
}

ECode WebSettingsClassic::GetDomStorageEnabled(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = GetDomStorageEnabled();
    return NOERROR;
}

ECode WebSettingsClassic::GetDatabasePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = GetDatabasePath();
    return NOERROR;
}

ECode WebSettingsClassic::GetDatabaseEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = GetDatabaseEnabled();
    return NOERROR;
}

ECode WebSettingsClassic::GetJavaScriptEnabled(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = GetJavaScriptEnabled();
    return NOERROR;
}

ECode WebSettingsClassic::GetAllowUniversalAccessFromFileURLs(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetAllowUniversalAccessFromFileURLs();
    return NOERROR;
}

ECode WebSettingsClassic::GetAllowFileAccessFromFileURLs(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetAllowFileAccessFromFileURLs();
    return NOERROR;
}

ECode WebSettingsClassic::GetPluginsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = GetPluginsEnabled();
    return NOERROR;
}

ECode WebSettingsClassic::GetPluginState(
    /* [out] */ PluginState* state)
{
    VALIDATE_NOT_NULL(state);
    *state = GetPluginState();
    return NOERROR;
}

ECode WebSettingsClassic::GetPluginsPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = GetPluginsPath();
    return NOERROR;
}

ECode WebSettingsClassic::GetJavaScriptCanOpenWindowsAutomatically(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = GetJavaScriptCanOpenWindowsAutomatically();
    return NOERROR;
}

ECode WebSettingsClassic::GetDefaultTextEncodingName(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    *encoding = GetDefaultTextEncodingName();
    return NOERROR;
}

ECode WebSettingsClassic::GetUserAgentString(
    /* [out] */ String* ua)
{
    VALIDATE_NOT_NULL(ua);
    *ua = GetUserAgentString();
    return NOERROR;
}

ECode WebSettingsClassic::GetCacheMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = GetCacheMode();
    return NOERROR;
}

//begin callback definition
//return the value from Enum ordinal
Int32 WebSettingsClassic::GetLayoutAlgorithm(
    /* [in] */ IInterface* obj)
{
    if (DebugFlags::WEB_SETTINGS)
        Logger::E(LOGTAG, "WebSettingsClassic::GetLayoutAlgorithm");
    LayoutAlgorithm algorithm = ((WebSettingsClassic*)obj)->GetLayoutAlgorithm();
    return (Int32)algorithm;
}

Int32 WebSettingsClassic::GetTextSize(
    /* [in] */ IInterface* obj)
{
    TextSize t;
    ((WebSettingsClassic*)obj)->GetTextSize(&t);
    if (DebugFlags::WEB_SETTINGS)
        Logger::E(LOGTAG, "WebSettingsClassic::GetTextSize, textsize:%d", t);
    return (Int32)t;
}

String WebSettingsClassic::GetStandardFontFamily(
    /* [in] */ IInterface* obj)
{
    String fam = ((WebSettingsClassic*)obj)->GetStandardFontFamily();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetStandardFontFamily:%s", fam.string());
    return fam;
}

String WebSettingsClassic::GetFixedFontFamily(
    /* [in] */ IInterface* obj)
{
    String fam = ((WebSettingsClassic*)obj)->GetFixedFontFamily();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetFixedFontFamily:%s", fam.string());
    return fam;
}

String WebSettingsClassic::GetSansSerifFontFamily(
    /* [in] */ IInterface* obj)
{
    String fam = ((WebSettingsClassic*)obj)->GetSansSerifFontFamily();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetSansSerifFontFamily:%s", fam.string());
    return fam;
}

String WebSettingsClassic::GetSerifFontFamily(
    /* [in] */ IInterface* obj)
{
    String fam = ((WebSettingsClassic*)obj)->GetSerifFontFamily();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetSerifFontFamily:%s",fam.string());
    return fam;
}

String WebSettingsClassic::GetCursiveFontFamily(
    /* [in] */ IInterface* obj)
{
    String fam = ((WebSettingsClassic*)obj)->GetCursiveFontFamily();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetCursiveFontFamily:%s",fam.string());
    return fam;
}

String WebSettingsClassic::GetFantasyFontFamily(
    /* [in] */ IInterface* obj)
{
    String fam = ((WebSettingsClassic*)obj)->GetFantasyFontFamily();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetFantasyFontFamily:%s",fam.string());
    return fam;
}

String WebSettingsClassic::GetDefaultTextEncoding(
    /* [in] */ IInterface* obj)
{
    String testEncoding = ((WebSettingsClassic*)obj)->GetDefaultTextEncodingName();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetDefaultTextEncoding:%s", testEncoding.string());
    return testEncoding;
}

String WebSettingsClassic::GetUserAgentString(
    /* [in] */ IInterface* obj)//method
{
    String userAgent = ((WebSettingsClassic*)obj)->GetUserAgentString();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetUserAgentString:%s,", userAgent.string());
    return userAgent;
}

String WebSettingsClassic::GetAcceptLanguage(
    /* [in] */ IInterface* obj)//method
{
    String al = ((WebSettingsClassic*)obj)->GetAcceptLanguage();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAcceptLanguage:%s", al.string());
    return al;
}

Int32 WebSettingsClassic::GetMinimumFontSize(
    /* [in] */ IInterface* obj)
{
    Int32 size = ((WebSettingsClassic*)obj)->GetMinimumFontSize();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetMinimumFontSize:%d",size);
    return size;
}

Int32 WebSettingsClassic::GetMinimumLogicalFontSize(
    /* [in] */ IInterface* obj)
{
    Int32 size = ((WebSettingsClassic*)obj)->GetMinimumLogicalFontSize();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetMinimumLogicalFontSize:%d",size);
    return size;
}

Int32 WebSettingsClassic::GetDefaultFontSize(
    /* [in] */ IInterface* obj)
{
    Int32 size = ((WebSettingsClassic*)obj)->GetDefaultFontSize();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetDefaultFontSize:%d", size);
    return size;
}

Int32 WebSettingsClassic::GetDefaultFixedFontSize(
    /* [in] */ IInterface* obj)
{
    Int32 size = ((WebSettingsClassic*)obj)->GetDefaultFixedFontSize();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetDefaultFixedFontSize:%d", size);
    return size;
}

Boolean WebSettingsClassic::GetLoadsImagesAutomatically(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetLoadsImagesAutomatically();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetLoadsImagesAutomatically:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetBlockNetworkImage(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetBlockNetworkImage();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetBlockNetworkImage:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetBlockNetworkLoads(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetBlockNetworkLoads();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetBlockNetworkLoads:%d", t);
    return t;
}

Boolean WebSettingsClassic::GetJavaScriptEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetJavaScriptEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetJavaScriptEnabled:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetAllowUniversalAccessFromFileURLs(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetAllowUniversalAccessFromFileURLs();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAllowUniversalAccessFromFileURLs:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetAllowFileAccessFromFileURLs(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetAllowFileAccessFromFileURLs();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAllowFileAccessFromFileURLs:%d",t);
    return t;
}

Int32 WebSettingsClassic::GetPluginState(
    /* [in] */ IInterface* obj)//return the value from Enum ordina
{
    PluginState t = ((WebSettingsClassic*)obj)->GetPluginState();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetPluginState:%d",t);
    return (Int32)t;
}

Boolean WebSettingsClassic::GetAppCacheEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mAppCacheEnabled;//GetAppCacheEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "Need check WebSettingsClassic::GetAppCacheEnabled:%d",t);
    return t;
}

String WebSettingsClassic::GetAppCachePath(
    /* [in] */ IInterface* obj)
{
    String t = ((WebSettingsClassic*)obj)->mAppCachePath;//GetAppCachePath();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "Need check WebSettingsClassic::GetAppCachePath:%s",t.string());
    return t;
}

Int64 WebSettingsClassic::GetAppCacheMaxSize(
    /* [in] */ IInterface* obj)
{
    Int64 t = ((WebSettingsClassic*)obj)->mAppCacheMaxSize;//GetAppCacheMaxSize();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "Need check WebSettingsClassic::GetAppCacheMaxSize%ld", t);
    return t;
}

Boolean WebSettingsClassic::GetWorkersEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mWorkersEnabled;//GetWorkersEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "Need check WebSettingsClassic::GetWorkersEnabled:%d", t);
    return t;
}

Boolean WebSettingsClassic::GetJavaScriptCanOpenWindowsAutomatically(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetJavaScriptCanOpenWindowsAutomatically();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetJavaScriptCanOpenWindowsAutomatically:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetUseWideViewport(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetUseWideViewPort();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetUseWideViewport:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetSupportMultipleWindows(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->SupportMultipleWindows();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetSupportMultipleWindows:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetShrinksStandaloneImagesToFit(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mShrinksStandaloneImagesToFit;//GetShrinksStandaloneImagesToFit();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "Need check WebSettingsClassic::GetShrinksStandaloneImagesToFit:%d",t);
    return t;
}

Int64 WebSettingsClassic::GetMaximumDecodedImageSize(
    /* [in] */ IInterface* obj)
{
    Int64 t = ((WebSettingsClassic*)obj)->mMaximumDecodedImageSize;//GetMaximumDecodedImageSize();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "Need check WebSettingsClassic::GetMaximumDecodedImageSize:%ld",t);
    return t;
}

Boolean WebSettingsClassic::GetPrivateBrowsingEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->IsPrivateBrowsingEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetPrivateBrowsingEnabled:%d", t);
    return t;
}

Boolean WebSettingsClassic::GetSyntheticLinksEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mSyntheticLinksEnabled;//GetSyntheticLinksEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "Need check WebSettingsClassic::GetSyntheticLinksEnabled:%d", t);
    return t;
}

Boolean WebSettingsClassic::GetUseDoubleTree(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetUseDoubleTree();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetUseDoubleTree:%d",t);
    return t;
}

Int32 WebSettingsClassic::GetPageCacheCapacity(
    /* [in] */ IInterface* obj)
{
    Int32 t = ((WebSettingsClassic*)obj)->mPageCacheCapacity;//GetPageCacheCapacity();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetPageCacheCapacity:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetDatabaseEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetDatabaseEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetDatabaseEnabled:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetDomStorageEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetDomStorageEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetDomStorageEnabled:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetGeolocationEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mGeolocationEnabled;//GetGeolocationEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetGeolocationEnabled:%d",t);
    return t;
}

String WebSettingsClassic::GetGeolocationDatabasePath(
    /* [in] */ IInterface* obj)
{
    String t = ((WebSettingsClassic*)obj)->mGeolocationDatabasePath;//GetGeolocationDatabasePath();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetGeolocationDatabasePath:%s",t.string());
    return t;
}

Boolean WebSettingsClassic::GetXSSAuditorEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mXSSAuditorEnabled;//GetXSSAuditorEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetXSSAuditorEnabled:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetLinkPrefetchEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mLinkPrefetchEnabled;//GetLinkPrefetchEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetLinkPrefetchEnabled:%d",t);
    return t;
}

String WebSettingsClassic::GetDatabasePath(
    /* [in] */ IInterface* obj)
{
    String t = ((WebSettingsClassic*)obj)->GetDatabasePath();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetDatabasePath:%s",t.string());
    return t;
}

Boolean WebSettingsClassic::GetDatabasePathHasBeenSet(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mDatabasePathHasBeenSet;//GetDatabasePathHasBeenSet();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetDatabasePathHasBeenSet:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetAutoFillEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mAutoFillEnabled;//GetAutoFillEnabled();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillEnabled:%d",t);
    return t;
}

AutoPtr<IInterface> WebSettingsClassic::GetAutoFillProfile(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IAutoFillProfile> t;
    ((WebSettingsClassic*)obj)->GetAutoFillProfile((IAutoFillProfile**)&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfile:%d",t.Get());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileFullName(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetFullName(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileFullName:%s",t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileEmailAddress(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetEmailAddress(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileEmailAddress:%s",t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileCompanyName(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetCompanyName(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileCompanyName:%s", t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileAddressLine1(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetAddressLine1(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileAddressLine1:%s",t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileAddressLine2(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetAddressLine2(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileAddressLine2:%s",t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileCity(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetCity(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileCity:%s", t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileState(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetState(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileState:%s",t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileZipCode(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetZipCode(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileZipCode:%s",t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfileCountry(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetCountry(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfileCountry:%s",t.string());
    return t;
}

String WebSettingsClassic::GetAutoFillProfilePhoneNumber(
    /* [in] */ IInterface* obj)
{
    String t;
    IAutoFillProfile::Probe(obj)->GetPhoneNumber(&t);
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetAutoFillProfilePhoneNumber:%s",t.string());
    return t;
}

Int32 WebSettingsClassic::GetOverrideCacheMode(
    /* [in] */ IInterface* obj)
{
    Int32 t = ((WebSettingsClassic*)obj)->GetCacheMode();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetOverrideCacheMode:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetPasswordEchoEnabled(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->mPasswordEchoEnabled;
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetPasswordEchoEnabled:%d",t);
    return t;
}

Boolean WebSettingsClassic::GetMediaPlaybackRequiresUserGesture(
    /* [in] */ IInterface* obj)
{
    Boolean t = ((WebSettingsClassic*)obj)->GetMediaPlaybackRequiresUserGesture();
    if (DebugFlags::WEB_SETTINGS)
         Logger::E(LOGTAG, "WebSettingsClassic::GetMediaPlaybackRequiresUserGesture:%d",t);
    return t;
}
//end callback definition

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
