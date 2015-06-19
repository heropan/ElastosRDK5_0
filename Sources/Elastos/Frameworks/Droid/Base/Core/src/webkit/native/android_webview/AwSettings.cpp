
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//            AwSettings::SetInitialPageScaleRunnable
//===============================================================

AwSettings::SetInitialPageScaleRunnable::SetInitialPageScaleRunnable(
    /* [in] */ AwSettings* owner)
    : mOwner(owner)
{
}

ECode AwSettings::SetInitialPageScaleRunnable::Run()
{
    if (mNativeAwSettings != 0) {
        NativeUpdateInitialPageScaleLocked(mNativeAwSettings);
    }

    return NOERROR;
}

//===============================================================
//           AwSettings::SetSaveFormDataRunnable
//===============================================================

AwSettings::SetSaveFormDataRunnable::SetSaveFormDataRunnable(
    /* [in] */ AwSettings* owner)
    : mOwner(owner)
{
}

ECode AwSettings::SetSaveFormDataRunnable::Run()
{
    if (mNativeAwSettings != 0) {
        NativeUpdateFormDataPreferencesLocked(mNativeAwSettings);
    }

    return NOERROR;
}

//===============================================================
//            AwSettings::SetUserAgentStringRunnable
//===============================================================

AwSettings::SetUserAgentStringRunnable::SetUserAgentStringRunnable(
    /* [in] */ AwSettings* owner)
    : mOwner(owner)
{
}

ECode AwSettings::SetUserAgentStringRunnable::Run()
{
    if (mNativeAwSettings != 0) {
        NativeUpdateUserAgentLocked(mNativeAwSettings);
    }
    return NOERROR;
}

//===============================================================
//          AwSettings::SetLoadWithOverviewModeRunnable
//===============================================================

AwSettings::SetLoadWithOverviewModeRunnable::SetLoadWithOverviewModeRunnable(
    /* [in] */ AwSettings* owner)
    : mOwner(owner)
{
}

ECode AwSettings::SetLoadWithOverviewModeRunnable::Run()
{
    if (mNativeAwSettings != 0) {
        UpdateWebkitPreferencesOnUiThreadLocked();
        NativeResetScrollAndScaleState(mNativeAwSettings);
    }
    return NOERROR;
}

//===============================================================
//       AwSettings::OnGestureZoomSupportChangedRunnable
//===============================================================

AwSettings::OnGestureZoomSupportChangedRunnable::OnGestureZoomSupportChangedRunnable(
    /* [in] */ AwSettings* owner,
    /* [in] */ Boolean supportsDoubleTapZoom,
    /* [in] */ Boolean supportsMultiTouchZoom)
    : mOwner(owner)
    , mSupportsDoubleTapZoom(supportsDoubleTapZoom)
    , mSupportsMultiTouchZoom(supportsMultiTouchZoom)
{
}

ECode AwSettings::OnGestureZoomSupportChangedRunnable::Run()
{
    Object::Autolock lock(mAwSettingsLock);
    if (mZoomChangeListener != NULL) {
        mZoomChangeListener->OnGestureZoomSupportChanged(
                supportsDoubleTapZoom, supportsMultiTouchZoom);
    }

    return NOERROR;
}

//============================================================================
//           AwSettings::SetVideoOverlayForEmbeddedVideoEnabledRunnable
//============================================================================

AwSettings::SetVideoOverlayForEmbeddedVideoEnabledRunnable::SetVideoOverlayForEmbeddedVideoEnabledRunnable(
    /* [in] */ AwSettings* owner)
    : mOwner(owner)
{
}

ECode AwSettings::SetVideoOverlayForEmbeddedVideoEnabledRunnable::Run()
{
    if (mOwner->mNativeAwSettings != 0) {
        mOwner->NativeUpdateRendererPreferencesLocked(mOwner->mNativeAwSettings);
    }

    return NOERROR;
}

//===============================================================
//              AwSettings::LazyDefaultUserAgent
//===============================================================

const String AwSettings::LazyDefaultUserAgent::sInstance = NativeGetDefaultUserAgent();

//===============================================================
//             AwSettings::EventHandler::InnerHandler
//===============================================================

AwSettings::EventHandler::InnerHandler::InnerHandler(
    /* [in] */ EventHandler* owner)
    : mOwner(owner)
{
}

ECode AwSettings::EventHandler::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case RUN_RUNNABLE_BLOCKING:
            {
                Object::Autolock lock(mAwSettingsLock);
                if (mNativeAwSettings != 0) {
                    AutoPtr<IRunnable> runnable;
                    msg->GetObj((IInterface**)&runnable);
                    runnable->Run();
                }
                mSynchronizationPending = FALSE;
                mAwSettingsLock.NotifyAll();
            }
            break;
    }

    return NOERROR;
}

//===============================================================
//            AwSettings::EventHandler::InnerRunnable
//===============================================================

AwSettings::EventHandler::InnerRunnable::InnerRunnable(
    /* [in] */ EventHandler* owner)
    : mOwner(owner)
{
}

ECode AwSettings::EventHandler::InnerRunnable::Run()
{
    mOwner->mOwner->UpdateWebkitPreferencesOnUiThreadLocked();
    return NOERROR;
}

//===============================================================
//                   AwSettings::EventHandler
//===============================================================

const Int32 AwSettings::EventHandler::RUN_RUNNABLE_BLOCKING;

AwSettings::EventHandler::EventHandler(
    /* [in] */ AwSettings* owner)
    : mOwner(owner)
    , mSynchronizationPending(FALSE)
{
}

void AwSettings::EventHandler::BindUiThread()
{
    if (mHandler != NULL) return;

    AutoPtr<ILooper> looper = ThreadUtils::GetUiThreadLooper();
    mHandler = new InnerHandler(this, looper);
}

