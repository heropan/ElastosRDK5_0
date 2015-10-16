#ifndef __ELASTOS_DROID_NET_CWIFIP2PSERVICE_H__
#define __ELASTOS_DROID_NET_CWIFIP2PSERVICE_H__

#include "_Elastos_Droid_Net_CWifiP2pService.h"
#include "elastos/droid/ext/frameworkdef.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include "elastos/droid/utility/StateMachine.h"
#include "elastos/droid/utility/State.h"
#include "elastos/droid/utility/AsyncChannel.h"
#include "elastos/droid/os/SystemProperties.h"

using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::IMessenger;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Os::INetworkManagementService;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::INotification;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Net::Wifi::P2p::Nsd::IWifiP2pServiceInfo;
using Elastos::Droid::Net::Wifi::P2p::Nsd::IWifiP2pServiceRequest;
using Elastos::Droid::Net::Wifi::P2p::Nsd::IWifiP2pServiceResponse;
using Elastos::Droid::Internal::Utility::AsyncChannel;
using Elastos::Droid::Internal::Utility::StateMachine;
using Elastos::Droid::Internal::Utility::State;

namespace Elastos {
namespace Droid {
namespace Net {

class DhcpStateMachine;

namespace Wifi {

class WifiMonitor;
class WifiNative;

}}}}

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

CarClass(CWifiP2pService)
{
public:
    /**
     * Information about a particular client and we track the service discovery requests
     * and the local services registered by the client.
     */
    class ClientInfo
        : public ElRefBase
    {
    public:
        ClientInfo(
            /* [in] */ IMessenger* m,
            /* [in] */ CWifiP2pService* host);

        CARAPI GetMessenger(
            /* [out] */ IMessenger** messenger);

        CARAPI SetMessenger(
            /* [in] */ IMessenger* messenger);

    public:
        /*
         * A reference to WifiP2pManager.Channel handler.
         * The response of this request is notified to WifiP2pManager.Channel handler
         */
        AutoPtr<IMessenger> mMessenger;

        /*
         * A service discovery request list.
         */
        HashMap<Int32, AutoPtr<IWifiP2pServiceRequest> > mReqList;

        /*
         * A local service information list.
         */
        List<AutoPtr<IWifiP2pServiceInfo> > mServList;

        CWifiP2pService* mHost;
    };

    class P2pStateMachine
        : public StateMachine
    {
    public:
        class DefaultState : public State
        {
        public:
            DefaultState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("DefaultState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class P2pNotSupportedState : public State
        {
        public:
            P2pNotSupportedState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("P2pNotSupportedState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class P2pDisablingState : public State
        {
        public:
            P2pDisablingState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            // @Override
            virtual CARAPI Exit();

            virtual CARAPI_(String) GetName()
            {
                return String("P2pDisablingState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class P2pDisabledState : public State
        {
        public:
            P2pDisabledState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("P2pDisabledState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class P2pEnablingState : public State
        {
        public:
            P2pEnablingState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("P2pEnablingState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class P2pEnabledState : public State
        {
        public:
            P2pEnabledState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            // @Override
            virtual CARAPI Exit();

            virtual CARAPI_(String) GetName()
            {
                return String("P2pEnabledState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class InactiveState : public State
        {
        public:
            InactiveState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("InactiveState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class GroupCreatingState : public State
        {
        public:
            GroupCreatingState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("GroupCreatingState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class UserAuthorizingInvitationState : public State
        {
        public:
            UserAuthorizingInvitationState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            // @Override
            virtual CARAPI Exit();

            virtual CARAPI_(String) GetName()
            {
                return String("UserAuthorizingInvitationState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class ProvisionDiscoveryState : public State
        {
        public:
            ProvisionDiscoveryState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("ProvisionDiscoveryState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class GroupNegotiationState : public State
        {
        public:
            GroupNegotiationState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("GroupNegotiationState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class FrequencyConflictState : public State
        {
        public:
            FrequencyConflictState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            CARAPI NotifyFrequencyConflict();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI Exit();

            virtual CARAPI_(String) GetName()
            {
                return String("FrequencyConflictState");
            }
        private:
            P2pStateMachine* mHost;
            AutoPtr<IAlertDialog> mFrequencyConflictDialog;
        };

        class GroupCreatedState : public State
        {
        public:
            GroupCreatedState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI Exit();

            virtual CARAPI_(String) GetName()
            {
                return String("GroupCreatedState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class UserAuthorizingJoinState : public State
        {
        public:
            UserAuthorizingJoinState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            // @Override
            virtual CARAPI Exit();

            virtual CARAPI_(String) GetName()
            {
                return String("UserAuthorizingJoinState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class OngoingGroupRemovalState : public State
        {
        public:
            OngoingGroupRemovalState(
                /* [in] */ P2pStateMachine* host);

            // @Override
            virtual CARAPI Enter();

            // @Override
            virtual CARAPI ProcessMessage(
                /* [in] */ IMessage* message,
                /* [out] */ Boolean* result);

            virtual CARAPI_(String) GetName()
            {
                return String("OngoingGroupRemovalState");
            }
        private:
            P2pStateMachine* mHost;
        };

        class GroupDeleteListener
            : public ElRefBase
            , public IGroupDeleteListener
        {
        public:
            CAR_INTERFACE_DECL()

            GroupDeleteListener(
                /* [in] */ P2pStateMachine* host);

            CARAPI OnDeleteGroup(
                /* [in] */ Int32 netId);

        private:
            P2pStateMachine* mHost;
        };

        class PositiveButtonListener
            : public ElRefBase
            , public IDialogInterfaceOnClickListener
        {
        public:
            CAR_INTERFACE_DECL()

            PositiveButtonListener(
                /* [in] */ P2pStateMachine* host);

            CARAPI OnClick(
                /* [in] */ IDialogInterface* dialog,
                /* [in] */ Int32 which);
        private:
            P2pStateMachine* mHost;
        };

        class NegativeButtonListener
            : public ElRefBase
            , public IDialogInterfaceOnClickListener
        {
        public:
            CAR_INTERFACE_DECL()

            NegativeButtonListener(
                /* [in] */ P2pStateMachine* host);

            CARAPI OnClick(
                /* [in] */ IDialogInterface* dialog,
                /* [in] */ Int32 which);
        private:
            P2pStateMachine* mHost;
        };

        class DialogCancelListener
            : public ElRefBase
            , public IDialogInterfaceOnCancelListener
        {
        public:
            CAR_INTERFACE_DECL()

            DialogCancelListener(
                /* [in] */ P2pStateMachine* host);

            CARAPI OnCancel(
                /* [in] */ IDialogInterface* dialog);
        private:
            P2pStateMachine* mHost;
        };

    public:
        P2pStateMachine(
            /* [in] */ const String& name,
            /* [in] */ Boolean p2pSupported,
            /* [in] */ CWifiP2pService* host);

        CARAPI SendP2pStateChangedBroadcast(
            /* [in] */ Boolean enabled);

        CARAPI SendP2pDiscoveryChangedBroadcast(
            /* [in] */ Boolean started);

        CARAPI SendThisDeviceChangedBroadcast();

        CARAPI SendP2pPeersChangedBroadcast();

        CARAPI SendP2pConnectionChangedBroadcast();

        CARAPI SendP2pPersistentGroupsChangedBroadcast();

        CARAPI StartDhcpServer(
            /* [in] */ const String& intf);

        CARAPI StopDhcpServer(
            /* [in] */ const String& intf);

        CARAPI NotifyP2pEnableFailure();

        CARAPI AddRowToDialog(
            /* [in] */ IViewGroup* group,
            /* [in] */ Int32 stringId,
            /* [in] */ const String& value);

        CARAPI NotifyInvitationSent(
            /* [in] */ const String& pin,
            /* [in] */ const String& peerAddress);

        CARAPI NotifyInvitationReceived();

        /**
         * Synchronize the persistent group list between
         * wpa_supplicant and mGroups.
         */
        CARAPI UpdatePersistentNetworks(
            /* [in] */ Boolean reload);

        /**
         * Try to connect to the target device.
         *
         * Use the persistent credential if it has been stored.
         *
         * @param config
         * @param tryInvocation if true, try to invoke. Otherwise, never try to invoke.
         * @return
         */
        CARAPI_(Int32) Connect(
            /* [in] */ IWifiP2pConfig* config,
            /* [in] */ Boolean tryInvocation);

        /**
         * Return the network id of the group owner profile which has the p2p client with
         * the specified device address in it's client list.
         * If more than one persistent group of the same address is present in its client
         * lists, return the first one.
         *
         * @param deviceAddress p2p device address.
         * @return the network id. if not found, return -1.
         */
        CARAPI_(Int32) GetNetworkIdFromClientList(
            /* [in] */ const String& deviceAddress);

        /**
         * Return p2p client list associated with the specified network id.
         * @param netId network id.
         * @return p2p client list. if not found, return null.
         */
        CARAPI_(AutoPtr<ArrayOf<String> >) GetClientList(
            /* [in] */ Int32 netId);

        /**
         * Remove the specified p2p client from the specified profile.
         * @param netId network id of the profile.
         * @param addr p2p client address to be removed.
         * @param isRemovable if true, remove the specified profile if its client list becomes empty.
         * @return whether removing the specified p2p client is successful or not.
         */
        CARAPI_(Boolean) RemoveClientFromList(
            /* [in] */ Int32 netId,
            /* [in] */ const String& addr,
            /* [in] */ Boolean isRemovable);

        CARAPI SetWifiP2pInfoOnGroupFormation(
            /* [in] */ const String& serverAddress);

        CARAPI ResetWifiP2pInfo();

        CARAPI_(String) GetDeviceName(
            /* [in] */ const String& deviceAddress);

        CARAPI P2pConnectWithPinDisplay(
            /* [in] */ IWifiP2pConfig* config);

        CARAPI_(String) GetPersistedDeviceName();

        CARAPI_(Boolean) SetAndPersistDeviceName(
            /* [in] */ const String& devName);

        CARAPI_(Boolean) SetWfdInfo(
            /* [in] */ IWifiP2pWfdInfo* wfdInfo);

        CARAPI InitializeP2pSettings();

        CARAPI_(Boolean) SetGroupOwnerPsk(
            /* [in] */ const String& devName);

        CARAPI UpdateThisDevice(
            /* [in] */ Int32 status);

        CARAPI HandleGroupCreationFailure();

        CARAPI HandleGroupRemoved();

        //State machine initiated requests can have replyTo set to null indicating
        //there are no recipients, we ignore those reply actions
        CARAPI ReplyToMessage(
            /* [in] */ IMessage* msg,
            /* [in] */ Int32 what);

        CARAPI ReplyToMessage(
            /* [in] */ IMessage* msg,
            /* [in] */ Int32 what,
            /* [in] */ Int32 arg1);

        CARAPI ReplyToMessage(
            /* [in] */ IMessage* msg,
            /* [in] */ Int32 what,
            /* [in] */ IInterface* obj);

        using StateMachine::ObtainMessage;

        /* arg2 on the source message has a hash code that needs to be retained in replies
         * see WifiP2pManager for details */
        CARAPI_(AutoPtr<IMessage>) ObtainMessage(
            /* [in] */ IMessage* srcMsg);

        CARAPI Logd(
            /* [in] */ const String& s);

        CARAPI Loge(
            /* [in] */ const String& s);

        CARAPI Logd(
            /* [in] */ const char* s);

        CARAPI Loge(
            /* [in] */ const char* s);

        static CARAPI_(String) CmdToString(
            /* [in] */ Int32 what);

        CARAPI_(AutoPtr<IWifiP2pDevice>) GetWifiP2pDeviceFromPeers(
            /* [in] */ const String& s);

        CARAPI_(Boolean) ComparedMacAddr(
            /* [in] */ const String& inputa,
            /* [in] */ const String& inputb);

        /**
         * Update service discovery request to wpa_supplicant.
         */
        CARAPI_(Boolean) UpdateSupplicantServiceRequest();

        /**
         * Clear service discovery request in wpa_supplicant
         */
        CARAPI ClearSupplicantServiceRequest();

        /* TODO: We could track individual service adds separately and avoid
         * having to do update all service requests on every new request
         */
        CARAPI_(Boolean) AddServiceRequest(
            /* [in] */ IMessenger* m,
            /* [in] */ IWifiP2pServiceRequest* req);

        CARAPI RemoveServiceRequest(
            /* [in] */ IMessenger* m,
            /* [in] */ IWifiP2pServiceRequest* req);

        CARAPI ClearServiceRequests(
            /* [in] */ IMessenger* m);

        CARAPI_(Boolean) AddLocalService(
            /* [in] */ IMessenger* m,
            /* [in] */ IWifiP2pServiceInfo* servInfo);

        CARAPI RemoveLocalService(
            /* [in] */ IMessenger* m,
            /* [in] */ IWifiP2pServiceInfo* servInfo);

        CARAPI ClearLocalServices(
            /* [in] */ IMessenger* m);

        CARAPI ClearClientInfo(
            /* [in] */ IMessenger* m);

        /**
         * Send the service response to the WifiP2pManager.Channel.
         *
         * @param resp
         */
        CARAPI SendServiceResponse(
            /* [in] */ IWifiP2pServiceResponse* resp);

        /**
         * We dont get notifications of clients that have gone away.
         * We detect this actively when services are added and throw
         * them away.
         *
         * TODO: This can be done better with full async channels.
         */
        CARAPI ClearClientDeadChannels();

        /**
         * Return the specified ClientInfo.
         * @param m Messenger
         * @param createIfNotExist if true and the specified channel info does not exist,
         * create new client info.
         * @return the specified ClientInfo.
         */
        CARAPI GetClientInfo(
            /* [in] */ IMessenger* m,
            /* [in] */ Boolean createIfNotExist,
            /* [out] */ ClientInfo** clientInfo);

        /**
         * Send detached message to dialog listener in the foreground application.
         * @param reason
         */
        CARAPI SendDetachedMsg(
            /* [in] */ Int32 reason);

        /**
         * Send a request to show wps pin to dialog listener in the foreground application.
         * @param pin WPS pin
         * @return
         */
        CARAPI_(Boolean) SendShowPinReqToFrontApp(
            /* [in] */ const String& pin);

        /**
         * Send a request to establish the connection to dialog listener in the foreground
         * application.
         * @param dev source device
         * @param config
         * @return
         */
        CARAPI_(Boolean) SendConnectNoticeToApp(
            /* [in] */ IWifiP2pDevice* dev,
            /* [in] */ IWifiP2pConfig* config);

        /**
         * Send dialog event message to front application's dialog listener.
         * @param msg
         * @return true if success.
         */
        CARAPI_(Boolean) SendDialogMsgToFrontApp(
            /* [in] */ IMessage* msg);

        /**
         * Set dialog listener application.
         * @param m
         * @param appPkgName if null, reset the listener.
         * @param isReset if true, try to reset.
         * @return
         */
        CARAPI_(Boolean) SetDialogListenerApp(
            /* [in] */ IMessenger* m,
            /* [in] */ const String& appPkgName,
            /* [in] */ Boolean isReset);

        /**
         * Return true if the specified package name is foreground app's.
         *
         * @param pkgName application package name.
         * @return
         */
        CARAPI_(Boolean) IsForegroundApp(
            /* [in] */ const String& pkgName);

    public:
        AutoPtr<DefaultState> mDefaultState;// = new DefaultState();
        AutoPtr<P2pNotSupportedState> mP2pNotSupportedState;// = new P2pNotSupportedState();
        AutoPtr<P2pDisablingState> mP2pDisablingState;// = new P2pDisablingState();
        AutoPtr<P2pDisabledState> mP2pDisabledState;// = new P2pDisabledState();
        AutoPtr<P2pEnablingState> mP2pEnablingState;// = new P2pEnablingState();
        AutoPtr<P2pEnabledState> mP2pEnabledState;// = new P2pEnabledState();
        // Inactive is when p2p is enabled with no connectivity
        AutoPtr<InactiveState> mInactiveState;// = new InactiveState();
        AutoPtr<GroupCreatingState> mGroupCreatingState;// = new GroupCreatingState();
        AutoPtr<UserAuthorizingInvitationState> mUserAuthorizingInvitationState;// = new UserAuthorizingInvitationState();
        AutoPtr<ProvisionDiscoveryState> mProvisionDiscoveryState;// = new ProvisionDiscoveryState();
        AutoPtr<GroupNegotiationState> mGroupNegotiationState;// = new GroupNegotiationState();
        AutoPtr<FrequencyConflictState> mFrequencyConflictState;// =new FrequencyConflictState();

        AutoPtr<GroupCreatedState> mGroupCreatedState;
        AutoPtr<UserAuthorizingJoinState> mUserAuthorizingJoinState;// = new UserAuthorizingJoinState();
        AutoPtr<OngoingGroupRemovalState> mOngoingGroupRemovalState;// = new OngoingGroupRemovalState();

        AutoPtr<WifiNative> mWifiNative;
        AutoPtr<WifiMonitor> mWifiMonitor;

        AutoPtr<IWifiP2pDeviceList> mPeers;
        AutoPtr<IWifiP2pDeviceList> mNotP2pDevice;

        // /* During a connection, supplicant can tell us that a device was lost. From a supplicant's
        //  * perspective, the discovery stops during connection and it purges device since it does
        //  * not get latest updates about the device without being in discovery state.
        //  *
        //  * From the framework perspective, the device is still there since we are connecting or
        //  * connected to it. so we keep these devices in a seperate list, so that they are removed
        //  * when connection is cancelled or lost
        //  */
        AutoPtr<IWifiP2pDeviceList> mPeersLostDuringConnection;
        AutoPtr<IWifiP2pGroupList> mGroups;

        AutoPtr<IWifiP2pInfo> mWifiP2pInfo;
        AutoPtr<IWifiP2pGroup> mGroup;

        // wfd info
        AutoPtr<IWifiP2pDevice> mWifiP2pDevice;

        // Saved WifiP2pConfig for a peer connection
        AutoPtr<IWifiP2pConfig> mSavedPeerConfig;

        // Saved WifiP2pGroup from invitation request
        AutoPtr<IWifiP2pGroup> mSavedP2pGroup;

        // Saved WifiP2pDevice from provisioning request
        AutoPtr<IWifiP2pDevice> mSavedProvDiscDevice;

        CWifiP2pService* mHost;
    };

public:
    CWifiP2pService();

    ~CWifiP2pService();

    CARAPI constructor(
        /* [in] */ IContext * context);

    CARAPI ConnectivityServiceReady();

    CARAPI GetMessenger(
        /* [out] */ IMessenger** msg);

    static CARAPI_(P2pStatus) ValueOf(
        /* [in] */ Int32 error);

private:
    CARAPI EnforceAccessPermission();

    CARAPI EnforceChangePermission();

public:
    static const String TAG;
    static const Boolean DBG;
    static const String NETWORKTYPE;

    static const Boolean JOIN_GROUP;
    static const Boolean FORM_GROUP;

    static const Boolean TRY_REINVOCATION;
    static const Boolean NO_REINVOCATION;

    static const Boolean RELOAD;
    static const Boolean NO_RELOAD;

    static const Int32 CONNECT_FAILURE;
    static const Int32 CONNECT_SUCCESS;
    static const Int32 NEEDS_PROVISION_REQ;

    /* Two minutes comes from the wpa_supplicant setting */
    static const Int32 GROUP_CREATING_WAIT_TIME_MS;

    static const Int32 DISABLE_P2P_WAIT_TIME_MS;

    /* Set a two minute discover timeout to avoid STA scans from being blocked */
    static const Int32 DISCOVER_TIMEOUT_S;

    /* Idle time after a peer is gone when the group is torn down */
    static const Int32 GROUP_IDLE_TIME_S;

    static const Int32 BASE;

    /* User accepted a peer request */
    static const Int32 PEER_CONNECTION_USER_ACCEPT;
    /* User rejected a peer request */
    static const Int32 PEER_CONNECTION_USER_REJECT;
    /* User wants to disconnect wifi in favour of p2p */
    static const Int32 DROP_WIFI_USER_ACCEPT;
    /* User wants to keep his wifi connection and drop p2p */
    static const Int32 DROP_WIFI_USER_REJECT;

    /* indicate wfd device P2pGroupOnwer:1 or P2pClient:2,empty unknown*/
    static const String WFD_P2P_ROLE;// = "wlan.wfdp2p.role";
    static const String WFD_DNSMASQ_PEER;// = "wlan.wfddnsmasq.peer";
    static const String WFD_P2P_DEVICE_ADDR;// = "wlan.wfdp2p.addr";

    /* Is chosen as a unique range to avoid conflict with
       the range defined in Tethering.java */
    static const AutoPtr<ArrayOf<String> > DHCP_RANGE;
    static const String SERVER_ADDRESS;

    static const String SERVER_ADDRESS_TETHER;// = "192.168.43.1";//copy from Tethering.java

    //tether mode with p2p
    String mTetherInterfaceName;

    static Int32 mGroupCreatingTimeoutIndex;
    static Int32 mDisableP2pTimeoutIndex;

    AutoPtr<IContext> mContext;
    String mInterface;
    AutoPtr<INotification> mNotification;

    //tether mode with p2p
    AutoPtr<IConnectivityManager> mConnectivityManager;

    AutoPtr<INetworkManagementService> mNwService;
    AutoPtr<DhcpStateMachine> mDhcpStateMachine;

    AutoPtr<IActivityManager> mActivityMgr;

    AutoPtr<P2pStateMachine> mP2pStateMachine;
    AutoPtr<AsyncChannel> mReplyChannel;
    AutoPtr<AsyncChannel> mWifiChannel;

    Boolean mP2pSupported;

    AutoPtr<IWifiP2pDevice> mThisDevice;

    /* When a group has been explicitly created by an app, we persist the group
     * even after all clients have been disconnected until an explicit remove
     * is invoked */
    Boolean mAutonomousGroup;

    /* Invitation to join an existing p2p group */
    Boolean mJoinExistingGroup;

    /* Track whether we are in p2p discovery. This is used to avoid sending duplicate
     * broadcasts
     */
    Boolean mDiscoveryStarted;

    AutoPtr<INetworkInfo> mNetworkInfo;

    Boolean mTempoarilyDisconnectedWifi;

    /* The transaction Id of service discovery request */
    Byte mServiceTransactionId;

    /* Service discovery request ID of wpa_supplicant.
     * null means it's not set yet. */
    String mServiceDiscReqId;

    /* clients(application) information list. */
    HashMap<AutoPtr<IMessenger>, AutoPtr<ClientInfo> > mClientInfoList;

    /* The foreground application's messenger.
     * The connection request is notified only to foreground application  */
    AutoPtr<IMessenger> mForegroundAppMessenger;

    /* the package name of foreground application. */
    String mForegroundAppPkgName;
};

}
}
}
}
}
#endif // __ELASTOS_DROID_NET_CWIFIP2PSERVICE_H__
