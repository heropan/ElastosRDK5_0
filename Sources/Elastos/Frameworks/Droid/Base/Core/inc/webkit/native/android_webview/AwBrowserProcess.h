
#ifndef __AWBROWSERPROCESS_H__
#define __AWBROWSERPROCESS_H__

// import android.content.Context;
// import android.util.Log;

// import org.chromium.base.PathUtils;
// import org.chromium.base.ThreadUtils;
// import org.chromium.base.library_loader.LibraryLoader;
// import org.chromium.base.library_loader.ProcessInitException;
// import org.chromium.content.browser.BrowserStartupController;
// import org.chromium.media.MediaDrmBridge;

// import java.util.UUID;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Wrapper for the steps needed to initialize the java and native sides of webview chromium.
 */
class AwBrowserProcess
{
private:
    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRunnable(
            /* [in] */ AwBrowserProcess* owner,
            /* [in] */ const IContext* context);

        CARAPI Run();

    private:
        AwBrowserProcess* mOwner;
        const IContext* mContext
    };

public:
    /**
     * Loads the native library, and performs basic static construction of objects needed
     * to run webview in this process. Does not create threads; safe to call from zygote.
     * Note: it is up to the caller to ensure this is only called once.
     */
    static CARAPI_(void) LoadLibrary();

    /**
     * Starts the chromium browser process running within this process. Creates threads
     * and performs other per-app resource allocations; must not be called from zygote.
     * Note: it is up to the caller to ensure this is only called once.
     * @param context The Android application context
     */
    static CARAPI_(void) Start(
        /* [in] */ const IContext* context);

private:
    static const String PRIVATE_DATA_DIRECTORY_SUFFIX;
    static const String TAG;

    static CARAPI_(void) InitializePlatformKeySystem();
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWBROWSERPROCESS_H__
