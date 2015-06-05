
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

//===============================================================
//                  PowerMonitor::LazyHolder
//===============================================================

const AutoPtr<PowerMonitor> PowerMonitor::LazyHolder::INSTANCE = new PowerMonitor();

//===============================================================
//                 PowerMonitor::InnerRunnable
//===============================================================

PowerMonitor::InnerRunnable::InnerRunnable(
    /* [in] */ PowerMonitor* owner)
    : mOwner(owner)
{
}

ECode PowerMonitor::InnerRunnable::Run()
{
    NativeOnMainActivitySuspended();
}

//===============================================================
//                       PowerMonitor
//===============================================================

const Int64 PowerMonitor::SUSPEND_DELAY_MS;
AutoPtr<PowerMonitor> PowerMonitor::sInstance;

static AutoPtr<IRunnable> Runnable_Create()
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this);
    return runnable;
}

const AutoPtr<IRunnable> sSuspendTask = Runnable_Create();


static AutoPtr<IHandler> Handler_Create()
{
    AutoPtr<ILooperHelper> helper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
    AutoPtr<IHandler> handler;
    helper->GetMainLooper((IHandler**)&handler);
    return handler;
}

PowerMonitor::PowerMonitor()
    : mIsBatteryPower(FALSE)
    , mHandler(Handler_Create())
{
}


void PowerMonitor::CreateForTests(
    /* [in] */ IContext* context)
{
    // Applications will create this once the JNI side has been fully wired up both sides. For
    // tests, we just need native -> java, that is, we don't need to notify java -> native on
    // creation.
    sInstance = LazyHolder::INSTANCE;
}

/**
 * Create a PowerMonitor instance if none exists.
 * @param context The context to register broadcast receivers for.  The application context
 *                will be used from this parameter.
 */
void PowerMonitor::Create(
    /* [in] */ IContext* context)
{
    context->GetApplicationContext((IContext**)&context);
    if (sInstance == NULL) {
        sInstance = LazyHolder::INSTANCE;
        ApplicationStatus::RegisterApplicationStateListener(sInstance);
        AutoPtr<IIntentFilter> ifilter;
        CIntentFilter(IIntent::ACTION_BATTERY_CHANGED, (IIntentFilter**)&ifilter);
        AutoPtr<IIntent> batteryStatusIntent;
        context->RegisterReceiver(NULL, ifilter, (IIntent**)&batteryStatusIntent);
        OnBatteryChargingChanged(batteryStatusIntent);
    }
}

void PowerMonitor::OnBatteryChargingChanged(
    /* [in] */ IIntent* intent)
{
    if (sInstance == NULL) {
        // We may be called by the framework intent-filter before being fully initialized. This
        // is not a problem, since our constructor will check for the state later on.
        return;
    }
    Int32 chargePlug;
    intent->GetIntExtra(IBatteryManager::EXTRA_PLUGGED, -1, &chargePlug);
    // If we're not plugged, assume we're running on battery power.
    sInstance->mIsBatteryPower = chargePlug != IBatteryManager::BATTERY_PLUGGED_USB &&
                                chargePlug != IBatteryManager::BATTERY_PLUGGED_AC;
    nativeOnBatteryChargingChanged();
}

void PowerMonitor::OnApplicationStateChange(
    /* [in] */ Int32 newState)
{
    if (newState == ApplicationState::HAS_RUNNING_ACTIVITIES) {
        // Remove the callback from the message loop in case it hasn't been executed yet.
        mHandler->RemoveCallbacks(sSuspendTask);
        NativeOnMainActivityResumed();
    }
    else if (newState == ApplicationState::HAS_PAUSED_ACTIVITIES) {
        mHandler->PostDelayed(sSuspendTask, SUSPEND_DELAY_MS);
    }
}

//@CalledByNative
Boolean PowerMonitor::IsBatteryPower()
{
    return sInstance->mIsBatteryPower;
}

void PowerMonitor::NativeOnBatteryChargingChanged()
{
}

void PowerMonitor::NativeOnMainActivitySuspended()
{
}

void PowerMonitor::NativeOnMainActivityResumed()
{
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos