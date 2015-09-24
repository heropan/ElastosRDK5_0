
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWHTTPAUTHHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWHTTPAUTHHANDLER_H__

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//@JNINamespace("android_webview")
class AwHttpAuthHandler
{
public:
    CARAPI_(void) Proceed(
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI_(void) Cancel();

    CARAPI_(Boolean) IsFirstAttempt();

    //@CalledByNative
    static CARAPI_(AutoPtr<AwHttpAuthHandler>) Create(
        /* [in] */ Int64 nativeAwAuthHandler,
        /* [in] */ Boolean firstAttempt);

    //@CalledByNative
    CARAPI_(void) HandlerDestroyed();

private:
    AwHttpAuthHandler(
        /* [in] */ Int64 nativeAwHttpAuthHandler,
        /* [in] */ Boolean firstAttempt);

    CARAPI_(void) NativeProceed(
        /* [in] */ Int64 nativeAwHttpAuthHandler,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI_(void) NativeCancel(
        /* [in] */ Int64 nativeAwHttpAuthHandler);

private:
    Int64 mNativeAwHttpAuthHandler;
    const Boolean mFirstAttempt;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWHTTPAUTHHANDLER_H__
