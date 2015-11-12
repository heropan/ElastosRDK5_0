
#ifndef __ELASTOS_DROID_SERVER_CONNECTIVITY_CTETHERING_H__
#define __ELASTOS_DROID_SERVER_CONNECTIVITY_CTETHERING_H__

#include "_Elastos_Droid_Server_Connectivity_CTethering.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "util/State.h"
#include "util/StateMachine.h"
#include <elastos/core/Object.h>

using Elastos::Core::IInteger32;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Net::IIConnectivityManager;
using Elastos::Droid::Net::INetworkStatsService;
using Elastos::Droid::Internal::Utility::State;
using Elastos::Droid::Internal::Utility::StateMachine;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

CarClass(CTethering)
{
public:
    class TetherInterfaceSM
        : public StateMachine
        , public IInterface
    {
    public:
        class InitialState : public State
        {
        public:
            InitialState(
                /* [in] */ TetherInterfaceSM* owner) : mOwner(owner)
            {}

            CARAPI Enter();

            CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* value);

        private:
            TetherInterfaceSM* mOwner;
        };

        class StartingState : public State
        {
        public:
            StartingState(
                /* [in] */ TetherInterfaceSM* owner) : mOwner(owner)
            {}

            CARAPI Enter();

            CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* value);

        private:
            TetherInterfaceSM* mOwner;
        };

        class TetheredState : public State
        {
        public:
            TetheredState(
                /* [in] */ TetherInterfaceSM* owner) : mOwner(owner)
            {}

            CARAPI Enter();

            CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* value);

        private:
            CARAPI_(void) CleanupUpstream();

        private:
            TetherInterfaceSM* mOwner;
        };

        class UnavailableState : public State
        {
        public:
            UnavailableState(
                /* [in] */ TetherInterfaceSM* owner) : mOwner(owner)
            {}

            CARAPI Enter();

            CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* value);

        private:
            TetherInterfaceSM* mOwner;
        };

    public:
        TetherInterfaceSM(
            /* [in] */ const String& name,
            /* [in] */ ILooper* looper,
            /* [in] */ Boolean usb,
            /* [in] */ CTethering* owner);

        CAR_INTERFACE_DECL();

        CARAPI_(String) ToString();

        CARAPI_(Int32) GetLastError();

        CARAPI_(Boolean) IsAvailable();

        CARAPI_(Boolean) IsTethered();

        CARAPI_(Boolean) IsErrored();

        CARAPI_(void) SetLastErrorAndTransitionToInitialState(
            /* [in] */ Int32 error);

    private:
        CARAPI_(void) SetLastError(
            /* [in] */ Int32 error);

        CARAPI_(void) SetAvailable(
            /* [in] */ Boolean available);

        CARAPI_(void) SetTethered(
            /* [in] */ Boolean tethered);

    public:
        // notification from the master SM that it's not in tether mode
        static const Int32 CMD_TETHER_MODE_DEAD = 1;
        // request from the user that it wants to tether
        static const Int32 CMD_TETHER_REQUESTED = 2;
        //request from the user that it wants to untether
        static const Int32 CMD_TETHER_UNREQUESTED = 3;
        // notification that this interface is down
        static const Int32 CMD_INTERFACE_DOWN = 4;
        // notification that this interface is up
        static const Int32 CMD_INTERFACE_UP = 5;
         // notification from the master SM that it had an error turning on cellular dun
        static const Int32 CMD_CELL_DUN_ERROR = 6;
        // notification from the master SM that it had trouble enabling IP Forwarding
        static const Int32 CMD_IP_FORWARDING_ENABLE_ERROR = 7;
        // notification from the master SM that it had trouble disabling IP Forwarding
        static const Int32 CMD_IP_FORWARDING_DISABLE_ERROR = 8;
        // notification from the master SM that it had trouble staring tethering
        static const Int32 CMD_START_TETHERING_ERROR = 9;
        // notification from the master SM that it had trouble stopping tethering
        static const Int32 CMD_STOP_TETHERING_ERROR = 10;
        // notification from the master SM that it had trouble setting the DNS forwarders
        static const Int32 CMD_SET_DNS_FORWARDERS_ERROR = 11;
        // the upstream connection has changed
        static const Int32 CMD_TETHER_CONNECTION_CHANGED = 12;

    public:
        Int32 mLastError;

        String mIfaceName;
        String mMyUpstreamIfaceName;  // may change over time

        Boolean mUsb;

    private:
        friend class InitialState;
        friend class UnavailableState;

        AutoPtr<State> mDefaultState;

        AutoPtr<State> mInitialState;
        AutoPtr<State> mStartingState;
        AutoPtr<State> mTetheredState;

        AutoPtr<State> mUnavailableState;

        Boolean mAvailable;
        Boolean mTethered;

        CTethering* mOwner;
    };

    class TetherMasterSM : public StateMachine
    {
    public:
        class TetherMasterUtilState : public State
        {
        public:
            TetherMasterUtilState(
                /* [in] */ TetherMasterSM* owner) : mOwner(owner)
            {}

            CARAPI ProcessMessage(
                /* [in] */ IMessage* m,
                /* [out]*/ Boolean* result);

        protected:
            CARAPI_(String) EnableString(
                /* [in] */ Int32 apnType);

            CARAPI_(Boolean) TurnOnUpstreamMobileConnection(
                /* [in] */ Int32 apnType);

            CARAPI_(Boolean) TurnOffUpstreamMobileConnection();

            CARAPI_(Boolean) TurnOnMasterTetherSettings();

            CARAPI_(Boolean) TurnOffMasterTetherSettings();

            CARAPI_(void) ChooseUpstreamType(
                /* [in]*/ Boolean tryCell);

            CARAPI_(void) NotifyTetheredOfNewUpstreamIface(
                /* [in]*/ const String& ifaceName);

        public:
            static const Boolean TRY_TO_SETUP_MOBILE_CONNECTION;
            static const Boolean WAIT_FOR_NETWORK_TO_SETTLE;

        protected:
            TetherMasterSM* mOwner;
        };

        class InitialState : public TetherMasterUtilState
        {
        public:
            InitialState(
                /* [in] */ TetherMasterSM* owner)
                : TetherMasterUtilState(owner)
            {}

            CARAPI Enter();

            CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* value);
        };

        class TetherModeAliveState : public TetherMasterUtilState
        {
        public:
            TetherModeAliveState(
                /* [in] */ TetherMasterSM* owner)
                : TetherMasterUtilState(owner)
                , mTryCell(!WAIT_FOR_NETWORK_TO_SETTLE)
            {}

            CARAPI Enter();

            CARAPI Exit();

            CARAPI ProcessMessage(
                    /* [in] */ IMessage* message,
                    /* [out] */ Boolean* value);

        public:
            Boolean mTryCell;
        };

        class ErrorState : public State
        {
        public:
            ErrorState(
                /* [in] */ TetherMasterSM* owner)
                : mErrorNotification(0)
                , mOwner(owner)
            {}

            CARAPI ProcessMessage(
                    /* [in] */IMessage* m,
                    /* [out]*/Boolean* result);

            CARAPI_(void) Notify(
                    /* [in] */Int32 msgType);

        public:
            Int32 mErrorNotification;

        protected:
            TetherMasterSM* mOwner;
        };

        class SetIpForwardingEnabledErrorState : public ErrorState
        {
        public:
            SetIpForwardingEnabledErrorState(
                /* [in] */ TetherMasterSM* owner)
                : ErrorState(owner)
            {}

            CARAPI Enter();
        };

        class SetIpForwardingDisabledErrorState : public ErrorState
        {
        public:
            SetIpForwardingDisabledErrorState(
                /* [in] */ TetherMasterSM* owner)
                : ErrorState(owner)
            {}

            CARAPI Enter();
        };

        class StartTetheringErrorState : public ErrorState
        {
        public:
            StartTetheringErrorState(
                /* [in] */ TetherMasterSM* owner)
                : ErrorState(owner)
            {}

            CARAPI Enter();
        };

        class StopTetheringErrorState : public ErrorState
        {
        public:
            StopTetheringErrorState(
                /* [in] */ TetherMasterSM* owner)
                : ErrorState(owner)
            {}

            CARAPI Enter();
        };

        class SetDnsForwardersErrorState : public ErrorState
        {
        public:
            SetDnsForwardersErrorState(
                /* [in] */ TetherMasterSM* owner)
                : ErrorState(owner)
            {}

            CARAPI Enter();
        };

    public:
        TetherMasterSM(
            /* [in] */ const String& name,
            /* [in] */ ILooper* looper,
            /* [in] */ CTethering* owner);

        ~TetherMasterSM();

    public:
        // an interface SM has requested Tethering
        static const Int32 CMD_TETHER_MODE_REQUESTED = 1;
        // an interface SM has unrequested Tethering
        static const Int32 CMD_TETHER_MODE_UNREQUESTED = 2;
        // upstream connection change - do the right thing
        static const Int32 CMD_UPSTREAM_CHANGED = 3;
        // we received notice that the cellular DUN connection is up
        static const Int32 CMD_CELL_CONNECTION_RENEW = 4;
        // we don't have a valid upstream conn, check again after a delay
        static const Int32 CMD_RETRY_UPSTREAM = 5;

    private:
        friend class TetherMasterUtilState;
        friend class InitialState;

        Int32 mSequenceNumber;

        AutoPtr<State> mInitialState;
        AutoPtr<State> mTetherModeAliveState;

        AutoPtr<State> mSetIpForwardingEnabledErrorState;
        AutoPtr<State> mSetIpForwardingDisabledErrorState;
        AutoPtr<State> mStartTetheringErrorState;
        AutoPtr<State> mStopTetheringErrorState;
        AutoPtr<State> mSetDnsForwardersErrorState;

        List< AutoPtr<TetherInterfaceSM> > mNotifyList;

        Int32 mCurrentConnectionSequence;
        Int32 mMobileApnReserved;

        String mUpstreamIfaceName;

        static const Int32 UPSTREAM_SETTLE_TIME_MS = 10000;
        static const Int32 CELL_CONNECTION_RENEW_MS = 40000;
        CTethering* mOwner;
    };

private:
    class StateReceiver : public BroadcastReceiver
    {
    public:
        StateReceiver(
            /* [in] */ CTethering* owner) : mOwner(owner)
        {}

        CARAPI OnReceive(
            /* [in] */ IContext *content,
            /* [in] */ IIntent *intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CTethering::StateReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CTethering* mOwner;
    };

public:
    CTethering();

    ~CTethering();

    CARAPI constructor(
          /* [in] */ IContext* context,
          /* [in] */ INetworkManagementService* nmService,
          /* [in] */ INetworkStatsService* statsService,
          /* [in] */ IIConnectivityManager* connService,
          /* [in] */ ILooper* looper);

    CARAPI_(void) UpdateConfiguration();

    CARAPI InterfaceStatusChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    CARAPI InterfaceLinkStateChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    CARAPI_(Boolean) IsWifi(
        /* [in] */ const String& iface);

    CARAPI_(Boolean) IsBluetooth(
        /* [in] */ const  String& iface);

    CARAPI InterfaceAdded(
        /* [in] */ const String& iface);

    CARAPI InterfaceRemoved(
        /* [in] */ const String& iface);

    CARAPI LimitReached(
        /* [in] */ const String& limitName,
        /* [in] */ const String& iface);

    CARAPI InterfaceClassDataActivityChanged(
        /* [in] */ const String& lable,
        /* [in] */ Boolean active);

    CARAPI Tether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* value);

    CARAPI Untether(
        /* [in] */ const String& iface,
        /* [in] */ Int32* value);

    CARAPI GetLastTetherError(
        /* [in] */ const String& iface,
        /* [in] */ Int32* value);

    CARAPI GetTetherableUsbRegexs(
        /* [out] */ ArrayOf<String>** result);

    CARAPI GetTetherableWifiRegexs(
        /* [out] */ ArrayOf<String>** result);

    CARAPI GetTetherableBluetoothRegexs(
        /* [out] */ ArrayOf<String>** result);

    CARAPI SetUsbTethering(
        /* [in] */ Boolean enable,
        /* [out] */ Int32* result);

    CARAPI_(AutoPtr< ArrayOf<Int32> >) GetUpstreamIfaceTypes();

    CARAPI_(void) CheckDunRequired();

    CARAPI GetTetheredIfaces(
        /* [out] */ ArrayOf<String>** result);

    CARAPI GetTetheredIfacePairs(
        /* [out] */ ArrayOf<String>** result);

    CARAPI GetTetherableIfaces(
        /* [out] */ ArrayOf<String>** result);

    CARAPI_(AutoPtr< ArrayOf<String> >) GetErroredIfaces();

    CARAPI_(void) HandleTetherIfaceChange();

    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    CARAPI_(Boolean) IsUsb(
        /* [in] */ const String& iface);

    CARAPI_(void) SendTetherStateChangedBroadcast();

    CARAPI_(void) ShowTetheredNotification(
        /* [in] */ Int32 icon);

    CARAPI_(void) ClearTetheredNotification();

    CARAPI_(void) TetherUsb(
        /* [in] */ Boolean enable);

    CARAPI_(Boolean) ConfigureUsbIface(
        /* [in] */ Boolean enabled);

