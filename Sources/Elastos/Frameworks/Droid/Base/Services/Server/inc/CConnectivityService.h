
#ifndef __ELASTOS_DROID_SERVER_CCONNECTIVITYSERVICE_H__
#define __ELASTOS_DROID_SERVER_CCONNECTIVITYSERVICE_H__

#include "ext/frameworkext.h"
#include "_CConnectivityService.h"
#include "database/ContentObserver.h"
#include "net/CaptivePortalTracker.h"
#include "net/LockdownVpnTracker.h"
#include "net/NetworkStateTracker.h"
#include <elastos/utility/etl/HashSet.h>
#include <elastos/utility/etl/HashMap.h>
#include "os/HandlerBase.h"

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::HashSet;
using Elastos::Security::IKeyStore;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Net::IInetAddress;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Net::CaptivePortalTracker;
using Elastos::Droid::Net::ILinkProperties;
using Elastos::Droid::Net::IConnectivityManagerHelper;
using Elastos::Droid::Net::IINetworkPolicyManager;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Net::INetworkConfig;
using Elastos::Droid::Net::INetworkManagementEventObserver;
using Elastos::Droid::Net::INetworkPolicyListener;
using Elastos::Droid::Net::INetworkQuotaInfo;
using Elastos::Droid::Net::INetworkStatsService;
using Elastos::Droid::Net::INetworkState;
using Elastos::Droid::Net::INetworkStateTracker;
using Elastos::Droid::Net::IProxyProperties;
using Elastos::Droid::Net::IRouteInfo;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::INetworkManagementService;
using Elastos::Droid::Server::Net::LockdownVpnTracker;
using Elastos::Droid::Internal::Net::IVpnConfig;
using Elastos::Droid::Internal::Net::IVpnProfile;
using Elastos::Droid::Internal::Net::ILegacyVpnInfo;

namespace Elastos {
namespace Droid {
namespace Server {

namespace Connectivity {
class CTethering;
class Vpn;
}

class CDataActivityObserver;

CarClass(CConnectivityService)
{
public:
    friend class RadioAttributes;
    friend class InternalBroadcastReciver;
    friend class VpnCallback;
    friend class DefaultNetworkFactory;
    friend class NetworkStateTrackerHandler;
    friend class SettingsObserver;
    friend class FeatureUser;
    friend class CNetworkPolicyListener;
    friend class CDataActivityObserver;

    /**
     * Factory that creates {@link NetworkStateTracker} instances using given
     * {@link NetworkConfig}.
     */
    interface INetworkFactory : public IInterface
    {
        virtual CARAPI CreateTracker(
            /* [in] */ Int32 targetNetworkType,
            /* [in] */ INetworkConfig* config,
            /* [out] */ INetworkStateTracker** tracker) = 0;
    };

    /**
     * Callback for VPN subsystem. Currently VPN is not adapted to the service
     * through NetworkStateTracker since it works differently. For example, it
     * needs to override DNS servers but never takes the default routes. It
     * relies on another data network, and it could keep existing connections
     * alive after reconnecting, switching between networks, or even resuming
     * from deep sleep. Calls from applications should be done synchronously
     * to avoid race conditions. As these are all hidden APIs, refactoring can
     * be done whenever a better abstraction is developed.
     */
    class VpnCallback : public ElRefBase
    {
    public:
        VpnCallback(
            /* [in] */ CConnectivityService* owner)
            : mOwner(owner)
        {}

        CARAPI OnStateChanged(
            /* [in] */ INetworkInfo* info);

        CARAPI Override(
            /* [in] */ IObjectContainer* dnsServers,
            /* [in] */ IObjectContainer* searchDomains);

        CARAPI Restore();

    private:
        CConnectivityService* mOwner;
    };

private:
    class RadioAttributes : public ElLightRefBase
    {
    public:
        RadioAttributes(
            /* [in] */ const String& init);

    public:
        Int32 mSimultaneity;
        Int32 mType;
    };

    class DefaultNetworkFactory
        : public ElRefBase
        , public INetworkFactory
    {
    public:
        DefaultNetworkFactory(
            /* [in] */ IContext* context,
            /* [in] */ IHandler* trackerHandler,
            /* [in] */ CConnectivityService* owner);

        CAR_INTERFACE_DECL();

        CARAPI CreateTracker(
            /* [in] */ Int32 targetNetworkType,
            /* [in] */ INetworkConfig* config,
            /* [out] */ INetworkStateTracker** tracker);

    private:
        AutoPtr<IContext> mContext;
        AutoPtr<IHandler> mTrackerHandler;
        CConnectivityService* mOwner;
    };

    /**
     * Used to notice when the calling process dies so we can self-expire
     *
     * Also used to know if the process has cleaned up after itself when
     * our auto-expire timer goes off.  The timer has a link to an object.
     *
     */
    class FeatureUser
        : public ElRefBase
        , public IProxyDeathRecipient
    {
        friend class CConnectivityService;

    public:
        FeatureUser(
            /* [in] */ Int32 type,
            /* [in] */ const String& feature,
            /* [in] */ IBinder* binder,
            /* [in] */ CConnectivityService* owner);

        CAR_INTERFACE_DECL()

        CARAPI_(void) UnlinkDeathRecipient();

        CARAPI ProxyDied();

        CARAPI Expire();

        CARAPI_(Boolean) IsSameUser(
            /* [in] */ FeatureUser* u);

        CARAPI_(Boolean) IsSameUser(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 networkType,
            /* [in] */ const String& feature);

        CARAPI_(String) ToString();

    public:
        Int32 mNetworkType;
        String mFeature;
        AutoPtr<IBinder> mBinder;
        Int32 mPid;
        Int32 mUid;
        Int64 mCreateTime;

    private:
        CConnectivityService* mOwner;
    };

    class InternalBroadcastReciver: public BroadcastReceiver
    {
    public:
        InternalBroadcastReciver(
            /* [in] */ CConnectivityService* owner)
            : mOwner(owner)
        {}

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CConnectivityService::InternalBroadcastReciver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CConnectivityService* mOwner;
    };

    class InternalHandler
        : public HandlerBase
    {
    public:
        InternalHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CConnectivityService* owner)
            : HandlerBase(looper)
            , mOwner(owner)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CConnectivityService* mOwner;
    };

    // must be stateless - things change under us.
    class NetworkStateTrackerHandler
        : public HandlerBase
    {
    public:
        NetworkStateTrackerHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CConnectivityService* owner)
            : HandlerBase(looper)
            , mOwner(owner)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CConnectivityService* mOwner;
    };

    class SettingsObserver : public ContentObserver
    {
    public:
        SettingsObserver(
            /* [in] */ IHandler* observerHandler,
            /* [in] */ Int32 what,
            /* [in] */ CConnectivityService* owner);

        CARAPI_(void) Observe(
            /* [in] */ IContext* context);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        AutoPtr<IHandler> mHandler;
        Int32 mWhat;
        CConnectivityService* mOwner;
    };

public:
    CConnectivityService();

    ~CConnectivityService();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ INetworkManagementService* netd,
        /* [in] */ INetworkStatsService* statsService,
        /* [in] */ IINetworkPolicyManager* policyManager);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ INetworkManagementService* netManager,
        /* [in] */ INetworkStatsService* statsService,
        /* [in] */ IINetworkPolicyManager* policyManager,
        /* [in] */ Handle32 netFac);

    /**
    * Sets the preferred network.
    * @param preference the new preference
    */
    CARAPI SetNetworkPreference(
        /* [in] */ Int32 preference);

    CARAPI GetNetworkPreference(
        /* [out] */ Int32* preference);

    /**
    * Return NetworkInfo for the active (i.e., connected) network interface.
    * It is assumed that at most one network is active at a time. If more
    * than one is active, it is indeterminate which will be returned.
    * @return the info for the active network, or {@code null} if none is
    * active
    */
    CARAPI GetActiveNetworkInfo(
        /* [out] */ INetworkInfo** info);

    CARAPI GetActiveNetworkInfoUnfiltered(
        /* [out] */ INetworkInfo** info);

    CARAPI GetActiveNetworkInfoForUid(
        /* [in] */ Int32 uid,
        /* [out] */ INetworkInfo** info);

    CARAPI GetNetworkInfo(
        /* [in] */ Int32 networkType,
        /* [out] */ INetworkInfo** info);

    CARAPI GetAllNetworkInfo(
        /* [out, callee] */ ArrayOf<INetworkInfo*>** allInfo);

    CARAPI IsNetworkSupported(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    /**
    * Return LinkProperties for the active (i.e., connected) default
    * network interface.  It is assumed that at most one default network
    * is active at a time. If more than one is active, it is indeterminate
    * which will be returned.
    * @return the ip properties for the active network, or {@code null} if
    * none is active
    */
    CARAPI GetActiveLinkProperties(
        /* [out] */ ILinkProperties** properties);

    CARAPI GetLinkProperties(
        /* [in] */ Int32 networkType,
        /* [out] */ ILinkProperties** properties);

    CARAPI GetAllNetworkState(
        /* [out] */ ArrayOf<INetworkState*>** allStates);

    CARAPI GetActiveNetworkQuotaInfo(
        /* [out] */ INetworkQuotaInfo** info);

    CARAPI IsActiveNetworkMetered(
        /* [out] */ Boolean* result);

    CARAPI SetRadios(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI SetRadio(
        /* [in] */ Int32 netType,
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI StartUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [in] */ IBinder* binder,
        /* [out] */ Int32* status);

    CARAPI StopUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [out] */ Int32* status);

    /**
    * @deprecated use requestRouteToHostAddress instead
    *
    * Ensure that a network route exists to deliver traffic to the specified
    * host via the specified network interface.
    * @param networkType the type of the network over which traffic to the
    * specified host is to be routed
    * @param hostAddress the IP address of the host to which the route is
    * desired
    * @return {@code true} on success, {@code false} on failure
    */
    CARAPI RequestRouteToHost(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 hostAddress,
        /* [out] */ Boolean* result);

    /**
    * Ensure that a network route exists to deliver traffic to the specified
    * host via the specified network interface.
    * @param networkType the type of the network over which traffic to the
    * specified host is to be routed
    * @param hostAddress the IP address of the host to which the route is
    * desired
    * @return {@code true} on success, {@code false} on failure
    */
    CARAPI RequestRouteToHostAddress(
        /* [in] */ Int32 networkType,
        /* [in] */ ArrayOf<Byte>* hostAddress,
        /* [out] */ Boolean* result);

    /**
    * @see ConnectivityManager#getMobileDataEnabled()
    */
    CARAPI GetMobileDataEnabled(
        /* [out] */ Boolean* result);

    CARAPI SetDataDependency(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean met);

    /**
    * @see ConnectivityManager#setMobileDataEnabled(boolean)
    */
    CARAPI SetMobileDataEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SetPolicyDataEnable(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean enabled);

    CARAPI SendConnectedBroadcast(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) SystemReady();

    /** @hide */
    CARAPI CaptivePortalCheckComplete(
        /* [in] */ INetworkInfo* info);

    /**
    * Reads the network specific TCP buffer sizes from SystemProperties
    * net.tcp.buffersize.[default|wifi|umts|edge|gprs] and set them for system
    * wide use
    */
    CARAPI UpdateNetworkSettings(
        /* [in] */ INetworkStateTracker* nt);

    CARAPI Tether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* status);

    CARAPI Untether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* status);

    CARAPI GetLastTetherError(
        /* [in] */ const String& iface,
        /* [out] */ Int32* status);

    // TODO - proper iface API for selection by property, inspection, etc
    CARAPI GetTetherableUsbRegexs(
        /* [out, callee] */ ArrayOf<String>** regexs);

    CARAPI GetTetherableWifiRegexs(
        /* [out, callee] */ ArrayOf<String>** regexs);

    CARAPI GetTetherableBluetoothRegexs(
        /* [out, callee] */ ArrayOf<String>** regexs);

    CARAPI SetUsbTethering(
        /* [in] */ Boolean enable,
        /* [out] */ Int32* status);

    // TODO - move iface listing, queries, etc to new module
    // javadoc from interface
    CARAPI GetTetherableIfaces(
        /* [out, callee] */ ArrayOf<String>** ifaces);

    CARAPI GetTetheredIfaces(
        /* [out, callee] */ ArrayOf<String>** ifaces);

    CARAPI GetTetheredIfacePairs(
        /* [out, callee] */ ArrayOf<String>** ifaces);

    CARAPI GetTetheringErroredIfaces(
        /* [out, callee] */ ArrayOf<String>** ifaces);

    // if ro.tether.denied = true we default to no tethering
    // gservices could set the secure setting to 1 though to enable it on a build where it
    // had previously been turned off.
    CARAPI IsTetheringSupported(
        /* [out] */ Boolean* status);

    // An API NetworkStateTrackers can call when they lose their network.
    // This will automatically be cleared after X seconds or a network becomes CONNECTED,
    // whichever happens first.  The timer is started by the first caller and not
    // restarted by subsequent callers.
    CARAPI RequestNetworkTransitionWakelock(
        /* [in] */ const String& forWhom);

    // 100 percent is full good, 0 is full bad.
    CARAPI ReportInetCondition(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 percentage);

    CARAPI GetProxy(
        /* [out] */ IProxyProperties** proxyProperties);

    CARAPI SetGlobalProxy(
        /* [in] */ IProxyProperties* proxyProperties);

    CARAPI GetGlobalProxy(
        /* [out] */ IProxyProperties** proxyProperties);

    CARAPI_(Int32) ConvertFeatureToNetworkType(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature);

    /**
     * Protect a socket from VPN routing rules. This method is used by
     * VpnBuilder and not available in ConnectivityManager. Permissions
     * are checked in Vpn class.
     * @hide
     */
    CARAPI ProtectVpn(
        /* [in] */ IParcelFileDescriptor* socket,
        /* [out] */ Boolean* status);

    /**
     * Prepare for a VPN application. This method is used by VpnDialogs
     * and not available in ConnectivityManager. Permissions are checked
     * in Vpn class.
     * @hide
     */
    CARAPI PrepareVpn(
        /* [in] */ const String& oldPackage,
        /* [in] */ const String& newPackage,
        /* [out] */ Boolean* status);

    /**
     * Configure a TUN interface and return its file descriptor. Parameters
     * are encoded and opaque to this class. This method is used by VpnBuilder
     * and not available in ConnectivityManager. Permissions are checked in
     * Vpn class.
     * @hide
     */
    CARAPI EstablishVpn(
        /* [in] */ IVpnConfig* config,
        /* [out] */ IParcelFileDescriptor** fd);

    /**
     * Start legacy VPN, controlling native daemons as needed. Creates a
     * secondary thread to perform connection work, returning quickly.
     */
    CARAPI StartLegacyVpn(
        /* [in] */ IVpnProfile* profile);

    /**
     * Return the information of the ongoing legacy VPN. This method is used
     * by VpnSettings and not available in ConnectivityManager. Permissions
     * are checked in Vpn class.
     * @hide
     */
    CARAPI GetLegacyVpnInfo(
        /* [out] */ ILegacyVpnInfo** info);

