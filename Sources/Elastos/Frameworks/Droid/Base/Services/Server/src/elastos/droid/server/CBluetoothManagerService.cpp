#include "elastos/droid/server/CBluetoothManagerService.h"
#include "elastos/droid/server/IoThread.h"
#include "elastos/droid/os/Looper.h"
#include "elastos/droid/os/SystemClock.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <Elastos.Droid.Os.h>
#include <Elastos.Droid.App.h>
#include <Elastos.Droid.Content.h>
#include <Elastos.Droid.Bluetooth.h>
#include <Elastos.CoreLibrary.Utility.h>

using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::App::EIID_IServiceConnection;
using Elastos::Droid::Bluetooth::EIID_IIBluetoothManager;
using Elastos::Droid::Bluetooth::IIBluetoothCallback;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {

const Boolean DBG = TRUE;
const char* TAG = "CBluetoothManagerService";

const String CBluetoothManagerService::BLUETOOTH_ADMIN_PERM("android.permission.BLUETOOTH_ADMIN");
const String CBluetoothManagerService::BLUETOOTH_PERM("android.permission.BLUETOOTH");
const String CBluetoothManagerService::ACTION_SERVICE_STATE_CHANGED;//="com.android.bluetooth.btservice.action.STATE_CHANGED";
const String CBluetoothManagerService::EXTRA_ACTION=String("action");
const String CBluetoothManagerService::SECURE_SETTINGS_BLUETOOTH_ADDR_VALID=String("bluetooth_addr_valid");
const String CBluetoothManagerService::SECURE_SETTINGS_BLUETOOTH_ADDRESS=String("bluetooth_address");
const String CBluetoothManagerService::SECURE_SETTINGS_BLUETOOTH_NAME=String("bluetooth_name");

const Int32 CBluetoothManagerService::TIMEOUT_BIND_MS = 3000; //Maximum msec to wait for a bind
const Int32 CBluetoothManagerService::TIMEOUT_SAVE_MS = 500; //Maximum msec to wait for a save
//Maximum msec to wait for service restart
const Int32 CBluetoothManagerService::SERVICE_RESTART_TIME_MS = 200;
//Maximum msec to delay MESSAGE_USER_SWITCHED
const Int32 CBluetoothManagerService::USER_SWITCHED_TIME_MS = 200;

const Int32 CBluetoothManagerService::MESSAGE_ENABLE = 1;
const Int32 CBluetoothManagerService::MESSAGE_DISABLE = 2;
const Int32 CBluetoothManagerService::MESSAGE_REGISTER_ADAPTER = 20;
const Int32 CBluetoothManagerService::MESSAGE_UNREGISTER_ADAPTER = 21;
const Int32 CBluetoothManagerService::MESSAGE_REGISTER_STATE_CHANGE_CALLBACK = 30;
const Int32 CBluetoothManagerService::MESSAGE_UNREGISTER_STATE_CHANGE_CALLBACK = 31;
const Int32 CBluetoothManagerService::MESSAGE_BLUETOOTH_SERVICE_CONNECTED = 40;
const Int32 CBluetoothManagerService::MESSAGE_BLUETOOTH_SERVICE_DISCONNECTED = 41;
const Int32 CBluetoothManagerService::MESSAGE_RESTART_BLUETOOTH_SERVICE = 42;
const Int32 CBluetoothManagerService::MESSAGE_BLUETOOTH_STATE_CHANGE=60;
const Int32 CBluetoothManagerService::MESSAGE_TIMEOUT_BIND =100;
const Int32 CBluetoothManagerService::MESSAGE_TIMEOUT_UNBIND =101;
const Int32 CBluetoothManagerService::MESSAGE_GET_NAME_AND_ADDRESS=200;
const Int32 CBluetoothManagerService::MESSAGE_SAVE_NAME_AND_ADDRESS=201;
const Int32 CBluetoothManagerService::MESSAGE_USER_SWITCHED = 300;
const Int32 CBluetoothManagerService::MAX_SAVE_RETRIES=3;
// Bluetooth persisted setting is off
const Int32 CBluetoothManagerService::BLUETOOTH_OFF=0;
// Bluetooth persisted setting is on
// and Airplane mode won't affect Bluetooth state at start up
const Int32 CBluetoothManagerService::BLUETOOTH_ON_BLUETOOTH=1;
// Bluetooth persisted setting is on
// but Airplane mode will affect Bluetooth state at start up
// and Airplane mode will have higher priority.
const Int32 CBluetoothManagerService::BLUETOOTH_ON_AIRPLANE=2;

CAR_INTERFACE_IMPL(CBluetoothManagerService::BluetoothServiceConnection, Object, IServiceConnection)

CBluetoothManagerService::BluetoothServiceConnection::BluetoothServiceConnection(
    /* [in] */ CBluetoothManagerService* host)
    : mHost(host)
{ }

void CBluetoothManagerService::BluetoothServiceConnection::SetGetNameAddressOnly(
    /* [in] */ Boolean getOnly)
{
    mGetNameAddressOnly = getOnly;
}

Boolean CBluetoothManagerService::BluetoothServiceConnection::IsGetNameAddressOnly()
{
    return mGetNameAddressOnly;
}

ECode CBluetoothManagerService::BluetoothServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* className,
    /* [in] */ IBinder* service)
{
    String clsName;
    className->GetClassName(&clsName);

    if (DBG) {
        Logger::D(TAG, "BluetoothServiceConnection: connected to AdapterService %s",
            clsName.string());
    }

    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(MESSAGE_BLUETOOTH_SERVICE_CONNECTED, (IMessage**)&msg);
    // TBD if (className.getClassName().equals(IBluetooth.class.getName())) {
    if (clsName.Equals("com.android.bluetooth.btservice.AdapterService")) {
        msg->SetArg1(SERVICE_IBLUETOOTH);
        // } else if (className.getClassName().equals(IBluetoothGatt.class.getName())) {
    }
    else if (clsName.Equals("com.android.bluetooth.gatt.GattService")) {
        msg->SetArg1(SERVICE_IBLUETOOTHGATT);
    }
     else {
        Logger::E(TAG, "Unknown service connected: %s", clsName.string());
        return NOERROR;
    }

    msg->SetObj(service);
    Boolean bval;
    return mHost->mHandler->SendMessage(msg, &bal);
}

ECode CBluetoothManagerService::BluetoothServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* className)
{
    String clsName;
    className->GetClassName(&clsName);

    // Called if we unexpected disconnected.
    if (DBG) Logger::D(TAG, "BluetoothServiceConnection: disconnected from AdapterService%s",
        clsName.string());

    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(MESSAGE_BLUETOOTH_SERVICE_DISCONNECTED, (IMessage**)&msg);
    // TBD if (className.getClassName().equals(IBluetooth.class.getName())) {
    if (clsName.Equals("com.android.bluetooth.btservice.AdapterService")) {
        msg->SetArg1(SERVICE_IBLUETOOTH);
        // } else if (className.getClassName().equals(IBluetoothGatt.class.getName())) {
    }
    else if (clsName.Equals("com.android.bluetooth.gatt.GattService")) {
        msg->SetArg1(SERVICE_IBLUETOOTHGATT);
    }
     else {
        Logger::E(TAG, "Unknown service disconnected: %s", clsName.string());
        return NOERROR;
    }

    Boolean bval;
    return mHost->mHandler->SendMessage(msg, &bal);
}

CBluetoothManagerService::BluetoothHandler::BluetoothHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CBluetoothManagerService* host)
    : mHost(host)
{
}

void CBluetoothManagerService::BluetoothHandler::HandleMessage(
    /* [in] */ IMessage * msg)
{
    Int32 what, arg1,  arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);

    if (DBG) Logger::D (TAG, "Message: " + what);
    switch (what) {
        case MESSAGE_GET_NAME_AND_ADDRESS:
            if (DBG) Logger::D(TAG,"MESSAGE_GET_NAME_AND_ADDRESS");
            {
                AutoLock lock(mHost->mConnection);
                //Start bind request
                if ((mHost->mBluetooth == NULL) && (!mHost->mBinding)) {
                    if (DBG) Logger::D(TAG, "Binding to service to get name and address");
                    mHost->mConnection->SetGetNameAddressOnly(TRUE);
                    //Start bind timeout and bind
                    this->SendMessageDelayed(MESSAGE_TIMEOUT_BIND, 0, 0, NULL, TIMEOUT_BIND_MS);
                    AutoPtr<IIntent> i;
                    CIntent::New(IBluetooth.class.getName(), (IIntent**)&i);

                    if (!DoBind(i, mConnection,
                        IContext::BIND_AUTO_CREATE | IContext::BIND_IMPORTANT, UserHandle::CURRENT)) {
                            mHandler->RemoveMessages(MESSAGE_TIMEOUT_BIND);
                    }
                    else {
                        mHost->mBinding = TRUE;
                    }
                } else {
                    if (mHost->mBluetooth != NULL) {
                        this->SendMessage(MESSAGE_SAVE_NAME_AND_ADDRESS, 0, 0, NULL);
                    } else {
                        // if enable is also called to bind the service
                        // wait for MESSAGE_BLUETOOTH_SERVICE_CONNECTED
                        this->SendMessageDelayed(MESSAGE_SAVE_NAME_AND_ADDRESS, 0, 0, NULL, TIMEOUT_SAVE_MS);
                    }
                }
            }
            break;

        case MESSAGE_SAVE_NAME_AND_ADDRESS:
            Boolean unbind = FALSE;
            if (DBG) Logger::D(TAG,"MESSAGE_SAVE_NAME_AND_ADDRESS");
            {
                AutoLock lock(mHost->mConnection);
                if (!mHost->mEnable && mHost->mBluetooth != NULL)
                    mHost->mBluetooth->Enable();
            }
            if (mHost->mBluetooth != NULL)
                mHost->WaitForOnOff(TRUE, FALSE);

            {
                AutoLock lock(mHost->mConnection);
                if (mHost->mBluetooth != NULL) {
                    String name =  NULL;
                    String address = NULL;
                    mHost->mBluetooth->GetName(&name);
                    mHost->mBluetooth->GetAddress(&address);

                    if (name != NULL && address != NULL) {
                        mHost->StoreNameAndAddress(name,address);
                        if (mHost->mConnection->IsGetNameAddressOnly()) {
                            unbind = TRUE;
                        }
                    } else {
                        if (arg1 < MAX_SAVE_RETRIES) {
                            //if (DBG) Logger::D(TAG,"Retrying name/address remote retrieval and save.....Retry count =" + retryMsg.arg1);
                            this->SendMessageDelayed(MESSAGE_SAVE_NAME_AND_ADDRESS, 1+arg1, 0, NULL, TIMEOUT_SAVE_MS);
                        } else {
                            Logger::W(TAG,"Maximum name/address remote retrieval retry exceeded");
                            if (mHost->mConnection->IsGetNameAddressOnly())
                                unbind = TRUE;
                        }
                    }

                    if (!mHost->mEnable)
                        mHost->mBluetooth->Disable();
                } else {
                    // rebind service by Request GET NAME AND ADDRESS
                    // if service is unbinded by disable or
                    // MESSAGE_BLUETOOTH_SERVICE_CONNECTED is not received
                    this->SendMessage(MESSAGE_GET_NAME_AND_ADDRESS, 0, 0, NULL);
                }
            }
            if (!mHost->mEnable && mHost->mBluetooth != NULL)
                mHost->WaitForOnOff(FALSE, TRUE);
            if (unbind)
                mHost->UnbindAndFinish();
            break;

        case MESSAGE_ENABLE:
            if (DBG) {
                Logger::D(TAG, "MESSAGE_ENABLE: mBluetooth = " + mBluetooth);
            }
            mHost->mHandler->RemoveMessages(MESSAGE_RESTART_BLUETOOTH_SERVICE);
            mHost->mEnable = TRUE;
            mHost->HandleEnable(arg1 == 1);
            break;

        case MESSAGE_DISABLE:
            mHandler->RemoveMessages(MESSAGE_RESTART_BLUETOOTH_SERVICE);
            if (mEnable && mBluetooth != NULL) {
                mHost->WaitForOnOff(TRUE, FALSE);
                 mHost->mEnable = FALSE;
                mHost->HandleDisable();
                mHost->WaitForOnOff(FALSE, FALSE);
            } else {
                 mHost->mEnable = FALSE;
                mHost->HandleDisable();
            }
            break;

        case MESSAGE_REGISTER_ADAPTER:
            IBluetoothManagerCallback callback = (IBluetoothManagerCallback) msg.obj;
            Boolean added = mCallbacks.register(callback);
            Logger::D(TAG,"Added callback: " +  (callback == NULL? "NULL": callback)  +":" +added );
            break;

        case MESSAGE_UNREGISTER_ADAPTER:
            IBluetoothManagerCallback callback = (IBluetoothManagerCallback) msg.obj;
            Boolean removed = mCallbacks.unregister(callback);
            Logger::D(TAG,"Removed callback: " +  (callback == NULL? "NULL": callback)  +":" + removed);
            break;

        case MESSAGE_REGISTER_STATE_CHANGE_CALLBACK:
            IBluetoothStateChangeCallback callback = (IBluetoothStateChangeCallback) msg.obj;
            if (callback != null) {
                mStateChangeCallbacks.register(callback);
            }
            break;

        case MESSAGE_UNREGISTER_STATE_CHANGE_CALLBACK:
            IBluetoothStateChangeCallback callback = (IBluetoothStateChangeCallback) msg.obj;
            if (callback != null) {
                mHost->mStateChangeCallbacks.unregister(callback);
            }
            break;

        case MESSAGE_BLUETOOTH_SERVICE_CONNECTED:
            if (DBG) Logger::D(TAG,"MESSAGE_BLUETOOTH_SERVICE_CONNECTED %d", arg1);

            IBinder service = (IBinder) msg.obj;
            {
                AutoLock lock(mConnection);

                if (arg1 == SERVICE_IBLUETOOTHGATT) {
                    mBluetoothGatt = IIBluetoothGatt::Probe(service);
                    break;
                } // else must be SERVICE_IBLUETOOTH

                //Remove timeout
                mHandler->RemoveMessages(MESSAGE_TIMEOUT_BIND);

                mHost->mBinding = FALSE;
                mHost->mBluetooth = IIBluetooth::Probe(service);

                try {
                    boolean enableHciSnoopLog = (Settings.Secure.getInt(mContentResolver,
                        Settings.Secure.BLUETOOTH_HCI_LOG, 0) == 1);
                    if (!mBluetooth.configHciSnoopLog(enableHciSnoopLog)) {
                        Log.e(TAG,"IBluetooth.configHciSnoopLog return FALSE");
                    }
                } catch (RemoteException e) {
                    Log.e(TAG,"Unable to call configHciSnoopLog", e);
                }

                if (mHost->mConnection->IsGetNameAddressOnly()) {
                    //Request GET NAME AND ADDRESS
                    this->SendMessage(MESSAGE_GET_NAME_AND_ADDRESS, 0, 0, NULL);
                    if (!mHost->mEnable) return;
                }

                mHost->mConnection->SetGetNameAddressOnly(FALSE);
                //Register callback object
                mHost->mBluetooth->RegisterCallback(mBluetoothCallback);
                //Inform BluetoothAdapter instances that service is up
                mHost->SendBluetoothServiceUpCallback();

                //Do enable request
                if (mHost->mQuietEnable == FALSE) {
                    if(!mHost->mBluetooth->Enable())
                        Logger::E(TAG,"IBluetooth.enable() returned FALSE");
                } else {
                    if(!mHost->mBluetooth->EnableNoAutoConnect())
                        Logger::E(TAG,"IBluetooth.enableNoAutoConnect() returned FALSE");
                }
            }

            if (!mHost->mEnable) {
                mHost->WaitForOnOff(TRUE, FALSE);
                mHost->HandleDisable();
                mHost->WaitForOnOff(FALSE, FALSE);
            }
            break;

        case MESSAGE_TIMEOUT_BIND:
            Logger::E(TAG, "MESSAGE_TIMEOUT_BIND");
            {
                AutoLock lock(mConnection);
                mHost->mBinding = FALSE;
            }
            break;

        case MESSAGE_BLUETOOTH_STATE_CHANGE:
            Int32 prevState = arg1;
            Int32 newState = arg2;
            if (DBG) Logger::D(TAG, "MESSAGE_BLUETOOTH_STATE_CHANGE: prevState = " + prevState + ", newState=" + newState);
            mHost->mState = newState;
            mHost->BluetoothStateChangeHandler(prevState, newState);

            // handle error state transition case from TURNING_ON to OFF
            // unbind and rebind bluetooth service and enable bluetooth
            if ((prevState == BluetoothAdapter.STATE_TURNING_ON) &&
                (newState == BluetoothAdapter.STATE_OFF) &&
                (mBluetooth != null) && mEnable) {
                recoverBluetoothServiceFromError();
            }
            if (newState == BluetoothAdapter.STATE_ON) {
                // bluetooth is working, reset the counter
                if (mErrorRecoveryRetryCounter != 0) {
                    Log.w(TAG, "bluetooth is recovered from error");
                    mErrorRecoveryRetryCounter = 0;
                }
            }
            break;

        case MESSAGE_BLUETOOTH_SERVICE_DISCONNECTED:
            Logger::E(TAG, "MESSAGE_BLUETOOTH_SERVICE_DISCONNECTED %d", arg1);
            {
                AutoLock lock(mConnection);
                if (arg1 == SERVICE_IBLUETOOTH) {
                    // if service is unbinded already, do nothing and return
                    if (mBluetooth == null) break;
                    mBluetooth = null;
                } else if (arg1 == SERVICE_IBLUETOOTHGATT) {
                    mBluetoothGatt = null;
                    break;
                } else {
                    Log.e(TAG, "Bad msg.arg1: " + msg.arg1);
                    break;
                }
            }

            if (mHost->mEnable) {
                mHost->mEnable = FALSE;
                // Send a Bluetooth Restart message
                this->SendMessageDelayed(MESSAGE_RESTART_BLUETOOTH_SERVICE, 0, 0, NULL,
                    SERVICE_RESTART_TIME_MS);
            }

            if (!mHost->mConnection->IsGetNameAddressOnly()) {
                mHost->SendBluetoothServiceDownCallback();

                // Send BT state broadcast to update
                // the BT icon correctly
                if ((mState == IBluetoothAdapter::STATE_TURNING_ON) ||
                    (mState == IBluetoothAdapter::STATE_ON)) {
                    BluetoothStateChangeHandler(IBluetoothAdapter::STATE_ON,
                                                IBluetoothAdapter::STATE_TURNING_OFF);
                    mState = IBluetoothAdapter::STATE_TURNING_OFF;
                }
                if (mState == IBluetoothAdapter::STATE_TURNING_OFF) {
                    BluetoothStateChangeHandler(IBluetoothAdapter::STATE_TURNING_OFF,
                                                IBluetoothAdapter::STATE_OFF);
                }

                mHandler->RemoveMessages(MESSAGE_BLUETOOTH_STATE_CHANGE);

                mHost->BluetoothStateChangeHandler(IBluetoothAdapter::STATE_ON,
                                            IBluetoothAdapter::STATE_TURNING_OFF);
                mHost->mState = IBluetoothAdapter::STATE_OFF;
            }
            break;

        case MESSAGE_RESTART_BLUETOOTH_SERVICE:
            Logger::D(TAG, "MESSAGE_RESTART_BLUETOOTH_SERVICE:"
                +" Restart IBluetooth service");
            /* Enable without persisting the setting as
             it doesnt change when IBluetooth
             service restarts */
            mHost->mEnable = TRUE;
            mHost->HandleEnable(mHost->mQuietEnable);
            break;

        case MESSAGE_TIMEOUT_UNBIND:
            Logger::E(TAG, "MESSAGE_TIMEOUT_UNBIND");
            {
                AutoLock lock(mHost->mConnection);
                mHost->mUnbinding = FALSE;
            }
            break;

        case MESSAGE_USER_SWITCHED:
            if (DBG) {
                Logger::D(TAG, "MESSAGE_USER_SWITCHED");
            }
            mHandler->RemoveMessages(MESSAGE_USER_SWITCHED);
            /* disable and enable BT when detect a user switch */
            if (mHost->mEnable && mHost->mBluetooth != NULL) {
                {
                    AutoLock lock(mHost->mConnection);
                    if (mHost->mBluetooth != NULL) {
                        //Unregister callback object
                        mHost->mBluetooth->UnregisterCallback(mBluetoothCallback);
                    }
                }

                if (mState == IBluetoothAdapter::STATE_TURNING_OFF) {
                    // MESSAGE_USER_SWITCHED happened right after MESSAGE_ENABLE
                    BluetoothStateChangeHandler(mState, IBluetoothAdapter::STATE_OFF);
                    mState = IBluetoothAdapter::STATE_OFF;
                }
                if (mState == IBluetoothAdapter::STATE_OFF) {
                    BluetoothStateChangeHandler(mState, IBluetoothAdapter::STATE_TURNING_ON);
                    mState = IBluetoothAdapter::STATE_TURNING_ON;
                }

                mHost->WaitForOnOff(TRUE, FALSE);

                if (mState == IBluetoothAdapter::STATE_TURNING_ON) {
                    mHost->BluetoothStateChangeHandler(mHost->mState, IBluetoothAdapter::STATE_ON);
                }

                // disable
                mHost->HandleDisable();

                // Pbap service need receive STATE_TURNING_OFF intent to close
                mHost->BluetoothStateChangeHandler(
                    IBluetoothAdapter::STATE_ON, IBluetoothAdapter::STATE_TURNING_OFF);

                mHost->WaitForOnOff(FALSE, TRUE);

                mHost->BluetoothStateChangeHandler(
                    IBluetoothAdapter::STATE_TURNING_OFF, IBluetoothAdapter::STATE_OFF);

                mHost->SendBluetoothServiceDownCallback();
                {
                    AutoLock lock(mConnection);
                    if (mHost->mBluetooth != NULL) {
                        mHost->mBluetooth = NULL;
                        //Unbind
                        mHost->mContext->UnbindService(mConnection);
                    }
                }
                SystemClock.sleep(100);

                // enable
                mHandler->RemoveMessages(MESSAGE_BLUETOOTH_STATE_CHANGE);
                mState = IBluetoothAdapter::STATE_OFF;
                // enable
                mHost->HandleEnable(mQuietEnable);
            }
            else if (mHost->mBinding || mHost->mBluetooth != NULL) {
                // if user is switched when service is being binding
                // delay sending MESSAGE_USER_SWITCHED
                this->SendMessageDelayed(MESSAGE_USER_SWITCHED, 0, 1+arg2, NULL, USER_SWITCHED_TIME_MS);
                if (DBG)
                    Logger::D(TAG, "delay MESSAGE_USER_SWITCHED " + userMsg.arg2);
            }
            break;

    }
}

CBluetoothManagerService::MyRunnable::MyRunnable(
    /* [in] */ CBluetoothManagerService* host)
    : mHost(host)
{ }

ECode CBluetoothManagerService::MyRunnable::Run()
{
    mHost->ReleaseWakeLock();
    return NOERROR;
}

CAR_INTERFACE_IMPL(CBluetoothManagerService::MyBluetoothCallback, Object, IIBluetoothCallback)

CBluetoothManagerService::MyBluetoothCallback::MyBluetoothCallback(
    /* [in] */ CBluetoothManagerService* host)
    : mHost(host)
{ }

ECode CBluetoothManagerService::MyBluetoothCallback::OnBluetoothStateChange(
    /* [in] */ Int32 prevState,
    /* [in] */ Int32 newState)
{
    mHost->mHandler->SendMessage(MESSAGE_BLUETOOTH_STATE_CHANGE, prevState, newState, NULL);
    return NOERROR;
}

CBluetoothManagerService::MyRunnableEx::MyRunnableEx(
    /* [in] */ CBluetoothManagerService* host)
    : mHost(host)
{ }

ECode CBluetoothManagerService::MyRunnableEx::Run()
{
    if (mHost->mState == IBluetoothAdapter::STATE_ON) {
        if (DBG) Logger::D(TAG, "10s is passed , get rx count again");
        mHost->mHandler->SendMessage(MESSAGE_GET_RX_COUNT, 0, 0, NULL);
        mHost->mDelayedHandler.postDelayed(this, 10 * 1000);
    }
    return NOERROR;
}

CBluetoothManagerService::BTtimerRunnable::BTtimerRunnable(
    /* [in] */ CBluetoothManagerService* host)
    : mHost(host)
{ }

ECode CBluetoothManagerService::BTtimerRunnable::Run()
{
    if (mHost->mState == IBluetoothAdapter::STATE_ON) {
        Boolean bval;
        if (DBG) Logger::D(TAG, "10s is passed , get rx count again");
        mHost->mHandler->SendMessage(MESSAGE_GET_RX_COUNT, 0, 0, NULL, &bval);
        mHost->mDelayedHandler->PostDelayed(this, 10 * 1000, &bval);
    }
    return NOERROR;
}

CBluetoothManagerService::MyBroadcastReceiver::MyBroadcastReceiver(
    /* [in] */ CBluetoothManagerService* host)
    : mHost(host)
{ }

ECode CBluetoothManagerService::MyBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);

    if (IBluetoothAdapter::ACTION_LOCAL_NAME_CHANGED.Equals(action)) {
        String newName = intent.getStringExtra(IBluetoothAdapter::EXTRA_LOCAL_NAME);
        if (DBG) Logger::D(TAG, "Bluetooth Adapter name changed to " + newName);
        if (newName != NULL) {
            mHost->StoreNameAndAddress(newName, NULL);
        }
    }
    else if (IIntent::ACTION_AIRPLANE_MODE_CHANGED.Equals(action)) {
        AutoLock lock(mReceiver);
        if (mHost->IsBluetoothPersistedStateOn()) {
            if (mHost->IsAirplaneModeOn()) {
                mHost->PersistBluetoothSetting(BLUETOOTH_ON_AIRPLANE);
            } else {
                mHost->PersistBluetoothSetting(BLUETOOTH_ON_BLUETOOTH);
            }
        }
        if (mHost->IsAirplaneModeOn()) {
            // disable without persisting the setting
            mHost->SendDisableMsg();
        } else if (mEnableExternal) {
            // enable without persisting the setting
            mHost->SendEnableMsg(mQuietEnableExternal);
        }
    }
    else if (IIntent::ACTION_USER_SWITCHED.Equals(action)) {
        Int32 num;
        intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, 0, &num)
        mHost->mHandler->SendMessage(MESSAGE_USER_SWITCHED, num, 0, NULL);
    }
    else if (IIntent::ACTION_BOOT_COMPLETED.Equals(action)) {
        {
            AutoLock lock(mReceiver);
            if (mHost->mEnableExternal && mHost->IsBluetoothPersistedStateOnBluetooth()) {
                //Enable
                // if (DBG) Logger::D(TAG, "Auto-enabling Bluetooth.");
                mHost->SendEnableMsg(mQuietEnableExternal);
            }
        }

        if (!mHost->IsNameAndAddressSet()) {
            //Sync the Bluetooth name and address from the Bluetooth Adapter
            // if (DBG) Logger::D(TAG,"Retrieving Bluetooth Adapter name and address...");
            mHost->GetNameAndAddress();
        }
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(CBluetoothManagerService, Object, IIBluetoothManager, IBinder)

CAR_OBJECT_IMPL(CBluetoothManagerService)

CBluetoothManagerService::CBluetoothManagerService()
    : mBinding(FALSE)
    , mUnbinding(FALSE)
    , mQuietEnable(FALSE)
    , mQuietEnableExternal(FALSE)
    , mEnableExternal(FALSE)
    , mEnable(FALSE)
    , mState(0)
    , mErrorRecoveryRetryCounter(0)
    , mSystemUiUid(0)
{}

void CBluetoothManagerService::RegisterForAirplaneMode(
    /* [in] */ IIntentFilter* filter)
{
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    String airplaneModeRadios;
    Settings::Global::GetString(resolver, Settings::Global::AIRPLANE_MODE_RADIOS, &airplaneModeRadios);
    String toggleableRadios;
    Settings::Global::GetString(resolver, Settings::Global::AIRPLANE_MODE_TOGGLEABLE_RADIOS, &toggleableRadios);
    Boolean mIsAirplaneSensitive = airplaneModeRadios == NULL ? TRUE :
            airplaneModeRadios.Contains(Settings::Global::RADIO_BLUETOOTH);
    if (mIsAirplaneSensitive) {
        filter->AddAction(IIntent::ACTION_AIRPLANE_MODE_CHANGED);
    }
}

ECode CBluetoothManagerService::constructor(
    /* [in] */ IContext* context)
{
    AutoPtr<ILooper> looper;
    IoThread::Get()->GetLooper((ILooper**)&looper);
    mHandler = new BluetoothHandler(looper, this);


    mContext = context;
    mBluetooth = NULL;
    mBinding = FALSE;
    mUnbinding = FALSE;
    mEnable = FALSE;
    mState = IBluetoothAdapter::STATE_OFF;
    mQuietEnableExternal = FALSE;
    mEnableExternal = FALSE;
    mAddress = NULL;
    mName = NULL;
    context->GetContentResolver((IContentResolver**)&mContentResolver);
    mCallbacks = new RemoteCallbackList<IBluetoothManagerCallback>();
    mStateChangeCallbacks = new RemoteCallbackList<IBluetoothStateChangeCallback>();
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(IIntent::ACTION_BOOT_COMPLETE, (IIntentFilter**)&filter);
    filter->AddAction(IBluetoothAdapter::ACTION_LOCAL_NAME_CHANGED);
    filter->AddAction(IIntent::ACTION_USER_SWITCHED);
    RegisterForAirplaneMode(filter);
    filter->SetPriority(IIntentFilter::SYSTEM_HIGH_PRIORITY);
    mContext->RegisterReceiver(mReceiver, filter);
    Boolean airplaneModeOn = IsAirplaneModeOn();
    Boolean bluetoothOn = IsBluetoothPersistedStateOn();
    LoadStoredNameAndAddress();

    String value;
    if (IsBluetoothPersistedStateOn()) {
        mEnableExternal = true;
    }

    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    Int32 sysUiUid = -1;
    ECode ec = pm->GetPackageUid(String("com.android.systemui"), UserHandle::USER_OWNER, &sysUiUid);
    if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        Logger::E(TAG, "Unable to resolve SystemUI's UID.");
    }
    mSystemUiUid = sysUiUid;

    return NOERROR;
}

/**
 *  Returns TRUE if airplane mode is currently on
 */
Boolean CBluetoothManagerService::IsAirplaneModeOn()
{
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Int32 val;
    Settings::Global::GetInt32(resolver, Settings::Global::AIRPLANE_MODE_ON, 0, &val);
    return (val == 1);
}

/**
 *  Returns TRUE if the Bluetooth saved state is "on"
 */
Boolean CBluetoothManagerService::IsBluetoothPersistedStateOn()
{
    Int32 val;
    Settings::Global::GetInt32(mContentResolver, Settings::Global::BLUETOOTH_ON, 0, &val);
    return (val == BLUETOOTH_OFF);
}

/**
 *  Returns true if the Bluetooth saved state is BLUETOOTH_ON_BLUETOOTH
 */
Boolean CBluetoothManagerService::IsBluetoothPersistedStateOn()
{
    Int32 val;
    Settings::Global::GetInt32(mContentResolver, Settings::Global::BLUETOOTH_ON, 0, &val);
    return (val == BLUETOOTH_ON_BLUETOOTH);
}

/**
 *  Save the Bluetooth on/off state
 *
 */
void CBluetoothManagerService::PersistBluetoothSetting(
    /* [in] */ Int32 setOn)
{
    Boolean bol;
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Settings::Global::PutInt32(resolver, Settings::Global::BLUETOOTH_ON, setOn, &bol);
}

/**
 * Returns TRUE if the Bluetooth Adapter's name and address is
 * locally cached
 * @return
 */
Boolean CBluetoothManagerService::IsNameAndAddressSet()
{
    return mName !=NULL && mAddress!= NULL && mName.GetLength()>0 && mAddress.GetLength()>0;
}

/**
 * Retrieve the Bluetooth Adapter's name and address and save it in
 * in the local cache
 */
void CBluetoothManagerService::LoadStoredNameAndAddress()
{
    if (DBG) Logger::D(TAG, "Loading stored name and address");
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    Boolean bol;
    res->GetBoolean(R::bool_::config_bluetooth_address_validation, &bol);
    Int32 val;
    Settings::Secure::GetInt32(mContentResolver, SECURE_SETTINGS_BLUETOOTH_ADDR_VALID, 0, &val)
    if (bol && (val == 0)) {
        // if the valid flag is not set, don't load the address and name
        if (DBG) Logger::D(TAG, "invalid bluetooth name and address stored");
        return;
    }
    Settings::Secure::GetString(mContentResolver, SECURE_SETTINGS_BLUETOOTH_NAME, &mName);
    Settings::Secure::GetString(mContentResolver, SECURE_SETTINGS_BLUETOOTH_ADDRESS, &mAddress);
    if (DBG) Logger::D(TAG, "Stored bluetooth Name=" + mName + ",Address=" + mAddress);
}

/**
 * Save the Bluetooth name and address in the persistent store.
 * Only non-NULL values will be saved.
 * @param name
 * @param address
 */
void CBluetoothManagerService::StoreNameAndAddress(
    /* [in] */ const String& name,
    /* [in] */ const String& address)
{
    Boolean bol;
    String str;

    if (name != NULL) {
        Settings::Secure::PutString(mContentResolver, SECURE_SETTINGS_BLUETOOTH_NAME, name, &bol);
        mName = name;
        Settings::Secure::GetString(mContentResolver,SECURE_SETTINGS_BLUETOOTH_NAME, &str);
        if (DBG)
            Logger::D(TAG,"Stored Bluetooth name: " + str);
    }

    if (address != NULL) {
        Settings::Secure::PutString(mContentResolver, SECURE_SETTINGS_BLUETOOTH_ADDRESS, address, &bol);
        mAddress=address;
        Settings::Secure::GetString(mContentResolver,SECURE_SETTINGS_BLUETOOTH_ADDRESS, &str)
        if (DBG)  Logger::D(TAG,"Stored Bluetoothaddress: " + str);
    }

    if ((name != NULL) && (address != NULL)) {
        Settings::Secure::PutInt32(mContentResolver, SECURE_SETTINGS_BLUETOOTH_ADDR_VALID, 1, &bol);
    }
}

ECode CBluetoothManagerService::RegisterAdapter(
    /* [in] */ IIBluetoothManagerCallback* callback,
    /* [out] */ IIBluetooth** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (callback == NULL) {
        Logger::W(TAG, "Callback is null in RegisterAdapter");
        return NOERROR;
    }

    mHandler->SendMessage(MESSAGE_REGISTER_ADAPTER, 0, 0, callback);
    {
        AutoLock lock(mConnection);
        *result = mBluetooth;
        REFCOUNT_ADD(*result);
    }
    return NOERROR;
}

ECode CBluetoothManagerService::UnregisterAdapter(
    /* [in] */ IIBluetoothManagerCallback* callback)
{
    if (callback == NULL) {
        Logger::W(TAG, "Callback is null in UnregisterAdapter");
        return NOERROR;
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(BLUETOOTH_PERM,
                                            "Need BLUETOOTH permission"));
    mHandler->SendMessage(MESSAGE_UNREGISTER_ADAPTER, 0, 0, callback);
    return NOERROR;
}

ECode CBluetoothManagerService::RegisterStateChangeCallback(
    /* [in] */ IIBluetoothStateChangeCallback* callback)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(BLUETOOTH_PERM,
                                            "Need BLUETOOTH permission"));
    mHandler->SendMessage(MESSAGE_REGISTER_STATE_CHANGE_CALLBACK, 0, 0, callback);
    return NOERROR;
}

ECode CBluetoothManagerService::UnregisterStateChangeCallback(
    /* [in] */ IIBluetoothStateChangeCallback* callback)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(BLUETOOTH_PERM,
                                            "Need BLUETOOTH permission"));
    mHandler->SendMessage(MESSAGE_UNREGISTER_STATE_CHANGE_CALLBACK, 0, 0, callback);
    return NOERROR;
}

ECode CBluetoothManagerService::IsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if ((Binder::GetCallingUid() != IProcess::SYSTEM_UID) &&
        !CheckIfCallerIsForegroundUser()) {
        Logger::W(TAG,"isEnabled(): not allowed for non-active and non system user");
        return NOERROR;
    }

    {
        AutoLock lock(mConnection);
        Boolean bol;
        mBluetooth->IsEnabled(&bol);
        *result = (mBluetooth != NULL && bol);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CBluetoothManagerService::GetNameAndAddress()
{
    if (DBG) {
//        Logger::D(TAG,"getNameAndAddress(): mBluetooth = " + mBluetooth +
//              " mBinding = " + mBinding);
    }
    mHandler->SendMessage(MESSAGE_GET_NAME_AND_ADDRESS, 0, 0, NULL);
    return NOERROR;
}

ECode CBluetoothManagerService::EnableNoAutoConnect(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(BLUETOOTH_ADMIN_PERM,
                                            "Need BLUETOOTH ADMIN permission"));

    if (DBG) {
//        Logger::D(TAG,"enableNoAutoConnect():  mBluetooth =" + mBluetooth +
//                " mBinding = " + mBinding);
    }

    Int32 callingAppId = UserHandle::GetAppId(Binder::GetCallingUid());
    if (callingAppId != IProcess::NFC_UID) {
        // throw new SecurityException("no permission to enable Bluetooth quietly");
        return E_SECURITY_EXCEPTION;
    }

    {
        AutoLock lock(mReceiver);
        mQuietEnableExternal = TRUE;
        mEnableExternal = TRUE;
        SendEnableMsg(TRUE);
    }

    *result = TRUE;
    return NOERROR;

}

ECode CBluetoothManagerService::Enable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if ((Binder::GetCallingUid() != IProcess::SYSTEM_UID) &&
        (!CheckIfCallerIsForegroundUser())) {
        Logger::W(TAG,"enable(): not allowed for non-active and non system user");
        *result = FALSE;
        return NOERROR;
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        BLUETOOTH_ADMIN_PERM,
        String("Need BLUETOOTH ADMIN permission")));
    // if (DBG) {
    //     Logger::D(TAG,"enable():  mBluetooth =" + mBluetooth +
    //             " mBinding = " + mBinding);
    // }

    {
        AutoLock lock(mReceiver);
        mQuietEnableExternal = FALSE;
        mEnableExternal = TRUE;
        // waive WRITE_SECURE_SETTINGS permission check
        Int64 callingIdentity = Binder::ClearCallingIdentity();
        PersistBluetoothSetting(BLUETOOTH_ON_BLUETOOTH);
        Binder::RestoreCallingIdentity(callingIdentity);
        SendEnableMsg(FALSE);
    }

    *result = FALSE;
    return NOERROR;
}

ECode CBluetoothManagerService::Disable(
    /* [in] */ Boolean persist,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(BLUETOOTH_ADMIN_PERM,
                                            "Need BLUETOOTH ADMIN permissicacheNameAndAddresson"));

    if ((Binder::GetCallingUid() != IProcess::SYSTEM_UID) &&
        (!CheckIfCallerIsForegroundUser())) {
        Logger::W(TAG,"disable(): not allowed for non-active and non system user");
        *result = FALSE;
        return NOERROR;
    }

    // if (DBG) {
    //     Logger::D(TAG,"disable(): mBluetooth = " + mBluetooth +
    //         " mBinding = " + mBinding);
    // }

    {
        AutoLock lock(mReceiver);
        if (persist) {
            // waive WRITE_SECURE_SETTINGS permission check
            Int64 callingIdentity = Binder::ClearCallingIdentity();
            PersistBluetoothSetting(BLUETOOTH_OFF);
            Binder::RestoreCallingIdentity(callingIdentity);
        }
        mEnableExternal = FALSE;
        SendDisableMsg();
    }

    *result = TRUE;
    return NOERROR;
}

ECode CBluetoothManagerService::UnbindAndFinish()
{
    if (DBG) {
//        Logger::D(TAG,"UnbindAndFinish(): " + mBluetooth +
//            " mBinding = " + mBinding);
    }

    {
        AutoLock lock(mConnection);
        if (mUnbinding)
            return NOERROR;

        mUnbinding = TRUE;
        if (mBluetooth != NULL) {
            if (!mConnection->IsGetNameAddressOnly()) {
                //Unregister callback object
                mBluetooth->UnregisterCallback(mBluetoothCallback);
            }
            if (DBG) Logger::D(TAG, "Sending unbind request.");
            mBluetooth = NULL;
            //Unbind
            mContext->UnbindService(mConnection);
            mUnbinding = FALSE;
            mBinding = FALSE;
        } else {
            mUnbinding=FALSE;
        }
    }
    return NOERROR;
}

void CBluetoothManagerService::SendBluetoothStateCallback(
    /* [in] */ Boolean isUp)
{
    Int32 n;
    mStateChangeCallbacks->BeginBroadcast(&n);
    if (DBG) Logger::D(TAG,"Broadcasting onBluetoothStateChange("+isUp+") to " + n + " receivers.");
    for (Int32 i = 0; i < n; i++) {
         mStateChangeCallbacks->GetBroadcastItem(i).onBluetoothStateChange(isUp);
    }
    mStateChangeCallbacks->FinishBroadcast();
}

/**
 * Inform BluetoothAdapter instances that Adapter service is up
 */
void CBluetoothManagerService::SendBluetoothServiceUpCallback()
{
    if (!mConnection->IsGetNameAddressOnly()) {
        if (DBG) Logger::D(TAG,"Calling onBluetoothServiceUp callbacks");
        Int32 n;
        mCallbacks->BeginBroadcast(&n);
        Logger::D(TAG,"Broadcasting onBluetoothServiceUp() to " + n + " receivers.");
        for (Int32 i = 0; i < n; i++) {
            mCallbacks->GetBroadcastItem(i).onBluetoothServiceUp(mBluetooth);
        }
        mCallbacks->FinishBroadcast();
    }
}

ECode CBluetoothManagerService::GetBluetoothGatt(
    /* [out] */ IIBluetoothGatt** bg)
{
    VALIDATE_NOT_NULL(bg)
    // sync protection
    *bg = mBluetoothGatt;
    REFCOUNT_ADD(*bg)
    return NOERROR;
}

/**
 * Inform BluetoothAdapter instances that Adapter service is down
 */
void CBluetoothManagerService::SendBluetoothServiceDownCallback()
{
    if (!mConnection->IsGetNameAddressOnly()) {
        if (DBG) Logger::D(TAG,"Calling onBluetoothServiceDown callbacks");
        Int32 n;
        mCallbacks->BeginBroadcast(&n);
        Logger::D(TAG,"Broadcasting onBluetoothServiceDown() to " + n + " receivers.");
        for (Int32 i = 0; i < n; i++) {
             mCallbacks->GetBroadcastItem(i).onBluetoothServiceDown();
        }
        mCallbacks->FinishBroadcast();
    }
}
ECode CBluetoothManagerService::GetAddress(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        BLUETOOTH_PERM, String("Need BLUETOOTH permission")));

    if ((Binder::GetCallingUid() != IProcess::SYSTEM_UID) &&
            (!CheckIfCallerIsForegroundUser())) {
        Logger::W(TAG,"getAddress(): not allowed for non-active user");
        return NOERROR;
    }

    {
        AutoLock lock(mConnection);
        if (mBluetooth != NULL) {
             return mBluetooth->GetAddress(result);
        }
    }
    // mAddress is accessed from outside.
    // It is alright without a lock. Here, bluetooth is off, no other thread is
    // changing mAddress
    *result = mAddress;
    return NOERROR;
}

ECode CBluetoothManagerService::GetName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        BLUETOOTH_PERM, String("Need BLUETOOTH permission")));

    if ((Binder::GetCallingUid() != IProcess::SYSTEM_UID) &&
            (!CheckIfCallerIsForegroundUser())) {
        Logger::W(TAG,"getName(): not allowed for non-active user");
        return NOERROR;
    }

    {
        AutoLock lock(mConnection);
        if (mBluetooth != NULL) {
            return mBluetooth->GetName(result);
        }
    }
    // mName is accessed from outside.
    // It alright without a lock. Here, bluetooth is off, no other thread is
    // changing mName
    *result = mName;
    return NOERROR;
}

void CBluetoothManagerService::HandleEnable(
    /* [in] */ Boolean quietMode)
{
    mQuietEnable = quietMode;
    {
        AutoLock lock(mConnection);
        if ((mBluetooth == NULL) && (!mBinding)) {
            //Start bind timeout and bind
            mHandler->SendMessageDelayed(MESSAGE_TIMEOUT_BIND, 0, 0, NULL, TIMEOUT_BIND_MS);
            mConnection->SetGetNameAddressOnly(FALSE);
            AutoPtr<IIntent> i;
            CIntent::New(IBluetooth.class.getName(), (IIntent**)&i);
            if (!DoBind(i, mConnection, IContext::BIND_AUTO_CREATE | IContext::BIND_IMPORTANT,
                    UserHandle::CURRENT)) {
                mHandler->RemoveMessages(MESSAGE_TIMEOUT_BIND);
            }
            else {
                mBinding = TRUE;
            }
        }
        else if (mBluetooth != NULL) {
            if (mConnection->IsGetNameAddressOnly()) {
                // if GetNameAddressOnly is set, we can clear this flag,
                // so the service won't be unbind
                // after name and address are saved
                mConnection->SetGetNameAddressOnly(FALSE);
                //Register callback object
                mBluetooth->RegisterCallback(mBluetoothCallback);
                //Inform BluetoothAdapter instances that service is up
                SendBluetoothServiceUpCallback();
            }
            //Enable bluetooth

            Boolean bol;
            if (!mQuietEnable) {
                mBluetooth->Enable(&bol);
                if(!bol)
                    Logger::E(TAG,"IBluetooth.enable() returned FALSE");
            } else {
                mBluetooth->EnableNoAutoConnect(&bol);
                if(!bol)
                    Logger::E(TAG,"IBluetooth.enableNoAutoConnect() returned FALSE");
            }
        }
    }
}

Boolean CBluetoothManagerService::DoBind(
    /* [in] */ IIntent* intent,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user)
{
    ComponentName comp = intent.resolveSystemService(mContext.getPackageManager(), 0);
    intent.setComponent(comp);
    if (comp == null || !mContext.bindServiceAsUser(intent, conn, flags, user)) {
        Log.e(TAG, "Fail to bind to: " + intent);
        return FALSE;
    }
    return true;
}

void CBluetoothManagerService::HandleDisable()
{
    {
        AutoLock lock(mConnection);
        // don't need to disable if GetNameAddressOnly is set,
        // service will be unbinded after Name and Address are saved
        if ((mBluetooth != NULL) && (!mConnection->IsGetNameAddressOnly())) {
            if (DBG)
                Logger::D(TAG,"Sending off request.");
            Boolean bol;
            mBluetooth->Disable(&bol);
            if(!bol)
                Logger::E(TAG,"IBluetooth.disable() returned FALSE");
        }
    }
}

