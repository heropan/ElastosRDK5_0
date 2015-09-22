
#include "power/ShutdownThread.h"
#include "power/CMountShutdownObserver.h"
#include "power/CPowerManagerService.h"
#include "power/MobileDirectController.h"
#include "os/SystemClock.h"
#include "os/ServiceManager.h"
#include "app/ActivityManagerNative.h"
#include "R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::ISynchronize;
using Elastos::Core::ICharSequence;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnMultiChoiceClickListener;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::App::CProgressDialog;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::IVibrator;
using Elastos::Droid::Os::CSystemVibrator;
using Elastos::Droid::Os::Storage::IMountShutdownObserver;
using Elastos::Droid::Os::Storage::IMountService;
using Elastos::Droid::Bluetooth::IIBluetoothManager;
using Elastos::Droid::Bluetooth::IBluetoothAdapter;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsSystem;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Media::IMediaPlayer;
using Elastos::Droid::Media::CMediaPlayer;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

ShutdownThread::CloseDialogReceiver::CloseDialogReceiver(
    /* [in] */IContext* context)
    : mContext(context)
{
    AutoPtr<IIntentFilter> filter;
    ASSERT_SUCCEEDED(CIntentFilter::New(IIntent::ACTION_CLOSE_SYSTEM_DIALOGS, (IIntentFilter**)&filter));
    AutoPtr<IIntent> intent;
    ASSERT_SUCCEEDED(context->RegisterReceiver((IBroadcastReceiver*)this,
            filter, (IIntent**)&intent));
}

PInterface ShutdownThread::CloseDialogReceiver::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == Elastos::Droid::Content::EIID_IDialogInterfaceOnDismissListener) {
        return (IDialogInterfaceOnDismissListener*)this;
    }
    return BroadcastReceiver::Probe(riid);
}

UInt32 ShutdownThread::CloseDialogReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ShutdownThread::CloseDialogReceiver::Release()
{
    return ElRefBase::Release();
}

ECode ShutdownThread::CloseDialogReceiver::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IDialogInterfaceOnDismissListener*)this) {
        *pIID = Elastos::Droid::Content::EIID_IDialogInterfaceOnDismissListener;
        return NOERROR;
    }
    return BroadcastReceiver::GetInterfaceID(pObject, pIID);
}

ECode ShutdownThread::CloseDialogReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return mDialog->Cancel();
}

ECode ShutdownThread::CloseDialogReceiver::OnDismiss(
    /* [in] */ IDialogInterface* unused)
{
    return mContext->UnregisterReceiver((IBroadcastReceiver*)this);
}


ShutdownThread::ShutdownRadiosThread::ShutdownRadiosThread(
    /* [in] */ ArrayOf<Boolean>* done,
    /* [in] */ Int64 endTime)
    : mEndTime(endTime)
{
    Thread::constructor();
    mDone = done->Clone();
}

ECode ShutdownThread::ShutdownRadiosThread::Run()
{
    // Boolean nfcOff;
    Boolean bluetoothOff;
    // Boolean radioOff;

    // final INfcAdapter nfc =
    //         INfcAdapter.Stub.asInterface(ServiceManager.checkService("nfc"));
    // final ITelephony phone =
    //         ITelephony.Stub.asInterface(ServiceManager.checkService("phone"));
    // final IBluetoothManager bluetooth =
    //                     IBluetoothManager.Stub.asInterface(ServiceManager.checkService(
    //                             BluetoothAdapter.BLUETOOTH_MANAGER_SERVICE));
    AutoPtr<IIBluetoothManager> bluetooth =
            (IIBluetoothManager*)ServiceManager::CheckService(
                    IBluetoothAdapter::BLUETOOTH_MANAGER_SERVICE).Get();

    // try {
    //     nfcOff = nfc == null ||
    //              nfc.getState() == NfcAdapter.STATE_OFF;
    //     if (!nfcOff) {
    //         Log.w(TAG, "Turning off NFC...");
    //         nfc.disable(false); // Don't persist new state
    //     }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "RemoteException during NFC shutdown", ex);
    //     nfcOff = true;
    // }

    // try {
    Boolean isEnabled;
    bluetoothOff = bluetooth == NULL || (bluetooth->IsEnabled(&isEnabled), !isEnabled);
    if (!bluetoothOff) {
        Slogger::W(TAG, "Disabling Bluetooth...");
        Boolean result;
        if (FAILED(bluetooth->Disable(FALSE, &result))) {
            Slogger::E(TAG, "RemoteException during bluetooth shutdown");
            bluetoothOff = TRUE;
        }  // disable but don't persist new state
    }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "RemoteException during bluetooth shutdown", ex);
    //     bluetoothOff = true;
    // }

    // try {
    //     radioOff = phone == null || !phone.isRadioOn();
    //     if (!radioOff) {
    //         Log.w(TAG, "Turning off radio...");
    //         phone.setRadio(false);
    //     }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "RemoteException during radio shutdown", ex);
    //     radioOff = true;
    // }

    Slogger::I(TAG, "Waiting for NFC, Bluetooth and Radio...");

    while (SystemClock::GetElapsedRealtime() < mEndTime) {
        if (!bluetoothOff) {
            // try {
            Boolean result;
            if (FAILED(bluetooth->IsEnabled(&result))) {
                Slogger::E(TAG, "RemoteException during bluetooth shutdown");
                result = FALSE;
            }
            bluetoothOff = !result;
            // } catch (RemoteException ex) {
            //     Log.e(TAG, "RemoteException during bluetooth shutdown", ex);
            //     bluetoothOff = true;
            // }
            if (bluetoothOff) {
                Slogger::I(TAG, "Bluetooth turned off.");
            }
        }
        // if (!radioOff) {
        //     try {
        //         radioOff = !phone.isRadioOn();
        //     } catch (RemoteException ex) {
        //         Log.e(TAG, "RemoteException during radio shutdown", ex);
        //         radioOff = true;
        //     }
        //     if (radioOff) {
        //         Log.i(TAG, "Radio turned off.");
        //     }
        // }
        // if (!nfcOff) {
        //     try {
        //         nfcOff = nfc.getState() == NfcAdapter.STATE_OFF;
        //     } catch (RemoteException ex) {
        //         Log.e(TAG, "RemoteException during NFC shutdown", ex);
        //         nfcOff = true;
        //     }
        //     if (radioOff) {
        //         Log.i(TAG, "NFC turned off.");
        //     }
        // }

        // if (radioOff && bluetoothOff && nfcOff) {
        //     Log.i(TAG, "NFC, Radio and Bluetooth shutdown complete.");
        //     done[0] = true;
        //     break;
        // }
        SystemClock::Sleep(PHONE_STATE_POLL_SLEEP_MSEC);
    }
    return NOERROR;
}


