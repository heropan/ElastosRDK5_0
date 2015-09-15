// wuweizuo automatic build .cpp file from .java file.

#include "ScreenOrientationListener.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//               ScreenOrientationListener::InnerRunnable
//=====================================================================
ScreenOrientationListener::InnerRunnable::InnerRunnable(
    /* [in] */ ScreenOrientationListener* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ScreenOrientationListener::InnerRunnable::Run()
{
    // ==================before translated======================
    // obs.onScreenOrientationChanged(mOrientation);
    assert(0);
    return NOERROR;
}

//=====================================================================
//  ScreenOrientationListener::ScreenOrientationConfigurationListener
//=====================================================================
ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::StartListening()
{
    // ==================before translated======================
    // mAppContext.registerComponentCallbacks(this);
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::StopListening()
{
    // ==================before translated======================
    // mAppContext.unregisterComponentCallbacks(this);
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    VALIDATE_NOT_NULL(newConfig);
    // ==================before translated======================
    // notifyObservers();
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::OnLowMemory()
{
    assert(0);
    return NOERROR;
}

//=====================================================================
//     ScreenOrientationListener::ScreenOrientationDisplayListener
//=====================================================================
ECode ScreenOrientationListener::ScreenOrientationDisplayListener::StartListening()
{
    // ==================before translated======================
    // DisplayManager displayManager =
    //         (DisplayManager) mAppContext.getSystemService(Context.DISPLAY_SERVICE);
    // displayManager.registerDisplayListener(this, null);
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::StopListening()
{
    // ==================before translated======================
    // DisplayManager displayManager =
    //         (DisplayManager) mAppContext.getSystemService(Context.DISPLAY_SERVICE);
    // displayManager.unregisterDisplayListener(this);
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::OnDisplayAdded(
    /* [in] */ Int32 displayId)
{
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::OnDisplayRemoved(
    /* [in] */ Int32 displayId)
{
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::OnDisplayChanged(
    /* [in] */ Int32 displayId)
{
    // ==================before translated======================
    // notifyObservers();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                      ScreenOrientationListener
//=====================================================================
const String ScreenOrientationListener::TAG("ScreenOrientationListener");
AutoPtr<ScreenOrientationListener> ScreenOrientationListener::sInstance;

AutoPtr<ScreenOrientationListener> ScreenOrientationListener::GetInstance()
{
    // ==================before translated======================
    // ThreadUtils.assertOnUiThread();
    //
    // if (sInstance == null) {
    //     sInstance = new ScreenOrientationListener();
    // }
    //
    // return sInstance;
    assert(0);
    AutoPtr<ScreenOrientationListener> empty;
    return empty;
}

ECode ScreenOrientationListener::InjectConfigurationListenerBackendForTest()
{
    // ==================before translated======================
    // mBackend = new ScreenOrientationConfigurationListener();
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::AddObserver(
    /* [in] */ ScreenOrientationObserver* observer,
    /* [in] */ IContext* context)
{
    VALIDATE_NOT_NULL(observer);
    VALIDATE_NOT_NULL(context);
    // ==================before translated======================
    // if (mAppContext == null) {
    //     mAppContext = context.getApplicationContext();
    // }
    //
    // assert mAppContext == context.getApplicationContext();
    // assert mAppContext != null;
    //
    // if (!mObservers.addObserver(observer)) {
    //     Log.w(TAG, "Adding an observer that is already present!");
    //     return;
    // }
    //
    // // If we got our first observer, we should start listening.
    // if (mObservers.size() == 1) {
    //     updateOrientation();
    //     mBackend.startListening();
    // }
    //
    // // We need to send the current value to the added observer as soon as
    // // possible but outside of the current stack.
    // final ScreenOrientationObserver obs = observer;
    // ThreadUtils.assertOnUiThread();
    // ThreadUtils.postOnUiThread(new Runnable() {
    //     @Override
    //     public void run() {
    //         obs.onScreenOrientationChanged(mOrientation);
    //     }
    // });
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::RemoveObserver(
    /* [in] */ ScreenOrientationObserver* observer)
{
    VALIDATE_NOT_NULL(observer);
    // ==================before translated======================
    // if (!mObservers.removeObserver(observer)) {
    //     Log.w(TAG, "Removing an inexistent observer!");
    //     return;
    // }
    //
    // if (mObservers.isEmpty()) {
    //     // The last observer was removed, we should just stop listening.
    //     mBackend.stopListening();
    // }
    assert(0);
    return NOERROR;
}

ScreenOrientationListener::ScreenOrientationListener()
    : new ScreenOrientationConfigurationListener();
     }
{
    // ==================before translated======================
    // mBackend = Build.VERSION.SDK_INT >= 17 ?
    //         new ScreenOrientationDisplayListener() :
    //         new ScreenOrientationConfigurationListener();
}

ECode ScreenOrientationListener::NotifyObservers()
{
    // ==================before translated======================
    // int previousOrientation = mOrientation;
    // updateOrientation();
    //
    // DeviceDisplayInfo.create(mAppContext).updateNativeSharedDisplayInfo();
    //
    // if (mOrientation == previousOrientation) {
    //     return;
    // }
    //
    // for (ScreenOrientationObserver observer : mObservers) {
    //     observer.onScreenOrientationChanged(mOrientation);
    // }
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationListener::UpdateOrientation()
{
    // ==================before translated======================
    // WindowManager windowManager =
    //         (WindowManager) mAppContext.getSystemService(Context.WINDOW_SERVICE);
    //
    // switch (windowManager.getDefaultDisplay().getRotation()) {
    //     case Surface.ROTATION_0:
    //         mOrientation = 0;
    //         break;
    //     case Surface.ROTATION_90:
    //         mOrientation = 90;
    //         break;
    //     case Surface.ROTATION_180:
    //         mOrientation = 180;
    //         break;
    //     case Surface.ROTATION_270:
    //         mOrientation = -90;
    //         break;
    //     default:
    //         throw new IllegalStateException(
    //                 "Display.getRotation() shouldn't return that value");
    // }
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


