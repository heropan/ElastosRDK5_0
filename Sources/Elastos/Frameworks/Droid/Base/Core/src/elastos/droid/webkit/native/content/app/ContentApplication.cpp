
#include "webkit/native/content/app/ContentApplication.h"

// import android.os.Looper;
// import android.os.MessageQueue;

// import org.chromium.base.BaseChromiumApplication;
// import org.chromium.base.library_loader.LibraryLoader;
// import org.chromium.content.browser.TracingControllerAndroid;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace App {

AutoPtr<TracingControllerAndroid> ContentApplication::GetTracingController()
{
    if (mTracingController == NULL) {
        mTracingController = new TracingControllerAndroid(this);
    }
    return mTracingController;
}

//@Override
ECode ContentApplication::OnCreate()
{
    assert(0);
#if 0
    super.onCreate();

    // Delay TracingControllerAndroid.registerReceiver() until the main loop is idle.
    Looper.myQueue().addIdleHandler(new MessageQueue.IdleHandler() {
        @Override
        public boolean queueIdle() {
            // Will retry if the native library has not been initialized.
            if (!LibraryLoader.isInitialized()) return true;

            try {
                getTracingController().registerReceiver(ContentApplication.this);
            } catch (SecurityException e) {
                // Happens if the process is isolated. Ignore.
            }
            // Remove the idle handler.
            return false;
        }
    });
#endif

    return E_NOT_IMPLEMENTED;
}

/**
 * For emulated process environment only. On a production device, the application process is
 * simply killed without calling this method. We don't need to unregister the broadcast
 * receiver in the latter case.
 */
//@Override
ECode ContentApplication::OnTerminate()
{
    assert(0);
#if 0
    try {
        getTracingController().unregisterReceiver(this);
    } catch (SecurityException e) {
        // Happens if the process is isolated. Ignore.
    }

    super.onTerminate();
#endif

    return E_NOT_IMPLEMENTED;
}

} // namespace App
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