ShutdownThread::ShutdownThreadHandler::ShutdownThreadHandler(
    /* [in] */ IProgressDialog* pd)
    : mPd(pd)
{}

ECode ShutdownThread::ShutdownThreadHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Slogger::V("ShutdownThread", "close process dialog now");
    mPd->Dismiss();
    return NOERROR;
}

const String ShutdownThread::SHUTDOWN_ACTION_PROPERTY("sys.shutdown.requested");
const String ShutdownThread::REBOOT_SAFEMODE_PROPERTY("persist.sys.safemode");
const String ShutdownThread::TAG("ShutdownThread");
const Int32 ShutdownThread::PHONE_STATE_POLL_SLEEP_MSEC = 500;
const Int32 ShutdownThread::MAX_BROADCAST_TIME = 10*1000;
const Int32 ShutdownThread::MAX_SHUTDOWN_WAIT_TIME = 20*1000;
const Int32 ShutdownThread::MAX_RADIO_WAIT_TIME = 12*1000;
const Int32 ShutdownThread::SHUTDOWN_VIBRATE_MS = 500;
const Int32 ShutdownThread::CLOSE_PROCESS_DIALOG = 2;
const Int32 ShutdownThread::MAX_SERVICES = 100;
const Int32 ShutdownThread::MAX_ACTIVITYS = 100;
const Int32 ShutdownThread::BOOTFAST_WAIT_TIME = 2000;
Mutex ShutdownThread::sIsStartedGuard;
Boolean ShutdownThread::sIsStarted = FALSE;
Boolean ShutdownThread::mReboot = FALSE;
Boolean ShutdownThread::mRebootSafeMode = FALSE;
AutoPtr<IWindowManagerPolicy> ShutdownThread::mPolicy;
String ShutdownThread::mRebootReason(NULL);
AutoPtr<ShutdownThread> ShutdownThread::sInstance;
Boolean ShutdownThread::mBootFastEnable = FALSE;
AutoPtr<IAlertDialog> ShutdownThread::sConfirmDialog;

ShutdownThread::ShutdownThread()
    : mActionDone(FALSE)
{
    Thread::constructor();
}

ShutdownThread::~ShutdownThread()
{
}

void ShutdownThread::Shutdown(
    /* [in] */ IContext* context,
    /* [in] */ Boolean confirm,
    /* [in] */ IWindowManagerPolicy* policy)
{
    mReboot = FALSE;
    mRebootSafeMode = FALSE;
    mPolicy = policy;
    ShutdownInner(context, confirm);
}

ShutdownThread::DialogInterfaceOnClickListener::DialogInterfaceOnClickListener(
    /* [in] */ IContext* context)
    : mContext(context)
{}

CAR_INTERFACE_IMPL(ShutdownThread::DialogInterfaceOnClickListener, IDialogInterfaceOnClickListener)