void AwSettings::EventHandler::RunOnUiThreadBlockingAndLocked(
    /* [in] */ IRunnable* r)
{
    assert(Thread::HoldsLock(mAwSettingsLock));

    if (mHandler == NULL) return;

    if (ThreadUtils::RunningOnUiThread()) {
        r->Run();
    }
    else {
        assert(!mSynchronizationPending);
        mSynchronizationPending = TRUE;
        AutoPtr<IMessageHelper> msgHelper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&msgHelper);
        AutoPtr<IMessage> msg;
        msgHelper->Obtain(NULL, RUN_RUNNABLE_BLOCKING, r, (IMessage**)&msg);
        mHandler->SendMessage(msg);
        // try {
            while (mSynchronizationPending) {
                mAwSettingsLock.Wait();
            }
        // } catch (InterruptedException e) {
        //     Log.e(TAG, "Interrupted waiting a Runnable to complete", e);
        //     mSynchronizationPending = false;
        // }
    }
}

void AwSettings::EventHandler::MaybePostOnUiThread(
    /* [in] */ IRunnable* r)
{
    if (mHandler != NULL) {
        mHandler->Post(r);
    }
}

void AwSettings::EventHandler::UpdateWebkitPreferencesLocked()
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this);
    RunOnUiThreadBlockingAndLocked(runnable);
}

//===============================================================
//                         AwSettings
//===============================================================

const Int32 AwSettings::MIXED_CONTENT_ALWAYS_ALLOW;
const Int32 AwSettings::MIXED_CONTENT_NEVER_ALLOW;
const Int32 AwSettings::MIXED_CONTENT_COMPATIBILITY_MODE;
const String AwSettings::TAG("AwSettings");
static const Object AwSettings::sGlobalContentSettingsLock;

Boolean AwSettings::sAppCachePathIsSet = FALSE;
const Int32 AwSettings::MINIMUM_FONT_SIZE;
const Int32 AwSettings::MAXIMUM_FONT_SIZE;

AwSettings::AwSettings(
    /* [in] */ IContext* context,
    /* [in] */ Boolean isAccessFromFileURLsGrantedByDefault,
    /* [in] */ Boolean supportsLegacyQuirks)
    : mDIPScale(1.0)
    , mLayoutAlgorithm(NARROW_COLUMNS)
    , mTextSizePercent(100)
    , mStandardFontFamily("sans-serif")
    , mFixedFontFamily("monospace")
    , mSansSerifFontFamily("sans-serif")
    , mSerifFontFamily("serif")
    , mCursiveFontFamily("cursive")
    , mFantasyFontFamily("fantasy")
    , mMinimumFontSize(8)
    , mMinimumLogicalFontSize(8)
    , mDefaultFontSize(16)
    , mDefaultFixedFontSize(13)
    , mLoadsImagesAutomatically(TRUE)
    , mImagesEnabled(TRUE)
    , mJavaScriptEnabled(FALSE)
    , mAllowUniversalAccessFromFileURLs(FALSE)
    , mAllowFileAccessFromFileURLs(FALSE)
    , mJavaScriptCanOpenWindowsAutomatically(FALSE)
    , mSupportMultipleWindows(FALSE)
    , mPluginState(OFF)
    , mAppCacheEnabled(FALSE)
    , mDomStorageEnabled(FALSE)
    , mDatabaseEnabled(FALSE)
    , mUseWideViewport(FALSE)
    , mZeroLayoutHeightDisablesViewportQuirk(FALSE)
    , mForceZeroLayoutHeight(FALSE)
    , mLoadWithOverviewMode(FALSE)
    , mMediaPlaybackRequiresUserGesture(TRUE)
    , mInitialPageScalePercent(0)
    , mSpatialNavigationEnabled(FALSE)
    , mEnableSupportedHardwareAcceleratedFeatures(FALSE)
    , mMixedContentMode(MIXED_CONTENT_NEVER_ALLOW)
    , mVideoOverlayForEmbeddedVideoEnabled(FALSE)
    , mAcceptThirdPartyCookies(FALSE)
    , mBlockNetworkLoads(FALSE)
    , mAllowContentUrlAccess(TRUE)
    , mAllowFileUrlAccess(TRUE)
    , mCacheMode(WebSettings::LOAD_DEFAULT)
    , mShouldFocusFirstNode(TRUE)
    , mGeolocationEnabled(TRUE)
    , mAutoCompleteEnabled(TRUE)
    , mFullscreenSupported(FALSE)
    , mSupportZoom(TRUE)
    , mBuiltInZoomControls(FALSE)
    , mDisplayZoomControls(TRUE)
    , mNativeAwSettings(0)
{
    Int32 permission;
    context->CheckPermission(
                android::Manifest::permission::INTERNET,
                Process::MyPid(),
                Process::MyUid(), &permission);
    Boolean hasInternetPermission permission == PackageManager.PERMISSION_GRANTED;

    {
        Object::Autolock lock(mAwSettingsLock);
        mHasInternetPermission = hasInternetPermission;
        mBlockNetworkLoads = !hasInternetPermission;
        mEventHandler = new EventHandler();
        if (isAccessFromFileURLsGrantedByDefault) {
            mAllowUniversalAccessFromFileURLs = TRUE;
            mAllowFileAccessFromFileURLs = TRUE;
        }

        mDefaultTextEncoding = AwResource::GetDefaultTextEncoding();
        mUserAgent = LazyDefaultUserAgent::sInstance;

        // Best-guess a sensible initial value based on the features supported on the device.
        AutoPtr<IPackageManager> pm;
        context->GetPackageManager((IPackageManager**)&pm);
        pm->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN, &mSpatialNavigationEnabled);

        // Respect the system setting for password echoing.
        AutoPtr<IContentResolver> contentResolver;
        context->GetContentResolver((IContentResolver**)&contentResolver);
        AutoPtr<ISettingsSystem> settingsSystem;
        CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingsSystem);
        Int32 value;
        settingsSystem->GetInt32(contentResolver, ISettingsSystem::TEXT_SHOW_PASSWORD, 1, &value);
        mPasswordEchoEnabled = value == 1;

        // By default, scale the text size by the system font scale factor. Embedders
        // may override this by invoking setTextZoom().
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        AutoPtr<IConfiguration> configuration;
        resources->GetConfiguration((IConfiguration**)&configuration);
        Float* fontScale;
        configuration->GetFontScale(&fontScale);
        mTextSizePercent *= fontScale;

        mSupportLegacyQuirks = supportsLegacyQuirks;
    }
    // Defer initializing the native side until a native WebContents instance is set.
}

