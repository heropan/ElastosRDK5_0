// wuweizuo automatic build .cpp file from .java file.

#include "elastos/droid/webkit/native/content/browser/InterstitialPageDelegateAndroid.h"
#include "elastos/droid/webkit/native/content/api/InterstitialPageDelegateAndroid_dec.h"
#include <elastos/utility/logging/Logger.h>
using Elastos::Utility::Logging::Logger;

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
    return Elastos_InterstitialPageDelegateAndroid_nativeInit(THIS_PROBE(IInterface), htmlContent);
}

ECode InterstitialPageDelegateAndroid::NativeProceed(
    /* [in] */ Int64 nativeInterstitialPageDelegateAndroid)
{
    Elastos_InterstitialPageDelegateAndroid_nativeProceed(THIS_PROBE(IInterface),
            (Handle32)nativeInterstitialPageDelegateAndroid);
    return NOERROR;
}

ECode InterstitialPageDelegateAndroid::NativeDontProceed(
    /* [in] */ Int64 nativeInterstitialPageDelegateAndroid)
{
    Elastos_InterstitialPageDelegateAndroid_nativeDontProceed(THIS_PROBE(IInterface),
            (Handle32)nativeInterstitialPageDelegateAndroid);
    return NOERROR;
}

void InterstitialPageDelegateAndroid::OnProceed(
    /* [in] */ IInterface* obj)
{
    AutoPtr<InterstitialPageDelegateAndroid> mObj = (InterstitialPageDelegateAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("InterstitialPageDelegateAndroid", "InterstitialPageDelegateAndroid::OnProceed, mObj is NULL");
        return;
    }
    mObj->OnProceed();
}

void InterstitialPageDelegateAndroid::OnDontProceed(
    /* [in] */ IInterface* obj)
{
    AutoPtr<InterstitialPageDelegateAndroid> mObj = (InterstitialPageDelegateAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("InterstitialPageDelegateAndroid", "InterstitialPageDelegateAndroid::OnDontProceed, mObj is NULL");
        return;
    }
    mObj->OnDontProceed();
}

void InterstitialPageDelegateAndroid::CommandReceived(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& command)
{
    AutoPtr<InterstitialPageDelegateAndroid> mObj = (InterstitialPageDelegateAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("InterstitialPageDelegateAndroid", "InterstitialPageDelegateAndroid::CommandReceived, mObj is NULL");
        return;
    }
    mObj->CommandReceived(command);
}

void InterstitialPageDelegateAndroid::OnNativeDestroyed(
    /* [in] */ IInterface* obj)
{
    AutoPtr<InterstitialPageDelegateAndroid> mObj = (InterstitialPageDelegateAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("InterstitialPageDelegateAndroid", "InterstitialPageDelegateAndroid::OnNativeDestroyed, mObj is NULL");
        return;
    }
    mObj->OnNativeDestroyed();
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
