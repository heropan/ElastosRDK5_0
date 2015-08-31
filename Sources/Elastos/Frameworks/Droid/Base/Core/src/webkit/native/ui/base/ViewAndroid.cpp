// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

ViewAndroid::ViewAndroid(
    /* in */ WindowAndroid* nativeWindow,
    /* in */ ViewAndroidDelegate* viewAndroidDelegate)
    : mWindowAndroid(nativeWindow)
    , mViewAndroidDelegate(viewAndroidDelegate)
    , mNativeViewAndroid(0)
    , mKeepScreenOnCount(0)
{
    mNativeViewAndroid = NativeInit(mWindowAndroid->GetNativePointer());
}

AutoPtr<ViewAndroidDelegate> ViewAndroid::GetViewAndroidDelegate()
{
    return mViewAndroidDelegate;
}

ECode ViewAndroid::Destroy()
{
    if (mNativeViewAndroid != 0) {
        NativeDestroy(mNativeViewAndroid);
        mNativeViewAndroid = 0;
    }
    return NOERROR;
}

Int64 ViewAndroid::GetNativePointer()
{
    return mNativeViewAndroid;
}

ECode ViewAndroid::IncrementKeepScreenOnCount()
{
    mKeepScreenOnCount++;
    if (mKeepScreenOnCount == 1) {
        mKeepScreenOnView = mViewAndroidDelegate->AcquireAnchorView();
        mViewAndroidDelegate->SetAnchorViewPosition(mKeepScreenOnView, 0, 0, 0, 0);
        mKeepScreenOnView->SetKeepScreenOn(TRUE);
    }
    return NOERROR;
}

ECode ViewAndroid::DecrementKeepScreenOnCount()
{
    assert (mKeepScreenOnCount > 0);
    mKeepScreenOnCount--;
    if (mKeepScreenOnCount == 0) {
        mViewAndroidDelegate->ReleaseAnchorView(mKeepScreenOnView);
        mKeepScreenOnView = NULL;
    }
    return NOERROR;
}

Int64 ViewAndroid::NativeInit(
    /* in */ Int64 windowPtr)
{
}

ECode ViewAndroid::NativeDestroy(
    /* in */ Int64 nativeViewAndroid)
{
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

