
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Framehost {

NavigationControllerImpl::NavigationControllerImpl(
    /* [in] */ Int64 nativeNavigationControllerAndroid)
{
    mNativeNavigationControllerAndroid = nativeNavigationControllerAndroid;
}

//@CalledByNative
AutoPtr<NavigationControllerImpl> NavigationControllerImpl::Create(
    /* [in] */ Int64 nativeNavigationControllerAndroid)
{
    AutoPtr<NavigationControllerImpl> ret = new NavigationControllerImpl(nativeNavigationControllerAndroid);
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
    /* [in] */ Int64 nativeNavigationControllerAndroid)
{
}

Boolean NavigationControllerImpl::NativeCanGoForward(
    /* [in] */ Int64 nativeNavigationControllerAndroid)
{
}

Boolean NavigationControllerImpl::NativeCanGoToOffset(
    /* [in] */ Int64 nativeNavigationControllerAndroid,
    /* [in] */ Int32 offset)
{
}

void NavigationControllerImpl::NativeGoBack(
    /* [in] */ Int64 nativeNavigationControllerAndroid)
{
}

void NavigationControllerImpl::NativeGoForward(
    /* [in] */ Int64 nativeNavigationControllerAndroid)
{
}

void NavigationControllerImpl::NativeGoToOffset(
    /* [in] */ Int64 nativeNavigationControllerAndroid,
    /* [in] */ Int32 offset)
{
}

void NavigationControllerImpl::NativeGoToNavigationIndex(
    /* [in] */ Int64 nativeNavigationControllerAndroid,
    /* [in] */ Int32 index)
{
}

} // namespace Framehost
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
