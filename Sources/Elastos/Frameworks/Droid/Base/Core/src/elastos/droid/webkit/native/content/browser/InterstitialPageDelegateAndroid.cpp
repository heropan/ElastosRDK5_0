// wuweizuo automatic build .cpp file from .java file.

#include "InterstitialPageDelegateAndroid.h"

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
    // ==================before translated======================
    // mNativePtr = nativeInit(htmlContent);
}

Int64 InterstitialPageDelegateAndroid::GetNative()
{
    // ==================before translated======================
    // return mNativePtr;
    assert(0);
    return 0;
}

ECode InterstitialPageDelegateAndroid::OnProceed()
{
    assert(0);
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::OnDontProceed()
{
    assert(0);
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::CommandReceived(
    /* [in] */ const String& command)
{
    assert(0);
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::Proceed()
{
    // ==================before translated======================
    // if (mNativePtr != 0) nativeProceed(mNativePtr);
    assert(0);
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::DontProceed()
{
    // ==================before translated======================
    // if (mNativePtr != 0) nativeDontProceed(mNativePtr);
    assert(0);
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::OnNativeDestroyed()
{
    // ==================before translated======================
    // mNativePtr = 0;
    assert(0);
    return NOERROR;
}

Int64 InterstitialPageDelegateAndroid::NativeInit(
    /* [in] */ const String& htmlContent)
{
    assert(0);
    return 0;
}

ECode InterstitialPageDelegateAndroid::NativeProceed(
    /* [in] */ Int64 nativeInterstitialPageDelegateAndroid)
{
    assert(0);
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::NativeDontProceed(
    /* [in] */ Int64 nativeInterstitialPageDelegateAndroid)
{
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