ECode ShutdownThread::DialogInterfaceOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    ShutdownThread::BeginShutdownSequence(mContext);
    return NOERROR;
}


ShutdownThread::DialogInterfaceOnMultiChoiceClickListener::DialogInterfaceOnMultiChoiceClickListener(
    /* [in] */ IContext* context)
    : mContext(context)
{}

CAR_INTERFACE_IMPL(ShutdownThread::DialogInterfaceOnMultiChoiceClickListener, IDialogInterfaceOnMultiChoiceClickListener);

ECode ShutdownThread::DialogInterfaceOnMultiChoiceClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which,
    /* [in] */ Boolean isChecked)
{
    Slogger::D(TAG, "which = %disChecked = %d", which, isChecked);
    if (which == 0) {
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        assert(0);
        // AutoPtr<ISettingsSystem> settingsSystem;
        // CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingsSystem);
        // if(isChecked){
        //     settingsSystem->PutInt32ForUser(cr, ISettingsSystem::BOOT_FAST_ENABLE, 1, IUserHandle::USER_CURRENT);
        // }
        // else{
        //     settingsSystem->PutInt32ForUser(cr, ISettingsSystem::BOOT_FAST_ENABLE, 0, IUserHandle::USER_CURRENT);
        // }
    }
    return NOERROR;
}


ShutdownThread::DialogInterfaceOnClickListener2::DialogInterfaceOnClickListener2(
    /* [in] */ IContext* context,
    /* [in] */ IWindowManagerPolicy* policy)
    : mContext(context)
    , mPolicy(policy)
{}

CAR_INTERFACE_IMPL(ShutdownThread::DialogInterfaceOnClickListener2, IDialogInterfaceOnClickListener);

ECode ShutdownThread::DialogInterfaceOnClickListener2::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    if (mPolicy != NULL) {
        mPolicy->AcquireBAView();
    }
    ShutdownThread::BeginShutdownSequence(mContext);
    return NOERROR;
}


void ShutdownThread::ShutdownInner(
    /* [in] */ IContext* context,
    /* [in] */ Boolean confirm)
{
    // ensure that only one thread is trying to power down.
    // any additional calls are just returned
    {
        AutoLock lock(sIsStartedGuard);
        if (sIsStarted) {
            Slogger::D(TAG, "Request to shutdown already running, returning.");
            return;
        }
    }

    AutoPtr<IResources> res;
    ASSERT_SUCCEEDED(context->GetResources((IResources**)&res));
    Int32 longPressBehavior;
    res->GetInteger(R::integer::config_longPressOnPowerBehavior, &longPressBehavior);
    Int32 resourceId = mRebootSafeMode
            ? R::string::reboot_safemode_confirm
            : (longPressBehavior == 2
                    ? R::string::shutdown_confirm_question
                    : R::string::shutdown_confirm);

    Slogger::D(TAG, "Notifying thread to start shutdown longPressBehavior=%d", longPressBehavior);

    if (confirm) {
        AutoPtr<CloseDialogReceiver> closer = new CloseDialogReceiver(context);
        if (sConfirmDialog != NULL) {
            sConfirmDialog->Dismiss();
        }
        if(mRebootSafeMode == TRUE){
            AutoPtr<IAlertDialogBuilder> dialogBuilder;
            ASSERT_SUCCEEDED(CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&dialogBuilder));
            dialogBuilder->SetTitle(mRebootSafeMode
                    ? R::string::reboot_safemode_title
                    : R::string::power_off);
            dialogBuilder->SetMessage(resourceId);
            AutoPtr<IDialogInterfaceOnClickListener> listener = new DialogInterfaceOnClickListener(context);
            dialogBuilder->SetPositiveButton(R::string::yes, listener);
            dialogBuilder->SetNegativeButton(R::string::no, NULL);
            ASSERT_SUCCEEDED(dialogBuilder->Create((IAlertDialog**)&sConfirmDialog));
        }
        else{
            AutoPtr<IContentResolver> cr;
            context->GetContentResolver((IContentResolver**)&cr);
            Int32 value;
            AutoPtr<ISystemProperties> sysProp;
            CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
            Boolean bootfast;
            AutoPtr<ISettingsGlobal> settingsGlobal;
            CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
            if ((settingsGlobal->GetInt32(cr, ISettingsGlobal::DEVICE_PROVISIONED, 1, &value), value == 1) &&
                    (sysProp->GetBoolean(String("ro.sys.bootfast"), FALSE, &bootfast), bootfast)) {
                AutoPtr< ArrayOf<Boolean> > enableBootFast = ArrayOf<Boolean>::Alloc(1);
                (*enableBootFast)[0] = FALSE;
                Int32 v = 0;
                // AutoPtr<ISettingsSystem> settingsSystem;
                // CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingsSystem);
                // settingsSystem->GetInt32ForUser(cr, ISettingsSystem::BOOT_FAST_ENABLE, 0, IUserHandle::USER_CURRENT, &v);
                (*enableBootFast)[0]  = v == 0 ? FALSE : TRUE;
                AutoPtr<IAlertDialogBuilder> dialogBuilder;
                ASSERT_SUCCEEDED(CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&dialogBuilder));
                dialogBuilder->SetTitle(mRebootSafeMode
                        ? R::string::reboot_safemode_title
                        : R::string::power_off);
                AutoPtr<IDialogInterfaceOnMultiChoiceClickListener> multiListener = new DialogInterfaceOnMultiChoiceClickListener(context);
                dialogBuilder->SetMultiChoiceItems(R::array::quick_boot_mode, enableBootFast, multiListener);
                AutoPtr<IDialogInterfaceOnClickListener> listener = new DialogInterfaceOnClickListener2(context, mPolicy);
                dialogBuilder->SetPositiveButton(R::string::yes, listener);
                dialogBuilder->SetNegativeButton(R::string::no, NULL);
                ASSERT_SUCCEEDED(dialogBuilder->Create((IAlertDialog**)&sConfirmDialog));
            }
            else {
                AutoPtr<IAlertDialogBuilder> dialogBuilder;
                ASSERT_SUCCEEDED(CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&dialogBuilder));
                dialogBuilder->SetTitle(mRebootSafeMode
                        ? R::string::reboot_safemode_title
                        : R::string::power_off);
                dialogBuilder->SetMessage(resourceId);
                AutoPtr<IDialogInterfaceOnClickListener> listener = new DialogInterfaceOnClickListener(context);
                dialogBuilder->SetPositiveButton(R::string::yes, listener);
                dialogBuilder->SetNegativeButton(R::string::no, NULL);
                ASSERT_SUCCEEDED(dialogBuilder->Create((IAlertDialog**)&sConfirmDialog));
            }
        }

        closer->mDialog = sConfirmDialog;
        sConfirmDialog->SetOnDismissListener(closer);
        AutoPtr<IWindow> win;
        ASSERT_SUCCEEDED(sConfirmDialog->GetWindow((IWindow**)&win));
        win->SetType(IWindowManagerLayoutParams::TYPE_KEYGUARD_DIALOG);
        ASSERT_SUCCEEDED(sConfirmDialog->Show());
    }
    else {
        BeginShutdownSequence(context);
    }
}

