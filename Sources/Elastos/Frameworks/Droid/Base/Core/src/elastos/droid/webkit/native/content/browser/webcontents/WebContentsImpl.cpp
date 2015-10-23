
#include "elastos/droid/webkit/native/content/browser/webcontents/WebContentsImpl.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Webcontents {

WebContentsImpl::WebContentsImpl(
    /* [in] */ Int64 nativeWebContentsAndroid,
    /* [in] */ NavigationController* navigationController)
    : mNativeWebContentsAndroid(nativeWebContentsAndroid)
    , mNavigationController(navigationController)
{
}

//@CalledByNative
AutoPtr<WebContentsImpl> WebContentsImpl::Create(
    /* [in] */ Int64 nativeWebContentsAndroid,
    /* [in] */ NavigationController* navigationController)
{
    return new WebContentsImpl(nativeWebContentsAndroid, navigationController);
}

//@CalledByNative
void WebContentsImpl::Destroy()
{
    mNativeWebContentsAndroid = 0;
    mNavigationController = NULL;
}

//@CalledByNative
Int64 WebContentsImpl::GetNativePointer()
{
    return mNativeWebContentsAndroid;
}

//@Override
AutoPtr<NavigationController> WebContentsImpl::GetNavigationController()
{
    return mNavigationController;
}

//@Override
String WebContentsImpl::GetTitle()
{
    return NativeGetTitle(mNativeWebContentsAndroid);
}

//@Override
String WebContentsImpl::GetVisibleUrl()
{
    return NativeGetVisibleURL(mNativeWebContentsAndroid);
}

//@Override
void WebContentsImpl::Stop()
{
    NativeStop(mNativeWebContentsAndroid);
}

String WebContentsImpl::NativeGetTitle(
    /* [in] */ Int64 nativeWebContentsAndroid)
{
    assert(0);
    return String(NULL);
}

String WebContentsImpl::NativeGetVisibleURL(
    /* [in] */ Int64 nativeWebContentsAndroid)
{
    assert(0);
    return String(NULL);
}

void WebContentsImpl::NativeStop(
    /* [in] */ Int64 nativeWebContentsAndroid)
{
}

} // namespace Webcontents
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