//@CalledByNative
void AwSettings::NativeAwSettingsGone(
    /* [in] */ Int32 nativeAwSettings)
{
    assert(mNativeAwSettings != 0 && mNativeAwSettings == nativeAwSettings);
    mNativeAwSettings = 0;
}

//@CalledByNative
Double AwSettings::GetDIPScaleLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mDIPScale;
}

void AwSettings::SetDIPScale(
    /* [in] */ Double dipScale)
{
    Object::Autolock lock(mAwSettingsLock);
    mDIPScale = dipScale;
    // TODO(joth): This should also be synced over to native side, but right now
    // the setDIPScale call is always followed by a setWebContents() which covers this.
}

void AwSettings::SetZoomListener(
    /* [in] */ ZoomSupportChangeListener* zoomChangeListener)
{
    Object::Autolock lock(mAwSettingsLock)
    mZoomChangeListener = zoomChangeListener;
}

void AwSettings::SetWebContents(
    /* [in] */ Int64 nativeWebContents)
{
    Object::Autolock lock(mAwSettingsLock)
    if (mNativeAwSettings != 0) {
        NativeDestroy(mNativeAwSettings);
        assert(mNativeAwSettings == 0);  // nativeAwSettingsGone should have been called.
    }
    if (nativeWebContents != 0) {
        mEventHandler->BindUiThread();
        mNativeAwSettings = NativeInit(nativeWebContents);
        UpdateEverythingLocked();
    }
}

void AwSettings::UpdateEverythingLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    assert(mNativeAwSettings != 0);
    nativeUpdateEverythingLocked(mNativeAwSettings);
    OnGestureZoomSupportChanged(
            SupportsDoubleTapZoomLocked(), SupportsMultiTouchZoomLocked());
}

/**
 * See {@link android.webkit.WebSettings#setBlockNetworkLoads}.
 */
void AwSettings::SetBlockNetworkLoads(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (!flag && !mHasInternetPermission) {
        // throw new SecurityException("Permission denied - " +
        //         "application missing INTERNET permission");
        assert(0);
    }
    mBlockNetworkLoads = flag;
}

/**
 * See {@link android.webkit.WebSettings#getBlockNetworkLoads}.
 */
Boolean AwSettings::GetBlockNetworkLoads()
{
    Object::Autolock lock(mAwSettingsLock);
    return mBlockNetworkLoads;
}

/**
 * Enable/disable third party cookies for an AwContents
 * @param accept true if we should accept third party cookies
 */
void AwSettings::SetAcceptThirdPartyCookies(
    /* [in] */ Boolean accept)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mAcceptThirdPartyCookies != accept) {
        mAcceptThirdPartyCookies = accept;
    }
}

/**
 * Return whether third party cookies are enabled for an AwContents
 * @return true if accept third party cookies
 */
Boolean AwSettings::GetAcceptThirdPartyCookies()
{
    Object::Autolock lock(mAwSettingsLock);
    return mAcceptThirdPartyCookies;
}

/**
 * See {@link android.webkit.WebSettings#setAllowFileAccess}.
 */
void AwSettings::SetAllowFileAccess(
    /* [in] */ Boolean allow)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mAllowFileUrlAccess != allow) {
        mAllowFileUrlAccess = allow;
    }
}

/**
 * See {@link android.webkit.WebSettings#getAllowFileAccess}.
 */
Boolean AwSettings::GetAllowFileAccess()
{
    Object::Autolock lock(mAwSettingsLock);
    return mAllowFileUrlAccess;
}

/**
 * See {@link android.webkit.WebSettings#setAllowContentAccess}.
 */
void AwSettings::SetAllowContentAccess(
    /* [in] */ Boolean allow)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mAllowContentUrlAccess != allow) {
        mAllowContentUrlAccess = allow;
    }
}

/**
 * See {@link android.webkit.WebSettings#getAllowContentAccess}.
 */
Boolean AwSettings::GetAllowContentAccess()
{
    Object::Autolock lock(mAwSettingsLock);
    return mAllowContentUrlAccess;
}

/**
 * See {@link android.webkit.WebSettings#setCacheMode}.
 */
void AwSettings::SetCacheMode(
    /* [in] */ Int32 mode)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mCacheMode != mode) {
        mCacheMode = mode;
    }
}

/**
 * See {@link android.webkit.WebSettings#getCacheMode}.
 */
Int32 AwSettings::GetCacheMode()
{
    Object::Autolock lock(mAwSettingsLock);
    return mCacheMode;
}

/**
 * See {@link android.webkit.WebSettings#setNeedInitialFocus}.
 */
void AwSettings::SetShouldFocusFirstNode(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    mShouldFocusFirstNode = flag;
}

/**
 * See {@link android.webkit.WebView#setInitialScale}.
 */
void AwSettings::SetInitialPageScale(
    /* [in] */ Float scaleInPercent)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mInitialPageScalePercent != scaleInPercent) {
        mInitialPageScalePercent = scaleInPercent;
        Autolock<IRunnable> runnable = new SetInitialPageScaleRunnable(this);
        mEventHandler->RunOnUiThreadBlockingAndLocked(runnable);
    }
}

//@CalledByNative
Float AwSettings::GetInitialPageScalePercentLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mInitialPageScalePercent;
}

void AwSettings::SetSpatialNavigationEnabled(
    /* [in] */ Boolean enable)
{
    Object::Autolock lock(mAwSettingsLock) ;
    if (mSpatialNavigationEnabled != enable) {
        mSpatialNavigationEnabled = enable;
        mEventHandler.updateWebkitPreferencesLocked();
    }
}

//@CalledByNative
Boolean AwSettings::GetSpatialNavigationLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mSpatialNavigationEnabled;
}

void AwSettings::SetEnableSupportedHardwareAcceleratedFeatures(
    /* [in] */ Boolean enable)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mEnableSupportedHardwareAcceleratedFeatures != enable) {
        mEnableSupportedHardwareAcceleratedFeatures = enable;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

//@CalledByNative
Boolean AwSettings::GetEnableSupportedHardwareAcceleratedFeaturesLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mEnableSupportedHardwareAcceleratedFeatures;
}