void ShutdownThread::Reboot(
    /* [in] */ IContext* context,
    /* [in] */ const String& reason,
    /* [in] */ Boolean confirm)
{
    mReboot = TRUE;
    mRebootSafeMode = FALSE;
    mRebootReason = reason;
    if(!reason.IsNull()){
        Slogger::D(TAG, "reboot reason is %s", mRebootReason.string());
    }
    ShutdownInner(context, confirm);
}

void ShutdownThread::RebootSafeMode(
    /* [in] */ IContext* context,
    /* [in] */ Boolean confirm)
{
    mReboot = TRUE;
    mRebootSafeMode = TRUE;
    mRebootReason = NULL;
    ShutdownInner(context, confirm);
}

void ShutdownThread::SaveAirplaneModeState(
    /* [in] */ Boolean enabled)
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 value;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(cr, ISettingsGlobal::AIRPLANE_MODE_ON_FAST_BOOT, 0, &value);
    Boolean lastState = value == 1 ? TRUE : FALSE;
    if (lastState == enabled) {
        return;
    }
    Boolean result;
    settingsGlobal->PutInt32(cr, ISettingsGlobal::AIRPLANE_MODE_ON_FAST_BOOT, enabled ? 1 : 0, &result);
}

void ShutdownThread::EnableAirplaneModeState()
{
    Slogger::D(TAG, "enableAirplaneModeState");
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 value;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(cr, ISettingsGlobal::AIRPLANE_MODE_ON, 0, &value);
    Boolean enable = value == 1 ? TRUE : FALSE;
    SaveAirplaneModeState(enable);
    if(enable) {
        Slogger::D(TAG, "already enable airplane mode");
        return;
    }
    Boolean result;
    settingsGlobal->PutInt32(cr, ISettingsGlobal::AIRPLANE_MODE_ON, 1, &result);
    // Post the intent
    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_AIRPLANE_MODE_CHANGED, (IIntent**)&intent);
    intent->PutBooleanExtra(String("state"), enable);
    mContext->SendBroadcast(intent);
}

void ShutdownThread::SetAirplaneModeState(
    /* [in] */ Boolean enabled)
{
    // TODO: Sets the view to be "awaiting" if not already awaiting

    Slogger::D(TAG, "setAirplaneModeState = %d", enabled);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 value;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(cr, ISettingsGlobal::AIRPLANE_MODE_ON, 0, &value);
    if(value == 1){
        Slogger::D(TAG, "already in Airplane mode return");
        return;
    }
    Boolean result;
    settingsGlobal->PutInt32(cr, ISettingsGlobal::AIRPLANE_MODE_ON, enabled ? 1 : 0, &result);
    // Post the intent
    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_AIRPLANE_MODE_CHANGED, (IIntent**)&intent);
    intent->PutBooleanExtra(String("state"), enabled);
    mContext->SendBroadcast(intent);
}

void ShutdownThread::BeginShutdownSequence(
    /* [in] */ IContext* context)
{
    {
        AutoLock lock(sIsStartedGuard);
        if (sIsStarted) {
            Slogger::D(TAG, "Shutdown sequence already running, returning.");
            return;
        }
        sIsStarted = TRUE;
    }

    AutoPtr<IMediaPlayer> mediaplayer;
    CMediaPlayer::New((IMediaPlayer**)&mediaplayer);
    // try{
    mediaplayer->SetDataSource(String("/system/media/shutdown.mp3"));
    mediaplayer->Prepare();
    mediaplayer->SetLooping(TRUE);
    mediaplayer->Start();
    // }catch (IOException e){

    // }
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    sysProp->Set(String("sys.start_shutdown"), String("1"));

    Boolean isBootfast = FALSE;
    sysProp->GetBoolean(String("ro.sys.bootfast"), FALSE, &isBootfast);
    AutoPtr<IContentResolver> contentResolver;
    context->GetContentResolver((IContentResolver**)&contentResolver);
    Int32 bootfaseEnable = 0;
    AutoPtr<ISettingsSystem> settingsSystem;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingsSystem);
    settingsSystem->GetInt32ForUser(contentResolver, ISettingsSystem::BOOT_FAST_ENABLE, 0, IUserHandle::USER_CURRENT, &bootfaseEnable);
    if(isBootfast && 1 == bootfaseEnable) {
        mBootFastEnable = TRUE;
    } else {
        mBootFastEnable = FALSE;
    }
    if(mReboot){
        mBootFastEnable = FALSE;
        Slogger::D(TAG, "reboot!");
    }
    if(mRebootSafeMode){
        mBootFastEnable = FALSE;
        Slogger::D(TAG, "Go Into Safe Mode real reboot");
    }
    // if(Zygote.systemInSafeMode){
    //     mBootFastEnable = FALSE;
    //     Slogger::D(TAG, "In Safe Mode real reboot");
    // }
    if(!mRebootReason.IsNull()){
        mBootFastEnable = FALSE;
        Slogger::D(TAG, "have reason %sreal reboot", mRebootReason.string());
    }
    Int32 value;
    if(sysProp->GetInt32(String("sys.battery_zero"), 0, &value), value == 1){
        mBootFastEnable = FALSE;
        Slogger::D(TAG, "Battery to low we really shutdown!");
    }
    if(sysProp->GetInt32(String("sys.temperature_high"), 0, &value), value == 1){
        mBootFastEnable = FALSE;
        Slogger::D(TAG, "temperature high we relly shutdown!");
    }

    // throw up an indeterminate system dialog to indicate radio is
    // shutting down.
    AutoPtr<IProgressDialog> pd;
    CProgressDialog::New(context, (IProgressDialog**)&pd);
    AutoPtr<ICharSequence> csq;
    context->GetText(R::string::power_off, (ICharSequence**)&csq);
    pd->SetTitle(csq);
    csq = NULL;
    context->GetText(R::string::shutdown_progress, (ICharSequence**)&csq);
    pd->SetMessage(csq);
    pd->SetIndeterminate(TRUE);
    pd->SetCancelable(FALSE);
    AutoPtr<IWindow> win;
    ASSERT_SUCCEEDED(pd->GetWindow((IWindow**)&win));
    win->SetType(IWindowManagerLayoutParams::TYPE_KEYGUARD_DIALOG);
    ASSERT_SUCCEEDED(pd->Show());

    sInstance = new ShutdownThread();
    sInstance->mContext = context;
    context->GetSystemService(IContext::POWER_SERVICE,
            (IInterface**)&sInstance->mPowerManager);

    // make sure we never fall asleep again
    sInstance->mCpuWakeLock = NULL;
    // try {
    ECode ec = sInstance->mPowerManager->NewWakeLock(
            IPowerManager::PARTIAL_WAKE_LOCK, TAG + String("-cpu"),
            (IPowerManagerWakeLock**)&sInstance->mCpuWakeLock);
    if (ec == (ECode)E_SECURITY_EXCEPTION) {
        Slogger::W(TAG, "No permission to acquire wake lock 0x%08x", ec);
        sInstance->mCpuWakeLock = NULL;
    }
    if (sInstance->mCpuWakeLock != NULL) {
        sInstance->mCpuWakeLock->SetReferenceCounted(FALSE);
        sInstance->mCpuWakeLock->AcquireLock();
    }
    // } catch (SecurityException e) {
    //     Log.w(TAG, "No permission to acquire wake lock", e);
    //     sInstance.mCpuWakeLock = null;
    // }

    // also make sure the screen stays on for better user experience
    sInstance->mScreenWakeLock = NULL;
    Boolean isScreenOn;
    if (sInstance->mPowerManager->IsScreenOn(&isScreenOn), isScreenOn) {
        // try {
        ec = sInstance->mPowerManager->NewWakeLock(
                IPowerManager::FULL_WAKE_LOCK, TAG + String("-screen"),
                (IPowerManagerWakeLock**)&sInstance->mScreenWakeLock);
        if (ec == (ECode)E_SECURITY_EXCEPTION) {
            Slogger::W(TAG, "No permission to acquire wake lock %08x0x", ec);
            sInstance->mScreenWakeLock = NULL;
        }
        if (sInstance->mScreenWakeLock != NULL) {
            sInstance->mScreenWakeLock->SetReferenceCounted(FALSE);
            sInstance->mScreenWakeLock->AcquireLock();
        }
        // } catch (SecurityException e) {
        //     Log.w(TAG, "No permission to acquire wake lock", e);
        //     sInstance.mScreenWakeLock = null;
        // }
    }

    // start the thread that initiates shutdown
    sInstance->mHandler = new ShutdownThreadHandler(pd);
    sInstance->Start();
}

void ShutdownThread::ActionDone()
{
    AutoLock lock(mActionDoneSync);
    mActionDone = TRUE;
    mActionDoneSync.NotifyAll();
}

ShutdownThread::ActionDoneBroadcastReceiver::ActionDoneBroadcastReceiver(
    /* [in] */ ShutdownThread* host)
    : mHost(host)
{}

ECode ShutdownThread::ActionDoneBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    mHost->ActionDone();
    return NOERROR;
}

ECode ShutdownThread::Run()
{
    AutoPtr<IBroadcastReceiver> br = (IBroadcastReceiver*)new ActionDoneBroadcastReceiver(this);
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    if(!mBootFastEnable){
        /*
         * Write a system property in case the system_server reboots before we
         * get to the actual hardware restart. If that happens, we'll retry at
         * the beginning of the SystemServer startup.
         */
        {
            String reason = (mReboot ? String("1") : String("0"))
                    + (mRebootReason != NULL ? mRebootReason : String(""));
            sysProp->Set(SHUTDOWN_ACTION_PROPERTY, reason);
        }

        /*
         * If we are rebooting into safe mode, write a system property
         * indicating so.
         */
        if (mRebootSafeMode) {
            sysProp->Set(REBOOT_SAFEMODE_PROPERTY, String("1"));
        }

        KillRemoveActivity(mContext);
        KillRemoveService(mContext);
        Slogger::I(TAG, "Sending shutdown broadcast...");

        // First send the high-level shut down broadcast.
        mActionDone = FALSE;
        AutoPtr<IIntent> intent;
        CIntent::New(IIntent::ACTION_SHUTDOWN, (IIntent**)&intent);
        AutoPtr<IUserHandleHelper> handleHelper;
        ASSERT_SUCCEEDED(CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&handleHelper));
        AutoPtr<IUserHandle> all;
        handleHelper->GetALL((IUserHandle**)&all);
        mContext->SendOrderedBroadcastAsUser(intent, all, String(NULL), br,
            (IHandler*)(mHandler->Probe(EIID_IHandler)), 0, String(NULL), NULL);

        Int64 endTime = SystemClock::GetElapsedRealtime() + MAX_BROADCAST_TIME;
        {
            AutoLock lock(mActionDoneSync);
            while (!mActionDone) {
                Int64 delay = endTime - SystemClock::GetElapsedRealtime();
                if (delay <= 0) {
                    Slogger::W(TAG, "Shutdown broadcast timed out");
                    break;
                }
                // try {
                mActionDoneSync.Wait(delay);
                // } catch (InterruptedException e) {
                // }
            }
        }

        Slogger::I(TAG, "Shutting down activity manager...");

        // final IActivityManager am =
        //         ActivityManagerNative.asInterface(ServiceManager.checkService("activity"));
        AutoPtr<IIActivityManager> am = (IIActivityManager*)ServiceManager::CheckService(
                String("activity")).Get();
        if (am != NULL) {
            // try {
            Boolean result;
            am->Shutdown(MAX_BROADCAST_TIME, &result);
            // } catch (RemoteException e) {
            // }
        }

        // Shutdown radios.
        // ShutdownRadios(MAX_RADIO_WAIT_TIME);
        String value;
        if(sysProp->Get(String("ro.sw.embeded.telephony"), &value), value.Equals("true")) {
            ShutdownRadios(MAX_RADIO_WAIT_TIME);
        }

        // Shutdown MountService to ensure media is in a safe state
        AutoPtr<IMountShutdownObserver> observer;
        CMountShutdownObserver::New((Handle32)this, (IMountShutdownObserver**)&observer);

        Slogger::I(TAG, "Shutting down MountService");

        // Set initial variables and time out time.
        mActionDone = FALSE;
        Int64 endShutTime = SystemClock::GetElapsedRealtime() + MAX_SHUTDOWN_WAIT_TIME;
        {
            AutoLock lock(mActionDoneSync);
            // try {
            // final IMountService mount = IMountService.Stub.asInterface(
            //         ServiceManager.checkService("mount"));
            AutoPtr<IMountService> mount = (IMountService*)ServiceManager::GetService(
                    String("mount")).Get();
            if (mount != NULL) {
                if (FAILED(mount->Shutdown(observer))) {
                    Slogger::E(TAG, "Exception during MountService shutdown");
                }
            }
            else {
                Slogger::W(TAG, "MountService unavailable for shutdown");
            }
            // } catch (Exception e) {
            //     Log.e(TAG, "Exception during MountService shutdown", e);
            // }
            while (!mActionDone) {
                Int64 delay = endShutTime - SystemClock::GetElapsedRealtime();
                if (delay <= 0) {
                    Slogger::W(TAG, "Shutdown wait timed out");
                    break;
                }
                // try {
                mActionDoneSync.Wait(delay);
                // } catch (InterruptedException e) {
                // }
            }
        }

        RebootOrShutdown(mReboot, mRebootReason);
    }

    if (SHUTDOWN_VIBRATE_MS > 0) {
        // vibrate before shutting down
        AutoPtr<IVibrator> vibrator;
        CSystemVibrator::New((IVibrator**)&vibrator);
        // try {
        if (FAILED(vibrator->Vibrate(SHUTDOWN_VIBRATE_MS))) {
            Slogger::W(TAG, "Failed to vibrate during shutdown.");
        }
        // } catch (Exception e) {
        //     // Failure to vibrate shouldn't interrupt shutdown.  Just log it.
        //     Log.w(TAG, "Failed to vibrate during shutdown.", e);
        // }
    }
    AutoPtr<IIWindowManager> windowManager = (IIWindowManager*)ServiceManager::GetService(String("window")).Get();
    if(windowManager != NULL) {
        // try{
        windowManager->FreezeRotation(ISurface::ROTATION_0);
        windowManager->UpdateRotation(TRUE, TRUE);
        windowManager->SetEventDispatching(FALSE);
        // }catch (RemoteException e) {
        // }
    }
    //shutdownRadios(MAX_RADIO_WAIT_TIME);
    //setAirplaneModeState(true); //set airplane mode
    EnableAirplaneModeState();
    KillRemoveActivity(mContext);
    KillRemoveService(mContext);
    AutoPtr<MobileDirectController> controller = MobileDirectController::GetInstance();
    if (controller->IsMobileModeAvailable()) {
        controller->SetNetworkEnable(FALSE);
    }

    SystemClock::Sleep(BOOTFAST_WAIT_TIME);

    sIsStarted = FALSE;
    // try{
    ECode ec = sInstance->mCpuWakeLock->Release();
    if (ec == (ECode)E_SECURITY_EXCEPTION) {
        sysProp->Set(String("sys.start_shutdown"), String("0"));
    }
    ec = sInstance->mScreenWakeLock->Release();
    if (ec == (ECode)E_SECURITY_EXCEPTION) {
        sysProp->Set(String("sys.start_shutdown"), String("0"));
    }
    // }catch(SecurityException e){
    //     SystemProperties.set("sys.start_shutdown", "0");
    // }
    //
    Boolean result;
    sInstance->mHandler->SendEmptyMessage(CLOSE_PROCESS_DIALOG, &result);
    Slogger::V(TAG, "CLOSE_PROCESS_DIALOG");
    sysProp->Set(String("sys.start_shutdown"), String("0"));
    sInstance->mPowerManager->GoToBootFastSleep(SystemClock::GetUptimeMillis());

    return NOERROR;
}

void ShutdownThread::KillRemoveActivity(
    /* [in] */ IContext* context)
{
    AutoPtr<IActivityManager> am;
    context->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&am);
    AutoPtr<IObjectContainer> recents;
    am->GetRecentTasks(MAX_ACTIVITYS, IActivityManager::RECENT_WITH_EXCLUDED, (IObjectContainer**)&recents);
    if(recents != NULL) {
        // Slogger::V(TAG,"Task Size is " + recents.size());
        Boolean hasNext = FALSE;
        AutoPtr<IObjectEnumerator> enumerator;
        recents->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IActivityManagerRecentTaskInfo> task;
            enumerator->Current((IInterface**)&task);
            if (task != NULL) {
                Int32 id;
                task->GetPersistentId(&id);
                AutoPtr<IActivityManagerTaskThumbnails> thumbs;
                am->GetTaskThumbnails(id, (IActivityManagerTaskThumbnails**)&thumbs);
                if(id > 0) {
                    AutoPtr<IIntent> intent;
                    task->GetBaseIntent((IIntent**)&intent);
                    AutoPtr<IComponentName> component;
                    intent->GetComponent((IComponentName**)&component);
                    String packageName;
                    component->GetPackageName(&packageName);
                    if(!packageName.Equals("com.android.launcher")) {
                        Boolean result;
                        am->RemoveTask(id, IActivityManager::REMOVE_TASK_KILL_PROCESS, &result);
                        Slogger::V(TAG, "remove a task %s", packageName.string());
                        Int32 numSubThumbbails;
                        thumbs->GetNumSubThumbbails(&numSubThumbbails);
                        if(numSubThumbbails > 0) {
                            for (Int32 j = 0; j < numSubThumbbails; j++) {
                                am->RemoveSubTask(id, j, &result);
                                Slogger::V(TAG, "remove a sub task ");
                            }
                        }
                    }
                }
            }
        }
     }
}