public:
    static const String TAG;

private:
    AutoPtr<IContext> mContext;
    static const Boolean DBG;
    static const Boolean VDBG;

    Boolean mWifiTethered;
    AutoPtr<IPowerManagerWakeLock> mShutdownLock;

    // TODO - remove both of these - should be part of interface inspection/selection stuff
    AutoPtr< ArrayOf<String> > mTetherableUsbRegexs;
    AutoPtr< ArrayOf<String> > mTetherableWifiRegexs;
    AutoPtr< ArrayOf<String> > mTetherableBluetoothRegexs;
    List< AutoPtr<IInteger32> > mUpstreamIfaceTypes;

    // used to synchronize public access to members
    Object mPublicSync;

    static const AutoPtr<IInteger32> MOBILE_TYPE;
    static const AutoPtr<IInteger32> HIPRI_TYPE;
    static const AutoPtr<IInteger32> DUN_TYPE;

    // if we have to connect to mobile, what APN type should we use?  Calculated by examining the
    // upstream type list and the DUN_REQUIRED secure-setting
    Int32 mPreferredUpstreamMobileApn;

    AutoPtr<INetworkManagementService> mNMService;
    AutoPtr<INetworkStatsService> mStatsService;
    AutoPtr<IIConnectivityManager> mConnService;
    AutoPtr<ILooper> mLooper;
    AutoPtr<IHandlerThread> mThread;

    HashMap<String, AutoPtr<TetherInterfaceSM> > mIfaces; // all tethered/tetherable ifaces

    AutoPtr<IBroadcastReceiver> mStateReceiver;

    static const String USB_NEAR_IFACE_ADDR;
    static const Int32 USB_PREFIX_LENGTH;

    // USB is  192.168.42.1 and 255.255.255.0
    // Wifi is 192.168.43.1 and 255.255.255.0
    // BT is limited to max default of 5 connections. 192.168.44.1 to 192.168.48.1
    // with 255.255.255.0

    AutoPtr< ArrayOf<String> > mDhcpRange;
    static AutoPtr< ArrayOf<String> > DHCP_DEFAULT_RANGE;

    AutoPtr< ArrayOf<String> > mDefaultDnsServers;
    static const String DNS_DEFAULT_SERVER1;
    static const String DNS_DEFAULT_SERVER2;

    AutoPtr<StateMachine> mTetherMasterSM;

    AutoPtr<INotification> mTetheredNotification;

    Boolean mRndisEnabled; // track the RNDIS function enabled state
    Boolean mUsbTetherRequested; // true if USB tethering should be started when RNDIS is enabled
};

} // Connectivity
} // Server
} // Droid
} // Elastos

#endif
