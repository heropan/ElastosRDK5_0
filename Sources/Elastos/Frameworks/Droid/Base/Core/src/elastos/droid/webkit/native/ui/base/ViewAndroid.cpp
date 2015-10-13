
#include "elastos/droid/webkit/native/ui/base/ViewAndroid.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                             ViewAndroid
//=====================================================================
ViewAndroid::ViewAndroid(
    /* [in] */ WindowAndroid* nativeWindow,
    /* [in] */ ViewAndroidDelegate* viewAndroidDelegate)
    : mNativeViewAndroid(0)
    , mViewAndroidDelegate(viewAndroidDelegate)
    , mWindowAndroid(nativeWindow)
    , mKeepScreenOnCount(0)
    , mKeepScreenOnView(NULL)
{
    // ==================before translated======================
    // mWindowAndroid = nativeWindow;
    // mViewAndroidDelegate = viewAndroidDelegate;
    // mNativeViewAndroid = nativeInit(mWindowAndroid.getNativePointer());

    mNativeViewAndroid = NativeInit(mWindowAndroid->GetNativePointer());
}

AutoPtr<ViewAndroidDelegate> ViewAndroid::GetViewAndroidDelegate()
{
    // ==================before translated======================
    // return mViewAndroidDelegate;

    return mViewAndroidDelegate;
}

ECode ViewAndroid::Destroy()
{
    // ==================before translated======================
    // if (mNativeViewAndroid != 0) {
    //     nativeDestroy(mNativeViewAndroid);
    //     mNativeViewAndroid = 0;
    // }

    if (mNativeViewAndroid != 0) {
        NativeDestroy(mNativeViewAndroid);
        mNativeViewAndroid = 0;
    }
    return NOERROR;
}

Int64 ViewAndroid::GetNativePointer()
{
    // ==================before translated======================
    // return mNativeViewAndroid;

    return mNativeViewAndroid;
}

ECode ViewAndroid::IncrementKeepScreenOnCount()
{
    // ==================before translated======================
    // mKeepScreenOnCount++;
    // if (mKeepScreenOnCount == 1) {
    //     mKeepScreenOnView = mViewAndroidDelegate.acquireAnchorView();
    //     mViewAndroidDelegate.setAnchorViewPosition(mKeepScreenOnView, 0, 0, 0, 0);
    //     mKeepScreenOnView.setKeepScreenOn(true);
    // }

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
    // ==================before translated======================
    // assert mKeepScreenOnCount > 0;
    // mKeepScreenOnCount--;
    // if (mKeepScreenOnCount == 0) {
    //     mViewAndroidDelegate.releaseAnchorView(mKeepScreenOnView);
    //     mKeepScreenOnView = null;
    // }

    assert (mKeepScreenOnCount > 0);
    --mKeepScreenOnCount;
    if (mKeepScreenOnCount == 0) {
        mViewAndroidDelegate->ReleaseAnchorView(mKeepScreenOnView);
        mKeepScreenOnView = NULL;
    }
    return NOERROR;
}

Int64 ViewAndroid::NativeInit(
    /* [in] */ Int64 windowPtr)
{
    assert(0);
    return 0;
}

ECode ViewAndroid::NativeDestroy(
    /* [in] */ Int64 nativeViewAndroid)
{
    assert(0);
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