void ShutdownThread::KillRemoveService(
    /* [in] */ IContext* context)
{
    AutoPtr<IActivityManager> am;
    context->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&am);
    AutoPtr<IObjectContainer> services;
    am->GetRunningServices(MAX_SERVICES, (IObjectContainer**)&services);
    if (services != NULL) {
        Boolean hasNext = FALSE;
        AutoPtr<IObjectEnumerator> enumerator;
        services->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IActivityManagerRunningServiceInfo> si;
            enumerator->Current((IInterface**)&si);
            // We are not interested in services that have not been started
            // and don't have a known client, because
            // there is nothing the user can do about them.
            Boolean started;
            Int32 clientLabel;
            if ((si->GetStarted(&started), !started) && (si->GetClientLabel(&clientLabel), clientLabel == 0)) {
                services->Remove((IInterface*)si.Get());
                continue;
            }
            // We likewise don't care about services running in a
            // persistent process like the system or phone.
            Int32 flags;
            si->GetFlags(&flags);
            if ((flags & IActivityManagerRunningServiceInfo::FLAG_PERSISTENT_PROCESS) != 0) {
                services->Remove((IInterface*)si.Get());
                continue;
            }
            String process;
            si->GetProcess(&process);
            Slogger::V(TAG, "service = %s", process.string());
            AutoPtr<IIntent> intent;
            CIntent::New((IIntent**)&intent);
            AutoPtr<IComponentName> cn;
            si->GetService((IComponentName**)&cn);
            intent->SetComponent(cn);
            Boolean result;
            context->StopService(intent, &result);
        }
    }
}

void ShutdownThread::ShutdownRadios(
    /* [in] */ Int32 timeout)
{
    // If a radio is wedged, disabling it may hang so we do this work in another thread,
    // just in case.
    Int64 endTime = SystemClock::GetElapsedRealtime() + timeout;
    AutoPtr< ArrayOf<Boolean> > done = ArrayOf<Boolean>::Alloc(1);
    AutoPtr<IThread> t = new ShutdownRadiosThread(done, endTime);

    t->Start();
    // try {
    t->Join(timeout);
    // } catch (InterruptedException ex) {
    // }
    if (!(*done)[0]) {
        Slogger::W(TAG, "Timed out waiting for NFC, Radio and Bluetooth shutdown.");
    }
}

void ShutdownThread::RebootOrShutdown(
    /* [in] */ Boolean reboot,
    /* [in] */ const String& reason)
{
    if (reboot) {
        Slogger::I(TAG, "Rebooting, reason: %s", (const char*)reason);
        // try {
        CPowerManagerService::LowLevelReboot(reason);
        // } catch (Exception e) {
        //     Log.e(TAG, "Reboot failed, will attempt shutdown instead", e);
        // }
    }
    else if (SHUTDOWN_VIBRATE_MS > 0) {
        // vibrate before shutting down
        AutoPtr<IVibrator> vibrator;
        CSystemVibrator::New((IVibrator**)&vibrator);
        // try {
        if (FAILED(vibrator->Vibrate(SHUTDOWN_VIBRATE_MS))) {
            Slogger::W(TAG, "Failed to vibrate during shutdown.");
        }
        // } catch (Exception e) {
        //     // Failure to vibrate shouldn't interrupt shutdown.  Just log it.
        //     Log.w(TAG, "Failed to vibrate during shutdown.", e);
        // }

        // vibrator is asynchronous so we need to wait to avoid shutting down too soon.
        // try {
        Thread::Sleep(SHUTDOWN_VIBRATE_MS);
        // } catch (InterruptedException unused) {
        // }
    }

    // Shutdown power
    Slogger::I(TAG, "Performing low-level shutdown...");
    CPowerManagerService::LowLevelShutdown();
}

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

