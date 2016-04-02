
#include "elastos/droid/webkit/webview/chromium/native/content/browser/framehost/NavigationControllerImpl.h"
#include "elastos/droid/webkit/webview/chromium/native/content/api/NavigationControllerImpl_dec.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
namespace Content {
namespace Browser {
namespace Framehost {

NavigationControllerImpl::NavigationControllerImpl(
    /* [in] */ Handle64 nativeNavigationControllerAndroid)
{
    mNativeNavigationControllerAndroid = nativeNavigationControllerAndroid;
}

//@CalledByNative
AutoPtr<IInterface> NavigationControllerImpl::Create(
    /* [in] */ Handle64 nativeNavigationControllerAndroid)
{
    AutoPtr<NavigationControllerImpl> ret = new NavigationControllerImpl(nativeNavigationControllerAndroid);
    return TO_IINTERFACE(ret);
}

//@CalledByNative
void NavigationControllerImpl::Destroy()
{
    mNativeNavigationControllerAndroid = 0;
}

//@Override
Boolean NavigationControllerImpl::CanGoBack()
{
    return mNativeNavigationControllerAndroid != 0
            && NativeCanGoBack(mNativeNavigationControllerAndroid);
}

//@Override
Boolean NavigationControllerImpl::CanGoForward()
{
    return mNativeNavigationControllerAndroid != 0
            && NativeCanGoForward(mNativeNavigationControllerAndroid);
}

//@Override
Boolean NavigationControllerImpl::CanGoToOffset(
    /* [in] */ Int32 offset)
{
    return mNativeNavigationControllerAndroid != 0
            && NativeCanGoToOffset(mNativeNavigationControllerAndroid, offset);
}

//@Override
void NavigationControllerImpl::GoToOffset(
    /* [in] */ Int32 offset)
{
    if (mNativeNavigationControllerAndroid != 0) {
        NativeGoToOffset(mNativeNavigationControllerAndroid, offset);
    }
}

//@Override
void NavigationControllerImpl::GoToNavigationIndex(
    /* [in] */ Int32 index)
{
    if (mNativeNavigationControllerAndroid != 0) {
        NativeGoToNavigationIndex(mNativeNavigationControllerAndroid, index);
    }
}

//@Override
void NavigationControllerImpl::GoBack()
{
    if (mNativeNavigationControllerAndroid != 0) {
        NativeGoBack(mNativeNavigationControllerAndroid);
    }
}

//@Override
void NavigationControllerImpl::GoForward()
{
    if (mNativeNavigationControllerAndroid != 0) {
        NativeGoForward(mNativeNavigationControllerAndroid);
    }
}

Boolean NavigationControllerImpl::NativeCanGoBack(
    /* [in] */ Handle64 nativeNavigationControllerAndroid)
{
    return Elastos_NavigationControllerImpl_nativeCanGoBack(TO_IINTERFACE(this), nativeNavigationControllerAndroid);
}

Boolean NavigationControllerImpl::NativeCanGoForward(
    /* [in] */ Handle64 nativeNavigationControllerAndroid)
{
    return Elastos_NavigationControllerImpl_nativeCanGoForward(TO_IINTERFACE(this), nativeNavigationControllerAndroid);
}

Boolean NavigationControllerImpl::NativeCanGoToOffset(
    /* [in] */ Handle64 nativeNavigationControllerAndroid,
    /* [in] */ Int32 offset)
{
    return Elastos_NavigationControllerImpl_nativeCanGoToOffset(TO_IINTERFACE(this), nativeNavigationControllerAndroid, offset);
}

void NavigationControllerImpl::NativeGoBack(
    /* [in] */ Handle64 nativeNavigationControllerAndroid)
{
    Elastos_NavigationControllerImpl_nativeGoBack(TO_IINTERFACE(this), nativeNavigationControllerAndroid);
}

void NavigationControllerImpl::NativeGoForward(
    /* [in] */ Handle64 nativeNavigationControllerAndroid)
{
    Elastos_NavigationControllerImpl_nativeGoForward(TO_IINTERFACE(this), nativeNavigationControllerAndroid);
}

void NavigationControllerImpl::NativeGoToOffset(
    /* [in] */ Handle64 nativeNavigationControllerAndroid,
    /* [in] */ Int32 offset)
{
    Elastos_NavigationControllerImpl_nativeGoToOffset(TO_IINTERFACE(this), nativeNavigationControllerAndroid, offset);
}

void NavigationControllerImpl::NativeGoToNavigationIndex(
    /* [in] */ Handle64 nativeNavigationControllerAndroid,
    /* [in] */ Int32 index)
{
    Elastos_NavigationControllerImpl_nativeGoToNavigationIndex(TO_IINTERFACE(this), nativeNavigationControllerAndroid, index);
}

void NavigationControllerImpl::Destroy(
    /* [in] */ IInterface* obj)
{
    AutoPtr<NavigationControllerImpl> mObj = (NavigationControllerImpl*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("NavigationControllerImpl", "NavigationControllerImpl::Destroy, mObj is NULL");
        return;
    }
    mObj->Destroy();
}

} // namespace Framehost
} // namespace Browser
} // namespace Content
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