//    //@Override
//    CARAPI SendMessage(
//        /* [in] */ Int32 what,
//        /* [in] */ INetworkInfo* info);

    CARAPI UpdateLockdownVpn(
        /* [out] */ Boolean* status);

protected:
    //@Override
    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ const ArrayOf<String>& args);

private:
    /**
     * Loads external WiMAX library and registers as system service, returning a
     * {@link NetworkStateTracker} for WiMAX. Caller is still responsible for
     * invoking {@link NetworkStateTracker#startMonitoring(Context, Handler)}.
     */
    static CARAPI_(AutoPtr<INetworkStateTracker>) MakeWimaxStateTracker(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* trackerHandler);

    CARAPI_(void) HandleSetNetworkPreference(
        /* [in] */ Int32 preference);

    CARAPI_(Int32) GetConnectivityChangeDelay();

    CARAPI_(Int32) GetPersistedNetworkPreference();

    /**
     * Make the state of network connectivity conform to the preference settings
     * In this method, we only tear down a non-preferred network. Establishing
     * a connection to the preferred network is taken care of when we handle
     * the disconnect event from the non-preferred network
     * (see {@link #handleDisconnect(NetworkInfo)}).
     */
    CARAPI_(void) EnforcePreference();

    CARAPI_(Boolean) Teardown(
        /* [in] */ INetworkStateTracker* netTracker);

    /**
     * Check if UID should be blocked from using the network represented by the
     * given {@link NetworkStateTracker}.
     */
    CARAPI_(Boolean) IsNetworkBlocked(
        /* [in] */ INetworkStateTracker* tracker,
        /* [in] */ Int32 uid);

    /**
     * Return a filtered {@link NetworkInfo}, potentially marked
     * {@link DetailedState#BLOCKED} based on
     * {@link #isNetworkBlocked(NetworkStateTracker, int)}.
     */
    CARAPI_(AutoPtr<INetworkInfo>) GetFilteredNetworkInfo(
        /* [in] */ INetworkStateTracker* tracker,
        /* [in] */ Int32 uid);

    CARAPI GetNetworkInfo(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 uid,
        /* [out] */ INetworkInfo** info);

    CARAPI_(AutoPtr<INetworkState>) GetNetworkStateUnchecked(
        /* [in] */ Int32 networkType);

    CARAPI_(Boolean) IsNetworkMeteredUnchecked(
        /* [in] */ Int32 networkType);

    CARAPI_(Int32) StopUsingNetworkFeature(
        /* [in] */ FeatureUser* u,
        /* [in] */ Boolean ignoreDups);

    CARAPI_(Boolean) AddRoute(
        /* [in] */ ILinkProperties* p,
        /* [in] */ IRouteInfo* r,
        /* [in] */ Boolean toDefaultTable);

    CARAPI_(Boolean) RemoveRoute(
        /* [in] */ ILinkProperties* p,
        /* [in] */ IRouteInfo* r,
        /* [in] */ Boolean toDefaultTable);

    CARAPI_(Boolean) AddRouteToAddress(
        /* [in] */ ILinkProperties* lp,
        /* [in] */ IInetAddress* addr);

    CARAPI_(Boolean) RemoveRouteToAddress(
        /* [in] */ ILinkProperties* lp,
        /* [in] */ IInetAddress* addr);

    CARAPI_(Boolean) ModifyRouteToAddress(
        /* [in] */ ILinkProperties* lp,
        /* [in] */ IInetAddress* addr,
        /* [in] */ Boolean doAdd,
        /* [in] */ Boolean toDefaultTable);

    CARAPI_(Boolean) ModifyRoute(
        /* [in] */ const String& ifaceName,
        /* [in] */ ILinkProperties* lp,
        /* [in] */ IRouteInfo* r,
        /* [in] */ Int32 cycleCount,
        /* [in] */ Boolean doAdd,
        /* [in] */ Boolean toDefaultTable);

    CARAPI_(void) HandleSetDependencyMet(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean met);

    CARAPI_(void) HandleSetMobileData(
        /* [in] */ Boolean enabled);

    CARAPI_(void) HandleSetPolicyDataEnable(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean enabled);

    CARAPI EnforceAccessPermission();

    CARAPI EnforceChangePermission();

    // TODO Make this a special check when it goes public
    CARAPI EnforceTetherChangePermission();

    CARAPI EnforceTetherAccessPermission();

    CARAPI EnforceConnectivityInternalPermission();

    /**
     * Handle a {@code DISCONNECTED} event. If this pertains to the non-active
     * network, we ignore it. If it is for the active network, we send out a
     * broadcast. But first, we check whether it might be possible to connect
     * to a different network.
     * @param info the {@code NetworkInfo} for the network
     */
    CARAPI_(void) HandleDisconnect(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) TryFailover(
        /* [in] */ Int32 prevNetType);

    CARAPI_(void) SendConnectedBroadcastDelayed(
        /* [in] */ INetworkInfo* info,
        /* [in] */ Int32 delayMs);

    CARAPI_(void) SendInetConditionBroadcast(
        /* [in] */ INetworkInfo* info);

    CARAPI_(AutoPtr<IIntent>) MakeGeneralIntent(
        /* [in] */ INetworkInfo* info,
        /* [in] */ const String& bcastType);

    CARAPI_(void) SendGeneralBroadcast(
        /* [in] */ INetworkInfo* info,
        /* [in] */ const String& bcastType);

    CARAPI_(void) SendGeneralBroadcastDelayed(
        /* [in] */ INetworkInfo* info,
        /* [in] */ const String& bcastType,
        /* [in] */ Int32 delayMs);

    CARAPI_(void) SendDataActivityBroadcast(
        /* [in] */ Int32 deviceType,
        /* [in] */ Boolean active);

    /**
     * Called when an attempt to fail over to another network has failed.
     * @param info the {@link NetworkInfo} for the failed network
     */
    CARAPI_(void) HandleConnectionFailure(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) SendStickyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI_(void) SendStickyBroadcastDelayed(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 delayMs);

    CARAPI_(Boolean) IsNewNetTypePreferredOverCurrentNetType(
        /* [in] */ Int32 type);

    CARAPI_(void) HandleConnect(
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) HandleCaptivePortalTrackerCheck(
        /* [in] */ INetworkInfo* info);

    /**
     * Setup data activity tracking for the given network interface.
     *
     * Every {@code setupDataActivityTracking} should be paired with a
     * {@link removeDataActivityTracking} for cleanup.
     */
    CARAPI_(void) SetupDataActivityTracking(
        /* [in] */ Int32 type);

    /**
     * Remove data activity tracking when network disconnects.
     */
    CARAPI_(void) RemoveDataActivityTracking(
        /* [in] */ Int32 type);

    /**
     * After a change in the connectivity state of a network. We're mainly
     * concerned with making sure that the list of DNS servers is set up
     * according to which networks are connected, and ensuring that the
     * right routing table entries exist.
     */
    CARAPI_(void) HandleConnectivityChange(
        /* [in] */ Int32 netType,
        /* [in] */ Boolean doReset);

    /**
     * Add and remove routes using the old properties (null if not previously connected),
     * new properties (null if becoming disconnected).  May even be double null, which
     * is a noop.
     * Uses isLinkDefault to determine if default routes should be set or conversely if
     * host routes should be set to the dns servers
     * returns a boolean indicating the routes changed
     */
    CARAPI_(Boolean) UpdateRoutes(
        /* [in] */ ILinkProperties* newLp,
        /* [in] */ ILinkProperties* curLp,
        /* [in] */ Boolean isLinkDefault);

    /**
     * Writes TCP buffer sizes to /sys/kernel/ipv4/tcp_[r/w]mem_[min/def/max]
     * which maps to /proc/sys/net/ipv4/tcp_rmem and tcpwmem
     *
     * @param bufferSizes in the format of "readMin, readInitial, readMax,
     *        writeMin, writeInitial, writeMax"
     */
    CARAPI_(void) SetBufferSize(
        /* [in] */ const String& bufferSizes);

    /**
     * Adjust the per-process dns entries (net.dns<x>.<pid>) based
     * on the highest priority active net which this process requested.
     * If there aren't any, clear it out
     */
    CARAPI_(void) ReassessPidDns(
        /* [in] */ Int32 myPid,
        /* [in] */ Boolean doBump);

    // return true if results in a change
    CARAPI_(Boolean) WritePidDns(
        /* [in] */ IObjectContainer* dnses,
        /* [in] */ Int32 pid);

    CARAPI_(void) BumpDns();

    // Caller must grab mDnsLock.
    CARAPI_(Boolean) UpdateDns(
        /* [in] */ const String& network,
        /* [in] */ const String& iface,
        /* [in] */ List< AutoPtr<IInetAddress> >& dnses,
        /* [in] */ const String& domains);

    CARAPI_(void) HandleDnsConfigurationChange(
        /* [in] */ Int32 netType);

    CARAPI_(Int32) GetRestoreDefaultNetworkDelay(
        /* [in] */ Int32 networkType);

    CARAPI_(void) HandleInetConditionChange(
        /* [in] */ Int32 netType,
        /* [in] */ Int32 condition);

    CARAPI_(void) HandleInetConditionHoldEnd(
        /* [in] */ Int32 netType,
        /* [in] */ Int32 sequence);

    CARAPI_(void) LoadGlobalProxy();

    CARAPI_(void) HandleApplyDefaultProxy(
        /* [in] */ IProxyProperties* proxy);

    CARAPI_(void) HandleDeprecatedGlobalHttpProxy();

    CARAPI_(void) SendProxyBroadcast(
        /* [in] */ IProxyProperties* proxy);

    /**
     * Internally set new {@link LockdownVpnTracker}, shutting down any existing
     * {@link LockdownVpnTracker}. Can be {@code null} to disable lockdown.
     */
    CARAPI_(void) SetLockdownTracker(
        /* [in] */ LockdownVpnTracker* tracker);

    CARAPI ThrowIfLockdownEnabled();

    static CARAPI EnforceSystemUid();

    CARAPI_(void) HandleEventClearNetTransitionWakelock(
        /* [in] */ Int32 arg1);

    CARAPI_(void) HandleEventRestoreDns();

    CARAPI_(void) HandleEventVpnStateChanged(
        /* [in] */ INetworkInfo* info);

public:
    friend class InternalHandler;

    /**
     * used internally as a delayed event to make us switch back to the
     * default network
     */
    static const Int32 EVENT_RESTORE_DEFAULT_NETWORK;

    /**
     * used internally to change our mobile data enabled flag
     */
    static const Int32 EVENT_CHANGE_MOBILE_DATA_ENABLED; // = 2;

    /**
     * used internally to change our network preference setting
     * arg1 = networkType to prefer
     */
    static const Int32 EVENT_SET_NETWORK_PREFERENCE; // = 3;

    /**
     * used internally to synchronize inet condition reports
     * arg1 = networkType
     * arg2 = condition (0 bad, 100 good)
     */
    static const Int32 EVENT_INET_CONDITION_CHANGE; // = 4;

    /**
     * used internally to mark the end of inet condition hold periods
     * arg1 = networkType
     */
    static const Int32 EVENT_INET_CONDITION_HOLD_END; // = 5;

    /**
     * used internally to set enable/disable cellular data
     * arg1 = ENBALED or DISABLED
     */
    static const Int32 EVENT_SET_MOBILE_DATA; // = 7;

    /**
     * used internally to clear a wakelock when transitioning
     * from one net to another
     */
    static const Int32 EVENT_CLEAR_NET_TRANSITION_WAKELOCK; // = 8;

    /**
     * used internally to reload global proxy settings
     */
    static const Int32 EVENT_APPLY_GLOBAL_HTTP_PROXY; // = 9;

    /**
     * used internally to set external dependency met/unmet
     * arg1 = ENABLED (met) or DISABLED (unmet)
     * arg2 = NetworkType
     */
    static const Int32 EVENT_SET_DEPENDENCY_MET; // = 10;

    /**
     * used internally to restore DNS properties back to the
     * default network
     */
    static const Int32 EVENT_RESTORE_DNS; // = 11;

    /**
     * used internally to send a sticky broadcast delayed.
     */
    static const Int32 EVENT_SEND_STICKY_BROADCAST_INTENT; // = 12;

    /**
     * Used internally to
     * {@link NetworkStateTracker#setPolicyDataEnable(boolean)}.
     */
    static const Int32 EVENT_SET_POLICY_DATA_ENABLE; // = 13;

    static const Int32 EVENT_VPN_STATE_CHANGED; // = 14;


private:
    static const String TAG;

    static const Boolean DBG;
    static const Boolean VDBG;

    static const Boolean LOGD_RULES;

    // TODO: create better separation between radio types and network types

    // how long to wait before switching back to a radio's default network
    static const Int32 RESTORE_DEFAULT_NETWORK_DELAY;
    // system property that can override the above value
    static const String NETWORK_RESTORE_DELAY_PROP_NAME;

    // used in recursive route setting to add gateways for the host for which
    // a host route was requested.
    static const Int32 MAX_HOSTROUTE_CYCLE_COUNT;

    AutoPtr<Elastos::Droid::Server::Connectivity::CTethering> mTethering;
    Boolean mTetheringConfigValid;

    AutoPtr<IKeyStore> mKeyStore;

    AutoPtr<Elastos::Droid::Server::Connectivity::Vpn> mVpn;
    AutoPtr<VpnCallback> mVpnCallback;

    Boolean mLockdownEnabled;
    AutoPtr<LockdownVpnTracker> mLockdownTracker;

    /** Lock around {@link #mUidRules} and {@link #mMeteredIfaces}. */
    Object mRulesLock;
    /** Currently active network rules by UID. */
    HashMap<Int32, Int32> mUidRules;
    /** Set of ifaces that are costly. */
    HashSet<String> mMeteredIfaces;

    /**
     * Sometimes we want to refer to the individual network state
     * trackers separately, and sometimes we just want to treat them
     * abstractly.
     */
    AutoPtr< ArrayOf<INetworkStateTracker*> > mNetTrackers;

    /* Handles captive portal check on a network */
    AutoPtr<CaptivePortalTracker> mCaptivePortalTracker;

    /**
     * The link properties that define the current links
     */
    AutoPtr< ArrayOf<ILinkProperties*> > mCurrentLinkProperties;

    /**
     * A per Net list of the PID's that requested access to the net
     * used both as a refcount and for per-PID DNS selection
     */
    AutoPtr< ArrayOf< AutoPtr< List<Int32> > > > mNetRequestersPids;

    // priority order of the nettrackers
    // (excluding dynamically set mNetworkPreference)
    // TODO - move mNetworkTypePreference into this
    AutoPtr< ArrayOf<Int32> > mPriorityList;

    AutoPtr<IContext> mContext;
    Int32 mNetworkPreference;
    Int32 mActiveDefaultNetwork;
    // 0 is full bad, 100 is full good
    Int32 mDefaultInetCondition;
    Int32 mDefaultInetConditionPublished;
    Boolean mInetConditionChangeInFlight;
    Int32 mDefaultConnectionSequence;

    Object mDnsLock;
    Int32 mNumDnsEntries;
    Boolean mDnsOverridden;

    Boolean mTestMode;
    static AutoPtr<CConnectivityService> sServiceInstance;

    AutoPtr<INetworkManagementService> mNetd;
    AutoPtr<IINetworkPolicyManager> mPolicyManager;

    static const Int32 ENABLED;
    static const Int32 DISABLED;

    static const Boolean ADD;
    static const Boolean REMOVE;

    static const Boolean TO_DEFAULT_TABLE;
    static const Boolean TO_SECONDARY_TABLE;

    /** Handler used for internal events. */
    AutoPtr<IHandler> mHandler;
    Object mLockInteral;
    /** Handler used for incoming {@link NetworkStateTracker} events. */
    AutoPtr<NetworkStateTrackerHandler> mTrackerHandler;

    // list of DeathRecipients used to make sure features are turned off when
    // a process dies
    List< AutoPtr<FeatureUser> > mFeatureUsers;

    Boolean mSystemReady;
    AutoPtr<IIntent> mInitialBroadcast;

    AutoPtr<IPowerManagerWakeLock> mNetTransitionWakeLock;
    String mNetTransitionWakeLockCausedBy;
    Int32 mNetTransitionWakeLockSerialNumber;
    Int32 mNetTransitionWakeLockTimeout;

    AutoPtr<IInetAddress> mDefaultDns;

    // this collection is used to refcount the added routes - if there are none left
    // it's time to remove the route from the route table
    List< AutoPtr<IRouteInfo> > mAddedRoutes;

    // used in DBG mode to track inet condition reports
    static const Int32 INET_CONDITION_LOG_MAX_SIZE;
    AutoPtr< List<String> > mInetLog;

    // track the current default http proxy - tell the world if we get a new one (real change)
    AutoPtr<IProxyProperties> mDefaultProxy;
    Object mDefaultProxyLock;
    Boolean mDefaultProxyDisabled;

    // track the global proxy.
    AutoPtr<IProxyProperties> mGlobalProxy;
    Object mGlobalProxyLock;

    AutoPtr<SettingsObserver> mSettingsObserver;

    AutoPtr< ArrayOf<INetworkConfig*> > mNetConfigs;
    Int32 mNetworksDefined;

    AutoPtr< ArrayOf<RadioAttributes*> > mRadioAttributes;
    // the set of network types that can only be enabled by system/sig apps
    List<Int32> mProtectedNetworks;

    AutoPtr<INetworkManagementEventObserver> mDataActivityObserver;

    AutoPtr<INetworkPolicyListener> mPolicyListener;

    AutoPtr<IBroadcastReceiver> mUserPresentReceiver;
};

} //namespace Server
} //namespace Droid
} //namespace Elastos

#endif // __ELASTOS_DROID_SERVER_CCONNECTIVITYSERVICE_H__
