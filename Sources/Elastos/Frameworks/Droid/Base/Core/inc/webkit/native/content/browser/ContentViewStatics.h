
#ifndef __CONTENTVIEWSTATICS_H__
#define __CONTENTVIEWSTATICS_H__

// import org.chromium.net.ProxyChangeListener;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Implementations of various static methods.
 */
class ContentViewStatics
{
public:
    /**
     * Return the first substring consisting of the address of a physical location.
     * @see {@link android.webkit.WebView#findAddress(String)}
     *
     * @param addr The string to search for addresses.
     * @return the address, or if no address is found, return null.
     */
    static CARAPI_(String) FindAddress(
        /* [in] */ String addr);

    /**
     * Suspends Webkit timers in all renderers.
     * New renderers created after this call will be created with the
     * default options.
     *
     * @param suspend true if timers should be suspended.
     */
    static CARAPI_(void) SetWebKitSharedTimersSuspended(
        /* [in] */ Boolean suspend);

    /**
     * Enables platform notifications of data state and proxy changes.
     * Notifications are enabled by default.
     */
    static CARAPI_(void) EnablePlatformNotifications();

    /**
     * Disables platform notifications of data state and proxy changes.
     * Notifications are enabled by default.
     */
    static CARAPI_(void) DisablePlatformNotifications ();

private:
    // Native functions

    static CARAPI_(String) NativeFindAddress(
        /* [in] */ String addr);

    static CARAPI_(void) NativeSetWebKitSharedTimersSuspended(
        /* [in] */ Boolean suspend);
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__CONTENTVIEWSTATICS_H__