void AwSettings::SetFullscreenSupported(
    /* [in] */ Boolean supported)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mFullscreenSupported != supported) {
        mFullscreenSupported = supported;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

//@CalledByNative
Boolean AwSettings::GetFullscreenSupportedLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mFullscreenSupported;
}

/**
 * See {@link android.webkit.WebSettings#setNeedInitialFocus}.
 */
Boolean AwSettings::ShouldFocusFirstNode()
{
    Object::Autolock lock(mAwSettingsLock);
    return mShouldFocusFirstNode;
}

/**
 * See {@link android.webkit.WebSettings#setGeolocationEnabled}.
 */
void AwSettings::SetGeolocationEnabled(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mGeolocationEnabled != flag) {
        mGeolocationEnabled = flag;
    }
}

/**
 * @return Returns if geolocation is currently enabled.
 */
Boolean AwSettings::GetGeolocationEnabled()
{
    Object::Autolock lock(mAwSettingsLock);
    return mGeolocationEnabled;
}

/**
 * See {@link android.webkit.WebSettings#setSaveFormData}.
 */
void AwSettings::SetSaveFormData(
    /* [in] */ Boolean enable)
{
    Object::Autolock lock(mAwSettingsLock);
        if (mAutoCompleteEnabled != enable) {
            mAutoCompleteEnabled = enable;
            AutoPtr<IRunnable> runnable =  new SetSaveFormDataRunnable(this);
            mEventHandler->RunOnUiThreadBlockingAndLocked(runnable);
        }
    }
}

/**
 * See {@link android.webkit.WebSettings#getSaveFormData}.
 */
Boolean AwSettings::GetSaveFormData()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetSaveFormDataLocked();
}

//@CalledByNative
Boolean AwSettings::GetSaveFormDataLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mAutoCompleteEnabled;
}

/**
 * @returns the default User-Agent used by each ContentViewCore instance, i.e. unless
 * overridden by {@link #setUserAgentString()}
 */
String AwSettings::GetDefaultUserAgent()
{
    return LazyDefaultUserAgent::sInstance;
}

/**
 * See {@link android.webkit.WebSettings#setUserAgentString}.
 */
void AwSettings::SetUserAgentString(String ua)
{
    Object::Autolock lock(mAwSettingsLock);
    const String oldUserAgent = mUserAgent;
    if (ua == NULL || ua.GetLength() == 0) {
        mUserAgent = LazyDefaultUserAgent::sInstance;
    }
    else {
        mUserAgent = ua;
    }

    if (!oldUserAgent.Equals(mUserAgent)) {
        Autolock<IRunnable> runnable = new SetUserAgentStringRunnable(this);
        mEventHandler->RunOnUiThreadBlockingAndLocked(runnable);
    }
}

/**
 * See {@link android.webkit.WebSettings#getUserAgentString}.
 */
String AwSettings::GetUserAgentString()
{
    Object::Autolock lock(mAwSettingsLock);
    return getUserAgentLocked();
}

//@CalledByNative
String AwSettings::GetUserAgentLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mUserAgent;
}

/**
 * See {@link android.webkit.WebSettings#setLoadWithOverviewMode}.
 */
void AwSettings::SetLoadWithOverviewMode(
    /* [in] */ Boolean overview)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mLoadWithOverviewMode != overview) {
        mLoadWithOverviewMode = overview;
        AutoPtr<IRunnable> runnable = new SetLoadWithOverviewModeRunnable(this);
        mEventHandler->RunOnUiThreadBlockingAndLocked(runnable);
    }
}

/**
 * See {@link android.webkit.WebSettings#getLoadWithOverviewMode}.
 */
Boolean AwSettings::GetLoadWithOverviewMode()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetLoadWithOverviewModeLocked();
}

//@CalledByNative
Boolean AwSettings::GetLoadWithOverviewModeLocked()
{
    assert Thread.holdsLock(mAwSettingsLock);
    return mLoadWithOverviewMode;
}

/**
 * See {@link android.webkit.WebSettings#setTextZoom}.
 */
void AwSettings::SetTextZoom(
    /* [in] */ Int32 textZoom)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mTextSizePercent != textZoom) {
        mTextSizePercent = textZoom;
        mEventHandler.updateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getTextZoom}.
 */
Int32 AwSettings::GetTextZoom()
{
    Object::Autolock lock(mAwSettingsLock);
    return getTextSizePercentLocked();
}

//@CalledByNative
Int32 AwSettings::GetTextSizePercentLocked()
{
    assert(Thread::holdsLock(mAwSettingsLock));
    return mTextSizePercent;
}

/**
 * See {@link android.webkit.WebSettings#setStandardFontFamily}.
 */
void AwSettings::SetStandardFontFamily(
    /* [in] */ String font)
{
    Object::Autolock lock(mAwSettingsLock);
    if (font != NULL && !mStandardFontFamily.Equals(font)) {
        mStandardFontFamily = font;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getStandardFontFamily}.
 */
String AwSettings::GetStandardFontFamily()
{
    Object::Autolock lock(mAwSettingsLock);
    return getStandardFontFamilyLocked();
}

//@CalledByNative
String AwSettings::GetStandardFontFamilyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mStandardFontFamily;
}

/**
 * See {@link android.webkit.WebSettings#setFixedFontFamily}.
 */
void AwSettings::SetFixedFontFamily(
    /* [in] */ String font)
{
    Object::Autolock lock(mAwSettingsLock);
    if (font != NULL && !mFixedFontFamily.Equals(font)) {
        mFixedFontFamily = font;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getFixedFontFamily}.
 */
String AwSettings::GetFixedFontFamily()
{
    Object::Autolock lock(mAwSettingsLock);
    return getFixedFontFamilyLocked();
}

//@CalledByNative
String AwSettings::GetFixedFontFamilyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mFixedFontFamily;
}

/**
 * See {@link android.webkit.WebSettings#setSansSerifFontFamily}.
 */
void AwSettings::SetSansSerifFontFamily(
    /* [in] */ String font)
{
    Object::Autolock lock(mAwSettingsLock);
    if (font != NULL && !mSansSerifFontFamily.Equals(font)) {
        mSansSerifFontFamily = font;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getSansSerifFontFamily}.
 */
String AwSettings::GetSansSerifFontFamily()
{
    synchronized (mAwSettingsLock) {
        return getSansSerifFontFamilyLocked();
    }
}

//@CalledByNative
String AwSettings::GetSansSerifFontFamilyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mSansSerifFontFamily;
}

