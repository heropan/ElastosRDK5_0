// wuweizuo automatic build .cpp file from .java file.

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
    /*
    mOwner = owner;
    */
}

ECode ScreenOrientationListener::InnerRunnable::Run()
{
    /*
    obs.onScreenOrientationChanged(mOrientation);
    */
    return NOERROR;
}

//=====================================================================
//  ScreenOrientationListener::ScreenOrientationConfigurationListener
//=====================================================================
ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::StartListening()
{
    /*
    mAppContext.registerComponentCallbacks(this);
    */
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::StopListening()
{
    /*
    mAppContext.unregisterComponentCallbacks(this);
    */
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    VALIDATE_NOT_NULL(newConfig);
    /*
    notifyObservers();
    */
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationConfigurationListener::OnLowMemory()
{
    return NOERROR;
}

//=====================================================================
//     ScreenOrientationListener::ScreenOrientationDisplayListener
//=====================================================================
ECode ScreenOrientationListener::ScreenOrientationDisplayListener::StartListening()
{
    /*
    DisplayManager displayManager =
            (DisplayManager) mAppContext.getSystemService(Context.DISPLAY_SERVICE);
    displayManager.registerDisplayListener(this, null);
    */
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::StopListening()
{
    /*
    DisplayManager displayManager =
            (DisplayManager) mAppContext.getSystemService(Context.DISPLAY_SERVICE);
    displayManager.unregisterDisplayListener(this);
    */
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::OnDisplayAdded(
    /* [in] */ Int32 displayId)
{
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::OnDisplayRemoved(
    /* [in] */ Int32 displayId)
{
    return NOERROR;
}

ECode ScreenOrientationListener::ScreenOrientationDisplayListener::OnDisplayChanged(
    /* [in] */ Int32 displayId)
{
    /*
    notifyObservers();
    */
    return NOERROR;
}

//=====================================================================
//                      ScreenOrientationListener
//=====================================================================
const String ScreenOrientationListener::TAG("ScreenOrientationListener");
AutoPtr<ScreenOrientationListener> ScreenOrientationListener::sInstance;

AutoPtr<ScreenOrientationListener> ScreenOrientationListener::GetInstance()
{
    /*
    ThreadUtils.assertOnUiThread();


    if (sInstance == null) {
        sInstance = new ScreenOrientationListener();
    }


    return sInstance;
    */
    return AutoPtr<ScreenOrientationListener>(this);
}

ECode ScreenOrientationListener::InjectConfigurationListenerBackendForTest()
{
    /*
    mBackend = new ScreenOrientationConfigurationListener();
    */
    return NOERROR;
}

ECode ScreenOrientationListener::AddObserver(
    /* [in] */ ScreenOrientationObserver* observer,
    /* [in] */ IContext* context)
{
    VALIDATE_NOT_NULL(observer);
    VALIDATE_NOT_NULL(context);
    /*
    if (mAppContext == null) {
        mAppContext = context.getApplicationContext();
    }


    assert mAppContext == context.getApplicationContext();
    assert mAppContext != null;


    if (!mObservers.addObserver(observer)) {
        Log.w(TAG, "Adding an observer that is already present!");
        return;
    }


    // If we got our first observer, we should start listening.
    if (mObservers.size() == 1) {
        updateOrientation();
        mBackend.startListening();
    }


    // We need to send the current value to the added observer as soon as
    // possible but outside of the current stack.
    final ScreenOrientationObserver obs = observer;
    ThreadUtils.assertOnUiThread();
    ThreadUtils.postOnUiThread(// [wuweizuo auto add note here: new InnerRunnable(this)] new Runnable() {
        @Override
        public void run() {
            obs.onScreenOrientationChanged(mOrientation);
        }
    });
    */
    return NOERROR;
}

ECode ScreenOrientationListener::RemoveObserver(
    /* [in] */ ScreenOrientationObserver* observer)
{
    VALIDATE_NOT_NULL(observer);
    /*
    if (!mObservers.removeObserver(observer)) {
        Log.w(TAG, "Removing an inexistent observer!");
        return;
    }


    if (mObservers.isEmpty()) {
        // The last observer was removed, we should just stop listening.
        mBackend.stopListening();
    }
    */
    return NOERROR;
}

ScreenOrientationListener::ScreenOrientationListener()
    : new ScreenOrientationConfigurationListener();
     }
{
    /*
    mBackend = Build.VERSION.SDK_INT >= 17 ?
            new ScreenOrientationDisplayListener() :
            new ScreenOrientationConfigurationListener();
    */
}

ECode ScreenOrientationListener::NotifyObservers()
{
    /*
    int previousOrientation = mOrientation;
    updateOrientation();


    DeviceDisplayInfo.create(mAppContext).updateNativeSharedDisplayInfo();


    if (mOrientation == previousOrientation) {
        return;
    }


    for (ScreenOrientationObserver observer : mObservers) {
        observer.onScreenOrientationChanged(mOrientation);
    }
    */
    return NOERROR;
}

ECode ScreenOrientationListener::UpdateOrientation()
{
    /*
    WindowManager windowManager =
            (WindowManager) mAppContext.getSystemService(Context.WINDOW_SERVICE);


    switch (windowManager.getDefaultDisplay().getRotation()) {
        case Surface.ROTATION_0:
            mOrientation = 0;
            break;
        case Surface.ROTATION_90:
            mOrientation = 90;
            break;
        case Surface.ROTATION_180:
            mOrientation = 180;
            break;
        case Surface.ROTATION_270:
            mOrientation = -90;
            break;
        default:
            throw new IllegalStateException(
                    "Display.getRotation() shouldn't return that value");
    }
    */
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


