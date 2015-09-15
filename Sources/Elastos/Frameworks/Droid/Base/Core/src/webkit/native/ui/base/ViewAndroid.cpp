// wuweizuo automatic build .cpp file from .java file.

#include "ViewAndroid.h"

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
{
    // ==================before translated======================
    // mWindowAndroid = nativeWindow;
    // mViewAndroidDelegate = viewAndroidDelegate;
    // mNativeViewAndroid = nativeInit(mWindowAndroid.getNativePointer());
}

AutoPtr<ViewAndroidDelegate> ViewAndroid::GetViewAndroidDelegate()
{
    // ==================before translated======================
    // return mViewAndroidDelegate;
    assert(0);
    AutoPtr<ViewAndroidDelegate> empty;
    return empty;
}

ECode ViewAndroid::Destroy()
{
    // ==================before translated======================
    // if (mNativeViewAndroid != 0) {
    //     nativeDestroy(mNativeViewAndroid);
    //     mNativeViewAndroid = 0;
    // }
    assert(0);
    return NOERROR;
}

Int64 ViewAndroid::GetNativePointer()
{
    // ==================before translated======================
    // return mNativeViewAndroid;
    assert(0);
    return 0;
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
    assert(0);
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
    assert(0);
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


