#ifndef __ELASTOS_DROID_NET_ETHERNET_ETHERNETDATATRACKER_H__
#define __ELASTOS_DROID_NET_ETHERNET_ETHERNETDATATRACKER_H__

#include "net/NetworkStateTracker.h"
#include "net/DhcpInfoInternal.h"
#include "content/BroadcastReceiver.h"
#include "os/Runnable.h"

using Elastos::Utility::Concurrent::Atomic::IAtomicBoolean;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IHandlerCallback;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::INetworkManagementService;
using Elastos::Droid::Net::INetworkStateTracker;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Net::IDhcpInfo;
using Elastos::Droid::Net::DhcpInfoInternal;
using Elastos::Droid::Net::NetworkInfoDetailedState;
using Elastos::Droid::Net::NetworkInfoState;
using Elastos::Droid::Net::ILinkProperties;
using Elastos::Droid::Net::ILinkCapabilities;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

class EthernetDataTracker
    : public ElRefBase
    , public INetworkStateTracker
{
    friend class CEDTInterfaceObserver;

private:
    class DhcpRunnable : public Runnable
    {
    public:
        DhcpRunnable(
            /* [in] */ EthernetDataTracker* owner);

        CARAPI Run();

    private:
        EthernetDataTracker* mOwner;
    };

    class PppoeStateReceiver : public BroadcastReceiver
    {
    public:
        PppoeStateReceiver(
            /* [in] */ EthernetDataTracker* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("EthernetDataTracker::PppoeStateReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        EthernetDataTracker* mOwner;
    };

public:
    static CARAPI_(AutoPtr<EthernetDataTracker>) GetInstance();

private:
    EthernetDataTracker();

public:
    ~EthernetDataTracker();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    AutoPtr<DhcpInfoInternal> GetIpConfigure(
        /* [in] */ IEthernetDevInfo* info);

    CARAPI_(void) ConnectNetwork(
        /* [in] */ Boolean up);

    CARAPI SetTeardownRequested(
        /* [in] */ Boolean isRequested);

    CARAPI IsTeardownRequested(
        /* [out] */ Boolean* isRequested);

    /**
     * Begin monitoring connectivity
     */
    CARAPI StartMonitoring(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* target);

    /**
     * Disable connectivity to a network
     * TODO: do away with return value after making MobileDataStateTracker async
     */
    CARAPI Teardown(
        /* [out] */ Boolean* result);

    /**
     * Re-enable connectivity to a network after a {@link #teardown()}.
     */
    CARAPI Reconnect(
        /* [out] */ Boolean* result);

    CARAPI CaptivePortalCheckComplete();

    /**
     * Turn the wireless radio off for a network.
     * @param turnOn {@code true} to turn the radio on, {@code false}
     */
    CARAPI SetRadio(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    /**
     * @return true - If are we currently tethered with another device.
     */
    /*synchronized*/
    CARAPI IsAvailable(
        /* [out] */ Boolean* isAvailable);

    /**
     * Tells the underlying networking system that the caller wants to
     * begin using the named feature. The interpretation of {@code feature}
     * is completely up to each networking implementation.
     * @param feature the name of the feature to be used
     * @param callingPid the process ID of the process that is issuing this request
     * @param callingUid the user ID of the process that is issuing this request
     * @return an integer value representing the outcome of the request.
     * The interpretation of this value is specific to each networking
     * implementation+feature combination, except that the value {@code -1}
     * always indicates failure.
     * TODO: needs to go away
     */
    CARAPI_(Int32) StartUsingNetworkFeature(
        /* [in] */ const String& feature,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid);

    /**
     * Tells the underlying networking system that the caller is finished
     * using the named feature. The interpretation of {@code feature}
     * is completely up to each networking implementation.
     * @param feature the name of the feature that is no longer needed.
     * @param callingPid the process ID of the process that is issuing this request
     * @param callingUid the user ID of the process that is issuing this request
     * @return an integer value representing the outcome of the request.
     * The interpretation of this value is specific to each networking
     * implementation+feature combination, except that the value {@code -1}
     * always indicates failure.
     * TODO: needs to go away
     */
    CARAPI_(Int32) StopUsingNetworkFeature(
        /* [in] */ const String& feature,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid);

    CARAPI SetUserDataEnable(
        /* [in] */ Boolean enabled);

    CARAPI SetPolicyDataEnable(
        /* [in] */ Boolean enabled);

    /**
     * Check if private DNS route is set for the network
     */
    CARAPI IsPrivateDnsRouteSet(
        /* [out] */ Boolean* isSet);

    /**
     * Set a flag indicating private DNS route is set
     */
    CARAPI SetPrivateDnsRoute(
        /* [in] */ Boolean enabled);

    /**
     * Fetch NetworkInfo for the network
     */
    /*Synchronized*/
    CARAPI GetNetworkInfo(
        /* [out] */ INetworkInfo** info);

    /**
     * Fetch LinkProperties for the network
     */
    /*Synchronized*/
    CARAPI GetLinkProperties(
        /* [out] */ ILinkProperties** linkProperties);

   /**
     * A capability is an Integer/String pair, the capabilities
     * are defined in the class LinkSocket#Key.
     *
     * @return a copy of this connections capabilities, may be empty but never null.
     */
    CARAPI GetLinkCapabilities(
        /* [out] */ ILinkCapabilities** linkCapabilities);

    /**
     * Fetch default gateway address for the network
     */
    CARAPI_(Int32) GetDefaultGatewayAddr();

    /**
     * Check if default route is set
     */
    CARAPI IsDefaultRouteSet(
        /* [out] */ Boolean* isSet);

    /**
     * Set a flag indicating default route is set for the network
     */
    CARAPI SetDefaultRoute(
        /* [in] */ Boolean enabled);

    /**
     * Return the system properties name associated with the tcp buffer sizes
     * for this network.
     */
    CARAPI GetTcpBufferSizesPropName(
        /* [out] */ String* propName);

    CARAPI SetDependencyMet(
        /* [in] */ Boolean met);

private:
    CARAPI_(void) InterfaceAdded(
        /* [in] */ const String& iface);

    CARAPI_(void) InterfaceRemoved(
        /* [in] */ const String& iface);

    CARAPI_(void) InterfaceLinkStateChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    CARAPI_(void) RunDhcp();

    CARAPI_(void) SendStateBroadcast(
        /* [in] */ Int32 event);

    CARAPI_(AutoPtr<IMessage>) ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI_(void) HandlePppoe (
        /* [in] */ Int32 pppoeState);

private:
    static const String NETWORKTYPE; // = "ETHERNET";
    static const String TAG; // = "Ethernet";

    static AutoPtr<EthernetDataTracker> sInstance;
    static Object sInstanceLock;

private:
    Boolean mLinkUp;
    String mIfaceMatch;
    String mIface;
    Object mIfaceLock;

    AutoPtr<IAtomicBoolean> mTeardownRequested;
    AutoPtr<IAtomicBoolean> mPrivateDnsRouteSet;
    AutoPtr<IAtomicInteger32> mDefaultGatewayAddr;
    AutoPtr<IAtomicBoolean> mDefaultRouteSet;

    AutoPtr<ILinkProperties> mLinkProperties;
    AutoPtr<ILinkCapabilities> mLinkCapabilities;
    AutoPtr<INetworkInfo> mNetworkInfo;
    AutoPtr<INetworkManagementEventObserver> mInterfaceObserver;
    AutoPtr<IEthernetManager> mEthManage;
    AutoPtr<INetworkManagementService> mNMService;

    /* For sending events to connectivity service handler */
    AutoPtr<IHandler> mCsHandler;
    AutoPtr<IContext> mContext;

    //add by celema
    AutoPtr<IIntentFilter> mPppoeStateFilter;
    NetworkInfoDetailedState mOldState;
    Boolean mOldisAvailable;
    Boolean mPppoe_ok;

    AutoPtr<BroadcastReceiver> mPppoeStateReceiver;

    Object mSelfLock;
};

} // Ethernet
} // Net
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_NET_ETHERNET_ETHERNETDATATRACKER_H__