/**
 * See {@link android.webkit.WebSettings#setSerifFontFamily}.
 */
void AwSettings::SetSerifFontFamily(
    /* [in] */ String font)
{
    Object::Autolock lock(mAwSettingsLock);
    if (font != NULL && !mSerifFontFamily.Equals(font)) {
        mSerifFontFamily = font;
        mEventHandler.UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getSerifFontFamily}.
 */
String AwSettings::GetSerifFontFamily()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetSerifFontFamilyLocked();
}

//@CalledByNative
String AwSettings::GetSerifFontFamilyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mSerifFontFamily;
}

/**
 * See {@link android.webkit.WebSettings#setCursiveFontFamily}.
 */
void AwSettings::SetCursiveFontFamily(
    /* [in] */ String font)
{
    Object::Autolock lock(mAwSettingsLock);
    if (font != NULL && !mCursiveFontFamily.Equals(font)) {
        mCursiveFontFamily = font;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getCursiveFontFamily}.
 */
String AwSettings::GetCursiveFontFamily()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetCursiveFontFamilyLocked();
}

//@CalledByNative
String AwSettings::GetCursiveFontFamilyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mCursiveFontFamily;
}

/**
 * See {@link android.webkit.WebSettings#setFantasyFontFamily}.
 */
void AwSettings::SetFantasyFontFamily(
    /* [in] */ String font)
{
    Object::Autolock lock(mAwSettingsLock);
    if (font != null && !mFantasyFontFamily.equals(font)) {
        mFantasyFontFamily = font;
        mEventHandler.updateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getFantasyFontFamily}.
 */
String AwSettings::GetFantasyFontFamily()
{
    Object::Autolock lock(mAwSettingsLock);
    return getFantasyFontFamilyLocked();
}

//@CalledByNative
String AwSettings::GetFantasyFontFamilyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mFantasyFontFamily;
}

/**
 * See {@link android.webkit.WebSettings#setMinimumFontSize}.
 */
void AwSettings::SetMinimumFontSize(
    /* [in] */ Int32 size)
{
    Object::Autolock lock(mAwSettingsLock);
    size = ClipFontSize(size);
    if (mMinimumFontSize != size) {
        mMinimumFontSize = size;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getMinimumFontSize}.
 */
Int32 AwSettings::GetMinimumFontSize()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetMinimumFontSizeLocked();
}

//@CalledByNative
Int32 AwSettings::GetMinimumFontSizeLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mMinimumFontSize;
}

/**
 * See {@link android.webkit.WebSettings#setMinimumLogicalFontSize}.
 */
void AwSettings::SetMinimumLogicalFontSize(
    /* [in] */ Int32 size)
{
    Object::Autolock lock(mAwSettingsLock);
    size = ClipFontSize(size);
    if (mMinimumLogicalFontSize != size) {
        mMinimumLogicalFontSize = size;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getMinimumLogicalFontSize}.
 */
Int32 AwSettings::GetMinimumLogicalFontSize()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetMinimumLogicalFontSizeLocked();
}

//@CalledByNative
Int32 AwSettings::GetMinimumLogicalFontSizeLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mMinimumLogicalFontSize;
}

/**
 * See {@link android.webkit.WebSettings#setDefaultFontSize}.
 */
void AwSettings::SetDefaultFontSize(
    /* [in] */ Int32 size)
{
    Object::Autolock lock(mAwSettingsLock);
    size = ClipFontSize(size);
    if (mDefaultFontSize != size) {
        mDefaultFontSize = size;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getDefaultFontSize}.
 */
Int32 AwSettings::GetDefaultFontSize()
{
    Object::Autolock lock(mAwSettingsLock);
    return getDefaultFontSizeLocked();
}

//@CalledByNative
Int32 AwSettings::GetDefaultFontSizeLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mDefaultFontSize;
}

/**
 * See {@link android.webkit.WebSettings#setDefaultFixedFontSize}.
 */
void AwSettings::SetDefaultFixedFontSize(
    /* [in] */ Int32 size)
{
    Object::Autolock lock(mAwSettingsLock);
    size = clipFontSize(size);
    if (mDefaultFixedFontSize != size) {
        mDefaultFixedFontSize = size;
        mEventHandler.updateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getDefaultFixedFontSize}.
 */
Int32 AwSettings::GetDefaultFixedFontSize()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetDefaultFixedFontSizeLocked();
}

//@CalledByNative
Int32 AwSettings::GetDefaultFixedFontSizeLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mDefaultFixedFontSize;
}

/**
 * See {@link android.webkit.WebSettings#setJavaScriptEnabled}.
 */
void AwSettings::SetJavaScriptEnabled(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);{
    if (mJavaScriptEnabled != flag) {
        mJavaScriptEnabled = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#setAllowUniversalAccessFromFileURLs}.
 */
void AwSettings::SetAllowUniversalAccessFromFileURLs(boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mAllowUniversalAccessFromFileURLs != flag) {
        mAllowUniversalAccessFromFileURLs = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#setAllowFileAccessFromFileURLs}.
 */
void AwSettings::SetAllowFileAccessFromFileURLs(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mAllowFileAccessFromFileURLs != flag) {
        mAllowFileAccessFromFileURLs = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#setLoadsImagesAutomatically}.
 */
void AwSettings::SetLoadsImagesAutomatically(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mLoadsImagesAutomatically != flag) {
        mLoadsImagesAutomatically = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getLoadsImagesAutomatically}.
 */
Boolean AwSettings::GetLoadsImagesAutomatically()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetLoadsImagesAutomaticallyLocked();
}

//@CalledByNative
Boolean AwSettings::GetLoadsImagesAutomaticallyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mLoadsImagesAutomatically;
}

/**
 * See {@link android.webkit.WebSettings#setImagesEnabled}.
 */
void AwSettings::SetImagesEnabled(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mImagesEnabled != flag) {
        mImagesEnabled = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getImagesEnabled}.
 */
Boolean AwSettings::GetImagesEnabled()
{
    Object::Autolock lock(mAwSettingsLock);
    return mImagesEnabled;
}

//@CalledByNative
Boolean AwSettings::GetImagesEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mImagesEnabled;
}

/**
 * See {@link android.webkit.WebSettings#getJavaScriptEnabled}.
 */
Boolean AwSettings::GetJavaScriptEnabled()
{
    Object::Autolock lock(mAwSettingsLock);
    return mJavaScriptEnabled;
}

//@CalledByNative
Boolean AwSettings::GetJavaScriptEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mJavaScriptEnabled;
}

