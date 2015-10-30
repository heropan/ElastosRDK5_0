#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWHTTPAUTHHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWHTTPAUTHHANDLER_H__
#include "elastos/droid/ext/frameworkext.h"

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//@JNINamespace("android_webview")
class AwHttpAuthHandler
    : public Object
{
public:
    CARAPI_(void) Proceed(
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI_(void) Cancel();

    CARAPI_(Boolean) IsFirstAttempt();

    //@CalledByNative return AwHttpAuthHandler
    static CARAPI_(AutoPtr<IInterface>) Create(
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

//callback function declaration
public:
    static CARAPI_(void*) ElaAwHttpAuthHandlerCallback_Init();

private:
    static CARAPI_(void) HandlerDestroyed(
        /* [in] */ IInterface* obj);

private:
    Int64 mNativeAwHttpAuthHandler;
    const Boolean mFirstAttempt;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWHTTPAUTHHANDLER_H__
