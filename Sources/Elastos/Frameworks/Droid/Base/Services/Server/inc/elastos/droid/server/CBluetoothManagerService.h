
#ifndef __ELASTOS_DROID_SERVER_CBLUETOOTHMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_CBLUETOOTHMANAGERSERVICE_H__

#include "_Elastos_Droid_Server_CBluetoothManagerService.h"
#include <elastos/droid/content/BroadcastReceiver.h>
#include <elastos/core/Thread.h>
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/Runnable.h"
#include <Elastos.Droid.Bluetooth.h>
#include <Elastos.Droid.Os.h>

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IRemoteCallbackList;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Bluetooth::IIBluetooth;
using Elastos::Droid::Bluetooth::IIBluetoothGatt;
using Elastos::Droid::Bluetooth::IIBluetoothCallback;
using Elastos::Droid::Bluetooth::IIBluetoothManager;
using Elastos::Droid::Bluetooth::IIBluetoothManagerCallback;
using Elastos::Droid::Bluetooth::IIBluetoothStateChangeCallback;
using Elastos::Core::IThread;
using Elastos::Core::Thread;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CBluetoothManagerService)
    , public Object
    , public IIBluetoothManager
    , public IBinder
{
private:
    class BluetoothServiceConnection
        : public Object
        , public IServiceConnection
    {
    public:
        CAR_INTERFACE_DECL()

        BluetoothServiceConnection(
            /* [in] */ CBluetoothManagerService* host);

        void SetGetNameAddressOnly(
            /* [in] */ Boolean getOnly);

        Boolean IsGetNameAddressOnly();

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* className,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* className);

    private:
        CBluetoothManagerService* mHost;
        Boolean mGetNameAddressOnly;
    };

    class BluetoothHandler
        : public Handler
    {
    friend class CBluetoothManagerService;
    public:
        BluetoothHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CBluetoothManagerService* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CBluetoothManagerService* mHost;
         AutoPtr<IHandler> mHandler;
    };

    class MyRunnable
        : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ CBluetoothManagerService* host);

        CARAPI Run();

    private:
        CBluetoothManagerService* mHost;
    };

    class MyBluetoothCallback
        : public Object
        , public IIBluetoothCallback
    {
    public:
        CAR_INTERFACE_DECL()

        MyBluetoothCallback(
            /* [in] */ CBluetoothManagerService* host);

        CARAPI OnBluetoothStateChange();

    private:
        CBluetoothManagerService* mHost;
    };

    class MyRunnableEx
        : public Runnable
    {
    public:
        MyRunnableEx(
            /* [in] */ CBluetoothManagerService* host);

        CARAPI Run();

    private:
        CBluetoothManagerService* mHost;
    };

    class MyBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        MyBroadcastReceiver(
            /* [in] */ CBluetoothManagerService* host);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CBluetoothManagerService::MyBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CBluetoothManagerService* mHost;
    };

    class BTtimerRunnable
        : public Runnable
    {
    public:
        BTtimerRunnable(
            /* [in] */ CBluetoothManagerService* host);

        virtual CARAPI Run();

    private:
        CBluetoothManagerService* mHost;
    };
public:

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CBluetoothManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI RegisterAdapter(
        /* [in] */ IIBluetoothManagerCallback* callback,
        /* [out] */ IIBluetooth** result);

    CARAPI UnregisterAdapter(
        /* [in] */ IIBluetoothManagerCallback* callback);

    CARAPI RegisterStateChangeCallback(
        /* [in] */ IIBluetoothStateChangeCallback* callback);

    CARAPI UnregisterStateChangeCallback(
        /* [in] */ IIBluetoothStateChangeCallback* callback);

    CARAPI IsEnabled(
        /* [out] */ Boolean* result);

    CARAPI GetNameAndAddress();

    CARAPI EnableNoAutoConnect(
        /* [out] */ Boolean* result);

    CARAPI Enable(
        /* [out] */ Boolean* result);

    CARAPI Disable(
        /* [in] */ Boolean persist,
        /* [out] */ Boolean* result);

    CARAPI UnbindAndFinish();

    CARAPI GetAddress(
        /* [out] */ String* result);

    CARAPI GetName(
        /* [out] */ String* result);

    CARAPI GetBluetoothGatt(
        /* [out] */ IIBluetoothGatt** bg);

    CARAPI ToString(
        /* [out] */ String* str);
private:
    CARAPI_(void) RegisterForAirplaneMode(
        /* [in] */ IIntentFilter* filter);

    CARAPI_(Boolean) IsAirplaneModeOn();

    CARAPI_(Boolean) IsBluetoothPersistedStateOn();

    CARAPI_(Boolean) IsBluetoothPersistedStateOnBluetooth();

    CARAPI_(void) PersistBluetoothSetting(
        /* [in] */ Int32 setOn);

    CARAPI_(Boolean) IsNameAndAddressSet();

    CARAPI_(void) LoadStoredNameAndAddress();

    CARAPI_(void) StoreNameAndAddress(
        /* [in] */ const String& name,
        /* [in] */ const String& address);

    CARAPI_(void) SendBluetoothStateCallback(
        /* [in] */ Boolean isUp);

    CARAPI_(void) SendBluetoothServiceUpCallback();

    CARAPI_(void) SendBluetoothServiceDownCallback();

    CARAPI_(void) HandleEnable(
        /* [in] */ Boolean quietMode);

    CARAPI_(Boolean) DoBind(
        /* [in] */ IIntent* intent,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [in] */ IUserHandle* user);

    CARAPI_(void) HandleDisable();

    CARAPI_(Boolean) CheckIfCallerIsForegroundUser();

    CARAPI_(void) BluetoothStateChangeHandler(
        /* [in] */ Int32 prevState,
        /* [in] */ Int32 newState);

    CARAPI_(Boolean) WaitForOnOff(
        /* [in] */ Boolean on,
        /* [in] */ Boolean off);

    CARAPI_(void) SendDisableMsg();

    CARAPI_(void) SendEnableMsg(
        /* [in] */ Boolean quietMode);

    CARAPI_(Boolean) CanUnbindBluetoothService();

    CARAPI_(void) RecoverBluetoothServiceFromError();

private:
    static const String BLUETOOTH_ADMIN_PERM;
    static const String BLUETOOTH_PERM;
    static const String ACTION_SERVICE_STATE_CHANGED;
    static const String EXTRA_ACTION;
    static const String SECURE_SETTINGS_BLUETOOTH_ADDR_VALID;
    static const String SECURE_SETTINGS_BLUETOOTH_ADDRESS;
    static const String SECURE_SETTINGS_BLUETOOTH_NAME;
    static const Int32 TIMEOUT_BIND_MS;// = 3000; //Maximum msec to wait for a bind
    static const Int32 TIMEOUT_SAVE_MS;// = 500; //Maximum msec to wait for a save
    //Maximum msec to wait for service restart
    static const Int32 SERVICE_RESTART_TIME_MS;// = 200;
    //Maximum msec to delay MESSAGE_USER_SWITCHED
    static const Int32 USER_SWITCHED_TIME_MS;// = 200;

    static const Int32 MESSAGE_ENABLE;// = 1;
    static const Int32 MESSAGE_DISABLE;// = 2;
    static const Int32 MESSAGE_REGISTER_ADAPTER;// = 20;
    static const Int32 MESSAGE_UNREGISTER_ADAPTER;// = 21;
    static const Int32 MESSAGE_REGISTER_STATE_CHANGE_CALLBACK;// = 30;
    static const Int32 MESSAGE_UNREGISTER_STATE_CHANGE_CALLBACK;// = 31;
    static const Int32 MESSAGE_BLUETOOTH_SERVICE_CONNECTED;// = 40;
    static const Int32 MESSAGE_BLUETOOTH_SERVICE_DISCONNECTED;// = 41;
    static const Int32 MESSAGE_RESTART_BLUETOOTH_SERVICE;// = 42;
    static const Int32 MESSAGE_BLUETOOTH_STATE_CHANGE;//=60;
    static const Int32 MESSAGE_TIMEOUT_BIND;// =100;
    static const Int32 MESSAGE_TIMEOUT_UNBIND;// =101;
    static const Int32 MESSAGE_GET_NAME_AND_ADDRESS;//=200;
    static const Int32 MESSAGE_SAVE_NAME_AND_ADDRESS;//=201;
    static const Int32 MESSAGE_USER_SWITCHED;// = 300;
    static const Int32 MAX_SAVE_RETRIES;//=3;
    // Bluetooth persisted setting is off
    static const Int32 BLUETOOTH_OFF;//=0;
    // Bluetooth persisted setting is on
    // and Airplane mode won't affect Bluetooth state at start up
    static const Int32 BLUETOOTH_ON_BLUETOOTH;//=1;
    // Bluetooth persisted setting is on
    // but Airplane mode will affect Bluetooth state at start up
    // and Airplane mode will have higher priority.
    static const Int32 BLUETOOTH_ON_AIRPLANE;//=2;

    AutoPtr<IContext> mContext;

    // Locks are not provided for mName and mAddress.
    // They are accessed in handler or broadcast receiver, same thread context.
    String mAddress;
    String mName;
    AutoPtr<IContentResolver> mContentResolver;
    AutoPtr<IRemoteCallbackList> mCallbacks; // RemoteCallbackList<IBluetoothManagerCallback>
    AutoPtr<IRemoteCallbackList> mStateChangeCallbacks; // RemoteCallbackList<IBluetoothStateChangeCallback>
    AutoPtr<IIBluetooth> mBluetooth;
    AutoPtr<IIBluetoothGatt> mBluetoothGatt;
    Boolean mBinding;
    Boolean mUnbinding;
    // used inside handler thread
    Boolean mQuietEnable;// = FALSE;
    // configuarion from external IBinder call which is used to
    // synchronize with broadcast receiver.
    Boolean mQuietEnableExternal;
    // configuarion from external IBinder call which is used to
    // synchronize with broadcast receiver.
    Boolean mEnableExternal;
    // used inside handler thread
    Boolean mEnable;
    Int32 mState;
    AutoPtr<BluetoothHandler> mHandler;
    AutoPtr<BluetoothServiceConnection> mConnection;// = new BluetoothServiceConnection();
    AutoPtr<MyRunnable> mBTShutDown;
    AutoPtr<MyBluetoothCallback> mBluetoothCallback;
    AutoPtr<MyRunnableEx> mBTtimer;
    AutoPtr<MyBroadcastReceiver> mReceiver;
    Int32 mErrorRecoveryRetryCounter;
    Int32 mSystemUiUid;

};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_CBLUETOOTHMANAGERSERVICE_H__