/**
 * See {@link android.webkit.WebSettings#getAllowUniversalAccessFromFileURLs}.
 */
Boolean AwSettings::GetAllowUniversalAccessFromFileURLs()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetAllowUniversalAccessFromFileURLsLocked();
}

//@CalledByNative
Boolean AwSettings::GetAllowUniversalAccessFromFileURLsLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mAllowUniversalAccessFromFileURLs;
}

/**
 * See {@link android.webkit.WebSettings#getAllowFileAccessFromFileURLs}.
 */
Boolean AwSettings::GetAllowFileAccessFromFileURLs()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetAllowFileAccessFromFileURLsLocked();
}

//@CalledByNative
Boolean AwSettings::GetAllowFileAccessFromFileURLsLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mAllowFileAccessFromFileURLs;
}

/**
 * See {@link android.webkit.WebSettings#setPluginsEnabled}.
 */
void AwSettings::SetPluginsEnabled(boolean flag)
{
    SetPluginState(flag ? PluginState.ON : PluginState.OFF);
}

/**
 * See {@link android.webkit.WebSettings#setPluginState}.
 */
void AwSettings::SetPluginState(PluginState state)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mPluginState != state) {
        mPluginState = state;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getPluginsEnabled}.
 */
Boolean AwSettings::GetPluginsEnabled()
{
    Object::Autolock lock(mAwSettingsLock);
    return mPluginState == PluginState.ON;
}

/**
 * Return true if plugins are disabled.
 * @return True if plugins are disabled.
 */
//@CalledByNative
Boolean AwSettings::GetPluginsDisabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mPluginState == PluginState.OFF;
}

/**
 * See {@link android.webkit.WebSettings#getPluginState}.
 */
PluginState AwSettings::GetPluginState()
{
    Object::Autolock lock(mAwSettingsLock);
    return mPluginState;
}


/**
 * See {@link android.webkit.WebSettings#setJavaScriptCanOpenWindowsAutomatically}.
 */
void AwSettings::SetJavaScriptCanOpenWindowsAutomatically(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mJavaScriptCanOpenWindowsAutomatically != flag) {
        mJavaScriptCanOpenWindowsAutomatically = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getJavaScriptCanOpenWindowsAutomatically}.
 */
Boolean AwSettings::GetJavaScriptCanOpenWindowsAutomatically()
{
    Object::Autolock lock(mAwSettingsLock);
    return getJavaScriptCanOpenWindowsAutomaticallyLocked();
}

//@CalledByNative
Boolean AwSettings::GetJavaScriptCanOpenWindowsAutomaticallyLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mJavaScriptCanOpenWindowsAutomatically;
}

/**
 * See {@link android.webkit.WebSettings#setLayoutAlgorithm}.
 */
void AwSettings::SetLayoutAlgorithm(
    /* [in] */ LayoutAlgorithm l)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mLayoutAlgorithm != l) {
        mLayoutAlgorithm = l;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getLayoutAlgorithm}.
 */
LayoutAlgorithm AwSettings::GetLayoutAlgorithm()
{
    Object::Autolock lock(mAwSettingsLock);
    return mLayoutAlgorithm;
}

/**
 * Gets whether Text Auto-sizing layout algorithm is enabled.
 *
 * @return true if Text Auto-sizing layout algorithm is enabled
 */
//@CalledByNative
Boolean AwSettings::GetTextAutosizingEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mLayoutAlgorithm == TEXT_AUTOSIZING;
}

/**
 * See {@link android.webkit.WebSettings#setSupportMultipleWindows}.
 */
void AwSettings::SetSupportMultipleWindows(
    /* [in] */ Boolean support)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mSupportMultipleWindows != support) {
        mSupportMultipleWindows = support;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#supportMultipleWindows}.
 */
Boolean AwSettings::SupportMultipleWindows()
{
    Object::Autolock lock(mAwSettingsLock);
    return mSupportMultipleWindows;
}

//@CalledByNative
Boolean AwSettings::GetSupportMultipleWindowsLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mSupportMultipleWindows;
}

//@CalledByNative
Boolean AwSettings::GetSupportLegacyQuirksLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mSupportLegacyQuirks;
}

/**
 * See {@link android.webkit.WebSettings#setUseWideViewPort}.
 */
void AwSettings::SetUseWideViewPort(
    /* [in] */ Boolean use)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mUseWideViewport != use) {
        mUseWideViewport = use;
        OnGestureZoomSupportChanged(
                SupportsDoubleTapZoomLocked(), SupportsMultiTouchZoomLocked());
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getUseWideViewPort}.
 */
Boolean AwSettings::GetUseWideViewPort()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetUseWideViewportLocked();
}

//@CalledByNative
Boolean AwSettings::GetUseWideViewportLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mUseWideViewport;
}

void AwSettings::SetZeroLayoutHeightDisablesViewportQuirk(
    /* [in] */ Boolean enabled)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mZeroLayoutHeightDisablesViewportQuirk != enabled) {
        mZeroLayoutHeightDisablesViewportQuirk = enabled;
        mEventHandler.updateWebkitPreferencesLocked();
    }
}

Boolean AwSettings::GetZeroLayoutHeightDisablesViewportQuirk()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetZeroLayoutHeightDisablesViewportQuirkLocked();
}

//@CalledByNative
Boolean AwSettings::GetZeroLayoutHeightDisablesViewportQuirkLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mZeroLayoutHeightDisablesViewportQuirk;
}

void AwSettings::SetForceZeroLayoutHeight(
    /* [in] */ Boolean enabled)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mForceZeroLayoutHeight != enabled) {
        mForceZeroLayoutHeight = enabled;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

Boolean AwSettings::GetForceZeroLayoutHeight()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetForceZeroLayoutHeightLocked();
}

