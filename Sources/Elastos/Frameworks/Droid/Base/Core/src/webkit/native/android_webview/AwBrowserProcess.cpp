
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//               AwBrowserProcess::InnerRunnable
//===============================================================

AwBrowserProcess::InnerRunnable::InnerRunnable(
    /* [in] */ AwBrowserProcess* owner,
    /* [in] */ const IContext* context)
    : mOwner(owner)
    , mContext(context)
{
}

ECode AwBrowserProcess::InnerRunnable::Run()
{
    //try {
        BrowserStartupController::Get(mContext)->StartBrowserProcessesSync(
                    BrowserStartupController::MAX_RENDERERS_SINGLE_PROCESS);
        InitializePlatformKeySystem();
    //} catch (ProcessInitException e) {
    //    throw new RuntimeException("Cannot initialize WebView", e);
    //}
}

//===============================================================
//                      AwBrowserProcess
//===============================================================

const String AwBrowserProcess::PRIVATE_DATA_DIRECTORY_SUFFIX("webview");
const String AwBrowserProcess::TAG("AwBrowserProcess");

/**
 * Loads the native library, and performs basic static construction of objects needed
 * to run webview in this process. Does not create threads; safe to call from zygote.
 * Note: it is up to the caller to ensure this is only called once.
 */
void AwBrowserProcess::LoadLibrary()
{
    PathUtils.setPrivateDataDirectorySuffix(PRIVATE_DATA_DIRECTORY_SUFFIX);
    try {
        LibraryLoader.loadNow();
    } catch (ProcessInitException e) {
        throw new RuntimeException("Cannot load WebView", e);
    }
}

/**
 * Starts the chromium browser process running within this process. Creates threads
 * and performs other per-app resource allocations; must not be called from zygote.
 * Note: it is up to the caller to ensure this is only called once.
 * @param context The Android application context
 */
void AwBrowserProcess::Start(
    /* [in] */ const IContext* context)
{
    // We must post to the UI thread to cover the case that the user
    // has invoked Chromium startup by using the (thread-safe)
    // CookieManager rather than creating a WebView.
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, context);
    ThreadUtils::RunOnUiThreadBlocking(runnable);
}

void AwBrowserProcess::InitializePlatformKeySystem()
{
    AutoPtr< ArrayOf<String> > mappings = AwResource::GetConfigKeySystemUuidMapping();
    Int32 length = mapping->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        //try {
            String fragments[] = mapping.split(",");
            String keySystem = fragments[0].trim();
            UUID uuid = UUID.fromString(fragments[1]);
            MediaDrmBridge.addKeySystemUuidMapping(keySystem, uuid);
        //} catch (java.lang.RuntimeException e) {
        //    Log.e(TAG, "Can't parse key-system mapping: " + mapping);
        //}
    }
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos