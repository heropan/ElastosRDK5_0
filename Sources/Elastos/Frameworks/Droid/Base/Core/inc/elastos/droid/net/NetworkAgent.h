
#ifndef __ELASTOS_DROID_NET_NETWORKAGENT_H__
#define __ELASTOS_DROID_NET_NETWORKAGENT_H__

#include "ext/frameworkext.h"
#include "Handler.h"

using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IMessage;
using Elastos::Utility::IArrayList;
using Elastos::Droid::Internal::Utility::IAsyncChannel;


namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A Utility class for handling for communicating between bearer-specific
 * code and ConnectivityService.
 *
 * A bearer may have more than one NetworkAgent if it can simultaneously
 * support separate networks (IMS / Internet / MMS Apns on cellular, or
 * perhaps connections with different SSID or P2P for Wi-Fi).
 *
 * @hide
 */
class NetworkAgent
    : public Handler
    , public INetworkAgent
{
public:
    CAR_INTERFACE_DECL()

    NetworkAgent();

    CARAPI constructor(
        /* [in] */ ILooper* looper,
        /* [in] */ IContext* context,
        /* [in] */ const String& logTag,
        /* [in] */ INetworkInfo* ni,
        /* [in] */ INetworkCapabilities* nc,
        /* [in] */ ILinkProperties* lp,
        /* [in] */ Int32 score);

    CARAPI constructor(
        /* [in] */ ILooper* looper,
        /* [in] */ IContext* context,
        /* [in] */ const String& logTag,
        /* [in] */ INetworkInfo* ni,
        /* [in] */ INetworkCapabilities* nc,
        /* [in] */ ILinkProperties* lp,
        /* [in] */ Int32 score,
        /* [in] */ INetworkMisc* misc);

    // @Override
    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    /**
     * Called by the bearer code when it has new LinkProperties data.
     */
    CARAPI SendLinkProperties(
        /* [in] */ ILinkProperties* linkProperties);

    /**
     * Called by the bearer code when it has new NetworkInfo data.
     */
    CARAPI SendNetworkInfo(
        /* [in] */ INetworkInfo* networkInfo);

    /**
     * Called by the bearer code when it has new NetworkCapabilities data.
     */
    CARAPI SendNetworkCapabilities(
        /* [in] */ INetworkCapabilities* networkCapabilities);

    /**
     * Called by the bearer code when it has a new score for this network.
     */
    CARAPI SendNetworkScore(
        /* [in] */ Int32 score);

    /**
     * Called by the VPN code when it wants to add ranges of UIDs to be routed
     * through the VPN network.
     */
    CARAPI AddUidRanges(
        /* [in] */ ArrayOf<IUidRange*>* ranges);

    /**
     * Called by the VPN code when it wants to remove ranges of UIDs from being routed
     * through the VPN network.
     */
    CARAPI RemoveUidRanges(
        /* [in] */ ArrayOf<IUidRange*>* ranges);

    /**
     * Called by the bearer to indicate this network was manually selected by the user.
     * This should be called before the NetworkInfo is marked CONNECTED so that this
     * Network can be given special treatment at that time.
     */
    CARAPI ExplicitlySelected();

    /**
     * Called when ConnectivityService has indicated they no longer want this network.
     * The parent factory should (previously) have received indication of the change
     * as well, either canceling NetworkRequests or altering their score such that this
     * network won't be immediately requested again.
     */
    virtual CARAPI Unwanted() = 0;

    /**
     * Called when the system determines the usefulness of this network.
     *
     * Networks claiming internet connectivity will have their internet
     * connectivity verified.
     *
     * Currently there are two possible values:
     * {@code VALID_NETWORK} if the system is happy with the connection,
     * {@code INVALID_NETWORK} if the system is not happy.
     * TODO - add indications of captive portal-ness and related success/failure,
     * ie, CAPTIVE_SUCCESS_NETWORK, CAPTIVE_NETWORK for successful login and detection
     *
     * This may be called multiple times as the network status changes and may
     * generate false negatives if we lose ip connectivity before the link is torn down.
     */
    CARAPI NetworkStatus(
        /* [in] */ Int32 status);

    CARAPI Log(
        /* [in] */ const String& s);

private:
    CARAPI QueueOrSendMessage(
        /* [in] */ Int32 what,
        /* [in] */ IObject* obj);

    // to initialize mPreConnectedQueue
    CARAPI_(AutoPtr<IArrayList>) CreatePreConnectQueue();

    const AutoPtr<IArrayList> mPreConnectedQueue;

    volatile AutoPtr<IAsyncChannel> mAsyncChannel;

    // LOG_TAG has duplicated macro
    String mLOG_TAG;

    static const Boolean DBG;

    static const Boolean VDBG;

    const AutoPtr<IContext> mContext;

    static const Int32 BASE;

    /**
     * Sent by ConnectivityService to the NetworkAgent to inform it of
     * suspected connectivity problems on its network.  The NetworkAgent
     * should take steps to verify and correct connectivity.
     */
    static const Int32 CMD_SUSPECT_BAD;

    /**
     * Sent by the NetworkAgent (note the EVENT vs CMD prefix) to
     * ConnectivityService to pass the current NetworkInfo (connection state).
     * Sent when the NetworkInfo changes, mainly due to change of state.
     * obj = NetworkInfo
     */
    static const Int32 EVENT_NETWORK_INFO_CHANGED;

    /**
     * Sent by the NetworkAgent to ConnectivityService to pass the current
     * NetworkCapabilties.
     * obj = NetworkCapabilities
     */
    static const Int32 EVENT_NETWORK_CAPABILITIES_CHANGED;

    /**
     * Sent by the NetworkAgent to ConnectivityService to pass the current
     * NetworkProperties.
     * obj = NetworkProperties
     */
    static const Int32 EVENT_NETWORK_PROPERTIES_CHANGED;

    /* centralize place where base network score, and network score scaling, will be
     * stored, so as we can consistently compare apple and oranges, or wifi, ethernet and LTE
     */
    static const Int32 WIFI_BASE_SCORE;

    /**
     * Sent by the NetworkAgent to ConnectivityService to pass the current
     * network score.
     * obj = network score Integer
     */
    static const Int32 EVENT_NETWORK_SCORE_CHANGED;

    /**
     * Sent by the NetworkAgent to ConnectivityService to add new UID ranges
     * to be forced into this Network.  For VPNs only.
     * obj = UidRange[] to forward
     */
    static const Int32 EVENT_UID_RANGES_ADDED;

    /**
     * Sent by the NetworkAgent to ConnectivityService to remove UID ranges
     * from being forced into this Network.  For VPNs only.
     * obj = UidRange[] to stop forwarding
     */
    static const Int32 EVENT_UID_RANGES_REMOVED;

    /**
     * Sent by ConnectivitySerice to the NetworkAgent to inform the agent of the
     * networks status - whether we could use the network or could not, due to
     * either a bad network configuration (no internet link) or captive portal.
     *
     * arg1 = either {@code VALID_NETWORK} or {@code INVALID_NETWORK}
     */
    static const Int32 CMD_REPORT_NETWORK_STATUS;

    static const Int32 VALID_NETWORK;

    static const Int32 INVALID_NETWORK;

    /**
     * Sent by the NetworkAgent to ConnectivityService to indicate this network was
     * explicitly selected.  This should be sent before the NetworkInfo is marked
     * CONNECTED so it can be given special treatment at that time.
     */
    static const Int32 EVENT_SET_EXPLICITLY_SELECTED;

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_NETWORKAGENT_H__
