// wuweizuo automatic build .cpp file from .java file.

#include "elastos/droid/webkit/native/content/browser/InterstitialPageDelegateAndroid.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                   InterstitialPageDelegateAndroid
//=====================================================================
InterstitialPageDelegateAndroid::InterstitialPageDelegateAndroid(
    /* [in] */ const String& htmlContent)
{
    mNativePtr = NativeInit(htmlContent);
}

Int64 InterstitialPageDelegateAndroid::GetNative()
{
    return mNativePtr;
}

ECode InterstitialPageDelegateAndroid::OnProceed()
{
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::OnDontProceed()
{
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::CommandReceived(
    /* [in] */ const String& command)
{
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::Proceed()
{
    if (mNativePtr != 0) {
        NativeProceed(mNativePtr);
    }

    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::DontProceed()
{
    if (mNativePtr != 0) {
        NativeDontProceed(mNativePtr);
    }

    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::OnNativeDestroyed()
{
    mNativePtr = 0;
    return NOERROR;
}

Int64 InterstitialPageDelegateAndroid::NativeInit(
    /* [in] */ const String& htmlContent)
{
    return 0;
}

ECode InterstitialPageDelegateAndroid::NativeProceed(
    /* [in] */ Int64 nativeInterstitialPageDelegateAndroid)
{
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::NativeDontProceed(
    /* [in] */ Int64 nativeInterstitialPageDelegateAndroid)
{
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