//@CalledByNative
Boolean AwSettings::GetForceZeroLayoutHeightLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mForceZeroLayoutHeight;
}

//@CalledByNative
Boolean AwSettings::GetPasswordEchoEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mPasswordEchoEnabled;
}

/**
 * See {@link android.webkit.WebSettings#setAppCacheEnabled}.
 */
void AwSettings::SetAppCacheEnabled(boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mAppCacheEnabled != flag) {
        mAppCacheEnabled = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#setAppCachePath}.
 */
void AwSettings::SetAppCachePath(
    /* [in] */ String path)
{
    Boolean needToSync = FALSE;
    {
        Object::Autolock lock(sGlobalContentSettingsLock)
        // AppCachePath can only be set once.
        if (!sAppCachePathIsSet && path != NULL && !path.IsEmpty()) {
            sAppCachePathIsSet = TRUE;
            needToSync = TRUE;
        }
    }
    // The obvious problem here is that other WebViews will not be updated,
    // until they execute synchronization from Java to the native side.
    // But this is the same behaviour as it was in the legacy WebView.
    if (needToSync) {
        Object::Autolock lock(mAwSettingsLock)
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * Gets whether Application Cache is enabled.
 *
 * @return true if Application Cache is enabled
 */
//@CalledByNative
Boolean AwSettings::GetAppCacheEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    if (!mAppCacheEnabled) {
        return FALSE;
    }

    {
        Object::Autolock lock(sGlobalContentSettingsLock);
        return sAppCachePathIsSet;
    }
}

/**
 * See {@link android.webkit.WebSettings#setDomStorageEnabled}.
 */
void AwSettings::SetDomStorageEnabled(
    /* [in] */ Boolean flag)
{
    Object::Autolock lock(mAwSettingsLock)
    {
        if (mDomStorageEnabled != flag) {
            mDomStorageEnabled = flag;
            mEventHandler->UpdateWebkitPreferencesLocked();
        }
    }
}

/**
 * See {@link android.webkit.WebSettings#getDomStorageEnabled}.
 */
Boolean AwSettings::GetDomStorageEnabled()
{
    Object::Autolock lock(mAwSettingsLock);
    return mDomStorageEnabled;
}

//@CalledByNative
Boolean AwSettings::GetDomStorageEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mDomStorageEnabled;
}

/**
 * See {@link android.webkit.WebSettings#setDatabaseEnabled}.
 */
void AwSettings::SetDatabaseEnabled(boolean flag)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mDatabaseEnabled != flag) {
        mDatabaseEnabled = flag;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getDatabaseEnabled}.
 */
Boolean AwSettings::GetDatabaseEnabled()
{
    Object::Autolock lock(mAwSettingsLock);
    return mDatabaseEnabled;
}

//@CalledByNative
Boolean AwSettings::GetDatabaseEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mDatabaseEnabled;
}

/**
 * See {@link android.webkit.WebSettings#setDefaultTextEncodingName}.
 */
void AwSettings::SetDefaultTextEncodingName(
    /* [in] */ String encoding)
{
    Object::Autolock lock(mAwSettingsLock);
    if (encoding != NULL && !mDefaultTextEncoding.Equals(encoding)) {
        mDefaultTextEncoding = encoding;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getDefaultTextEncodingName}.
 */
String AwSettings::GetDefaultTextEncodingName()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetDefaultTextEncodingLocked();
}

//@CalledByNative
String AwSettings::GetDefaultTextEncodingLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mDefaultTextEncoding;
}

/**
 * See {@link android.webkit.WebSettings#setMediaPlaybackRequiresUserGesture}.
 */
void AwSettings::SetMediaPlaybackRequiresUserGesture(
    /* [in] */ Boolean require)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mMediaPlaybackRequiresUserGesture != require) {
        mMediaPlaybackRequiresUserGesture = require;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getMediaPlaybackRequiresUserGesture}.
 */
Boolean AwSettings::GetMediaPlaybackRequiresUserGesture()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetMediaPlaybackRequiresUserGestureLocked();
}

//@CalledByNative
Boolean AwSettings::GetMediaPlaybackRequiresUserGestureLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mMediaPlaybackRequiresUserGesture;
}

/**
 * See {@link android.webkit.WebSettings#setDefaultVideoPosterURL}.
 */
void AwSettings::SetDefaultVideoPosterURL(
    /* [in] */ String url)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mDefaultVideoPosterURL != NULL && !mDefaultVideoPosterURL.Equals(url) ||
            mDefaultVideoPosterURL == NULL && url != NULL) {
        mDefaultVideoPosterURL = url;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

/**
 * See {@link android.webkit.WebSettings#getDefaultVideoPosterURL}.
 */
String AwSettings::GetDefaultVideoPosterURL()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetDefaultVideoPosterURLLocked();
}

//@CalledByNative
String AwSettings::GetDefaultVideoPosterURLLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mDefaultVideoPosterURL;
}

void AwSettings::OnGestureZoomSupportChanged(
    /* [in] */ Boolean supportsDoubleTapZoom,
    /* [in] */ Boolean supportsMultiTouchZoom)
{
    // Always post asynchronously here, to avoid doubling back onto the caller.
    AutoPtr<IRunnable> runnable = new OnGestureZoomSupportChangedRunnable(this);
    mEventHandler->MaybePostOnUiThread(runnable);
}

/**
 * See {@link android.webkit.WebSettings#setSupportZoom}.
 */
void AwSettings::SetSupportZoom(
    /* [in] */ Boolean support)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mSupportZoom != support) {
        mSupportZoom = support;
        OnGestureZoomSupportChanged(
                SupportsDoubleTapZoomLocked(), SupportsMultiTouchZoomLocked());
    }
}

/**
 * See {@link android.webkit.WebSettings#supportZoom}.
 */
Boolean AwSettings::SupportZoom()
{
    Object::Autolock lock(mAwSettingsLock);
    return mSupportZoom;
}

/**
 * See {@link android.webkit.WebSettings#setBuiltInZoomControls}.
 */
void AwSettings::SetBuiltInZoomControls(
    /* [in] */ Boolean enabled)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mBuiltInZoomControls != enabled) {
        mBuiltInZoomControls = enabled;
        OnGestureZoomSupportChanged(
                SupportsDoubleTapZoomLocked(), SupportsMultiTouchZoomLocked());
    }
}