Boolean CBluetoothManagerService::CheckIfCallerIsForegroundUser()
{
    Int32 foregroundUser;
    Int32 callingUser = UserHandle::GetCallingUserId();
    Int32 callingUid = Binder::GetCallingUid();
    Int64 callingIdentity = Binder::ClearCallingIdentity();
    Int32 callingAppId = UserHandle::GetAppId(callingUid);
    Boolean valid = FALSE;
//    try {
        foregroundUser = ActivityManager::GetCurrentUser();
        valid = (callingUser == foregroundUser) ||
                callingAppId == IProcess::NFC_UID ||
                callingAppId == mSystemUiUid;
        if (DBG) {
//            Logger::D(TAG, "checkIfCallerIsForegroundUser: valid=" + valid
//                + " callingUser=" + callingUser
//                + " foregroundUser=" + foregroundUser);
        }
//    } finally {
    Binder::RestoreCallingIdentity(callingIdentity);
//    }
    return valid;
}

void CBluetoothManagerService::BluetoothStateChangeHandler(
    /* [in] */ Int32 prevState,
    /* [in] */ Int32 newState)
{
    if (prevState != newState) {
        //Notify all proxy objects first of adapter state change
        if (newState == IBluetoothAdapter::STATE_ON || newState == IBluetoothAdapter::STATE_OFF) {
            Boolean isUp = (newState==IBluetoothAdapter::STATE_ON);
            SendBluetoothStateCallback(isUp);

            if (isUp) {
                // connect to GattService
                if (mContext.getPackageManager().hasSystemFeature(
                                                 PackageManager.FEATURE_BLUETOOTH_LE)) {
                    Intent i = new Intent(IBluetoothGatt.class.getName());
                    doBind(i, mConnection, Context.BIND_AUTO_CREATE | Context.BIND_IMPORTANT,
                            UserHandle.CURRENT);
                }
            }
            else {
                //If Bluetooth is off, send service down event to proxy objects, and unbind
                if (!isUp && CanUnbindBluetoothService()) {
                    SendBluetoothServiceDownCallback();
                    UnbindAndFinish();
                }
            }
        }

        //Send broadcast message to everyone else
        AutoPtr<IIntent> intent;
        CIntent::New(IBluetoothAdapter::ACTION_STATE_CHANGED, (IIntent**)&intent);
        intent->PutExtra(IBluetoothAdapter::EXTRA_PREVIOUS_STATE, prevState);
        intent->PutExtra(IBluetoothAdapter::EXTRA_STATE, newState);
        intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
        if (DBG)
            Logger::D(TAG,"Bluetooth State Change Intent: " + prevState + " -> " + newState);
        mContext->SendBroadcastAsUser(intent, UserHandle::ALL, BLUETOOTH_PERM);
    }
}

/**
 *  if on is TRUE, wait for state become ON
 *  if off is TRUE, wait for state become OFF
 *  if both on and off are FALSE, wait for state not ON
 */
Boolean CBluetoothManagerService::WaitForOnOff(
    /* [in] */ Boolean on,
    /* [in] */ Boolean off)
{
    Int32 i = 0;
    while (i < 10) {
        {
            AutoLock lock(mConnection);
                if (mBluetooth == NULL)
                    break;
                Int32 val;
                mBluetooth->GetState(&val);
                if (on) {
                    if (val == IBluetoothAdapter::STATE_ON) return TRUE;
                } else if (off) {
                    if (val == IBluetoothAdapter::STATE_OFF) return TRUE;
                } else {
                    if (val != IBluetoothAdapter::STATE_ON) return TRUE;
                }
        }

        if (on || off) {
            SystemClock::Sleep(300);
        } else {
            SystemClock::Sleep(50);
        }
        i++;
    }
    Logger::E(TAG,"WaitForOnOff time out");
    return FALSE;
}

void CBluetoothManagerService::SendDisableMsg()
{
    mHandler->SendMessage(mHandler.obtainMessage(MESSAGE_DISABLE));
}

void CBluetoothManagerService::SendEnableMsg(
    /* [in] */ Boolean quietMode)
{
    mHandler->SendMessage(mHandler.obtainMessage(MESSAGE_ENABLE,
                         quietMode ? 1 : 0, 0));
}

Boolean CBluetoothManagerService::CanUnbindBluetoothService()
{
    assert(0 && "TODO");
    // synchronized(mConnection) {
    //     //Only unbind with mEnable flag not set
    //     //For race condition: disable and enable back-to-back
    //     //Avoid unbind right after enable due to callback from disable
    //     //Only unbind with Bluetooth at OFF state
    //     //Only unbind without any MESSAGE_BLUETOOTH_STATE_CHANGE message
    //     try {
    //         if (mEnable || (mBluetooth == null)) return FALSE;
    //         if (mHandler.hasMessages(MESSAGE_BLUETOOTH_STATE_CHANGE)) return FALSE;
    //         return (mBluetooth.getState() == BluetoothAdapter.STATE_OFF);
    //     } catch (RemoteException e) {
    //         Log.e(TAG, "getState()", e);
    //     }
    // }
    return FALSE;
}

void CBluetoothManagerService::RecoverBluetoothServiceFromError()
{
    Log.e(TAG,"recoverBluetoothServiceFromError");
    synchronized (mConnection) {
        if (mBluetooth != null) {
            //Unregister callback object
            try {
                mBluetooth.unregisterCallback(mBluetoothCallback);
            } catch (RemoteException re) {
                Log.e(TAG, "Unable to unregister",re);
            }
        }
    }

    SystemClock.sleep(500);

    // disable
    HandleDisable();

    WaitForOnOff(FALSE, true);

    SendBluetoothServiceDownCallback();
    synchronized (mConnection) {
        if (mBluetooth != null) {
            mBluetooth = null;
            //Unbind
            mContext.unbindService(mConnection);
        }
    }

    mHandler->RemoveMessages(MESSAGE_BLUETOOTH_STATE_CHANGE);
    mState = BluetoothAdapter.STATE_OFF;

    mEnable = FALSE;

    if (mErrorRecoveryRetryCounter++ < MAX_ERROR_RESTART_RETRIES) {
        // Send a Bluetooth Restart message to reenable bluetooth
        Message restartMsg = mHandler.obtainMessage(
                         MESSAGE_RESTART_BLUETOOTH_SERVICE);
        mHandler->SendMessageDelayed(restartMsg, ERROR_RESTART_TIME_MS);
    } else {
        // todo: notify user to power down and power up phone to make bluetooth work.
    }
}


ECode CBluetoothManagerService::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder sb("CBluetoothManagerService:")
    sb += "name:";
    sb += mName.string();
    *str = sb.ToString();
    return NOERROR;
}

} // namespace Server
} // namespace Droid
} // namespace Elastos
