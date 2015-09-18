
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_APP_CONTENTAPPLICATION_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_APP_CONTENTAPPLICATION_H__

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

/**
 * Basic application functionality that should be shared among all browser applications
 * based on the content layer.
 */
class ContentApplication : public BaseChromiumApplication
{
public:
    CARAPI_(AutoPtr<TracingControllerAndroid>) GetTracingController();

    //@Override
    CARAPI OnCreate();

    /**
     * For emulated process environment only. On a production device, the application process is
     * simply killed without calling this method. We don't need to unregister the broadcast
     * receiver in the latter case.
     */
    //@Override
    CARAPI OnTerminate();

private:
    AutoPtr<TracingControllerAndroid> mTracingController;
};

} // namespace App
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_APP_CONTENTAPPLICATION_H__