/**
 * See {@link android.webkit.WebSettings#getBuiltInZoomControls}.
 */
Boolean AwSettings::GetBuiltInZoomControls()
{
    Object::Autolock lock(mAwSettingsLock);
    return mBuiltInZoomControls;
}

/**
 * See {@link android.webkit.WebSettings#setDisplayZoomControls}.
 */
void AwSettings::SetDisplayZoomControls(
    /* [in] */ Boolean enabled)
{
    Object::Autolock lock(mAwSettingsLock);
    mDisplayZoomControls = enabled;
}

/**
 * See {@link android.webkit.WebSettings#getDisplayZoomControls}.
 */
Boolean AwSettings::GetDisplayZoomControls()
{
    Object::Autolock lock(mAwSettingsLock);
    return mDisplayZoomControls;
}

void AwSettings::SetMixedContentMode(
    /* [in] */ Int32 mode)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mMixedContentMode != mode) {
        mMixedContentMode = mode;
        mEventHandler->UpdateWebkitPreferencesLocked();
    }
}

Int32 AwSettings::GetMixedContentMode()
{
    Object::Autolock lock(mAwSettingsLock);
    return mMixedContentMode;
}

//@CalledByNative
Boolean AwSettings::GetAllowRunningInsecureContentLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mMixedContentMode == MIXED_CONTENT_ALWAYS_ALLOW;
}

//@CalledByNative
Boolean AwSettings::GetAllowDisplayingInsecureContentLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mMixedContentMode == MIXED_CONTENT_ALWAYS_ALLOW ||
            mMixedContentMode == MIXED_CONTENT_COMPATIBILITY_MODE;
}

/**
 * Sets whether to use the video overlay for the embedded video.
 * @param flag whether to enable the video overlay for the embedded video.
 */
void AwSettings::SetVideoOverlayForEmbeddedVideoEnabled(
    /* [in] */ Boolean enabled)
{
    Object::Autolock lock(mAwSettingsLock);
    if (mVideoOverlayForEmbeddedVideoEnabled != enabled) {
        mVideoOverlayForEmbeddedVideoEnabled = enabled;
        AutoPtr<IRunnable> runnable = new SetVideoOverlayForEmbeddedVideoEnabledRunnable(this);
        mEventHandler->RunOnUiThreadBlockingAndLocked(runnable);
    }
}

/**
 * Gets whether to use the video overlay for the embedded video.
 * @return true if the WebView enables the video overlay for the embedded video.
 */
Boolean AwSettings::GetVideoOverlayForEmbeddedVideoEnabled()
{
    Object::Autolock lock(mAwSettingsLock);
    return GetVideoOverlayForEmbeddedVideoEnabledLocked();
}

//@CalledByNative
Boolean AwSettings::GetVideoOverlayForEmbeddedVideoEnabledLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mVideoOverlayForEmbeddedVideoEnabled;
}

//@CalledByNative
Boolean AwSettings::SupportsDoubleTapZoomLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mSupportZoom && mBuiltInZoomControls && mUseWideViewport;
}

Boolean AwSettings::SupportsMultiTouchZoomLocked()
{
    assert(Thread::HoldsLock(mAwSettingsLock));
    return mSupportZoom && mBuiltInZoomControls;
}

Boolean AwSettings::SupportsMultiTouchZoom() {
    Object::Autolock lock(mAwSettingsLock);
    return SupportsMultiTouchZoomLocked();
}

Boolean AwSettings::ShouldDisplayZoomControls()
{
    Object::Autolock lock(mAwSettingsLock);
    return SupportsMultiTouchZoomLocked() && mDisplayZoomControls;
}

Int32 AwSettings::ClipFontSize(
    /* [in] */ Int32 size)
{
    if (size < MINIMUM_FONT_SIZE) {
        return MINIMUM_FONT_SIZE;
    }
    else if (size > MAXIMUM_FONT_SIZE) {
        return MAXIMUM_FONT_SIZE;
    }
    return size;
}

//@CalledByNative
void AwSettings::UpdateEverything()
{
    Object::Autolock lock(mAwSettingsLock);
    UpdateEverythingLocked();
}

//@CalledByNative
void AwSettings::PopulateWebPreferences(
    /* [in] */ Int64 webPrefsPtr)
{
    Object::Autolock lock(mAwSettingsLock);
    assert(mNativeAwSettings != 0);
    NativePopulateWebPreferencesLocked(mNativeAwSettings, webPrefsPtr);
}

void AwSettings::UpdateWebkitPreferencesOnUiThreadLocked()
{
    assert(mEventHandler->mHandler != NULL);
    ThreadUtils::AssertOnUiThread();
    if (mNativeAwSettings != 0) {
        NativeUpdateWebkitPreferencesLocked(mNativeAwSettings);
    }
}

Int64 AwSettings::NativeInit(
    /* [in] */ Int64 webContentsPtr)
{
}

void AwSettings::NativeDestroy(
    /* [in] */ Int64 nativeAwSettings)
{
}

void AwSettings::NativePopulateWebPreferencesLocked(
    /* [in] */ Int64 nativeAwSettings,
    /* [in] */ Int64 webPrefsPtr)
{
}

void AwSettings::NativeResetScrollAndScaleState(
    /* [in] */ Int64 nativeAwSettings)
{
}

void AwSettings::NativeUpdateEverythingLocked(
    /* [in] */ Int64 nativeAwSettings)
{
}

void AwSettings::NativeUpdateInitialPageScaleLocked(
    /* [in] */ Int64 nativeAwSettings)
{
}

void AwSettings::NativeUpdateUserAgentLocked(
    /* [in] */ Int64 nativeAwSettings)
{
}

void AwSettings::NativeUpdateWebkitPreferencesLocked(
    /* [in] */ Int64 nativeAwSettings)
{
}

String AwSettings::NativeGetDefaultUserAgent()
{
}

void AwSettings::NativeUpdateFormDataPreferencesLocked(
    /* [in] */ Int64 nativeAwSettings)
{
}

void AwSettings::NativeUpdateRendererPreferencesLocked(
    /* [in] */ Int64 nativeAwSettings)
{
}
