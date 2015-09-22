
#ifndef __ELASTOS_DROID_NET_MOBILEDATASTATETRACKER_H__
#define __ELASTOS_DROID_NET_MOBILEDATASTATETRACKER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "BaseNetworkStateTracker.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Internal::Telephony::PhoneConstantsDataState;
using Elastos::Droid::Internal::Telephony::IITelephony;
using Elastos::Droid::Internal::Utility::IAsyncChannel;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Telephony::ISignalStrength;
using Elastos::Droid::Telephony::IPhoneStateListener;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Track the state of mobile data connectivity. This is done by
 * receiving broadcast intents from the Phone process whenever
 * the state of data connectivity changes.
 *
 * {@hide}
 */
class MobileDataStateTracker
    : public BaseNetworkStateTracker
    , public IMobileDataStateTracker
{
public:
    CAR_INTERFACE_DECL()
    class MdstHandler
        : public Handler
    {
    public:
        MdstHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ IMobileDataStateTracker* mdst);

        // @Override
        CARAPI HandleMessage(
            /* [in] */ AutoPtr<IMessage> msg);

    private:
        AutoPtr<IMobileDataStateTracker> mMdst;

    };

    class NetworkDataEntry {
    public:
        Int32 networkType;
        Int32 downloadBandwidth;               // in kbps
        Int32 uploadBandwidth;                 // in kbps
        Int32 latency;                         // in millisecond
        NetworkDataEntry(Int32 i1, Int32 i2, Int32 i3, Int32 i4) {
            networkType = i1;
            downloadBandwidth = i2;
            uploadBandwidth = i3;
            latency = i4;
        }
    };

    MobileDataStateTracker();

    /**
     * Create a new MobileDataStateTracker
     * @param netType the ConnectivityManager network type
     * @param tag the name of this network
     */
    CARAPI constructor(
        /* [in] */ Int32 netType,
        /* [in] */ const String& tag);

    /**
     * Begin monitoring data connectivity.
     *
     * @param context is the current Android context
     * @param target is the Hander to which to return the events.
     */
    CARAPI StartMonitoring(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* target);

    CARAPI IsPrivateDnsRouteSet(
        /* [out] */ Boolean* result);

    CARAPI PrivateDnsRouteSet(
        /* [in] */ Boolean enabled);

    CARAPI GetNetworkInfo(
        /* [out] */ INetworkInfo** result);

    CARAPI IsDefaultRouteSet(
        /* [out] */ Boolean* result);

    CARAPI DefaultRouteSet(
        /* [in] */ Boolean enabled);

    /**
     * This is not implemented.
     */
    CARAPI ReleaseWakeLock();

    /**
     * Report whether data connectivity is possible.
     */
    CARAPI IsAvailable(
        /* [out] */ Boolean* result);

    /**
     * Return the system properties name associated with the tcp buffer sizes
     * for this network.
     */
    CARAPI GetTcpBufferSizesPropName(
        /* [out] */ String* result);

    /**
     * Tear down mobile data connectivity, i.e., disable the ability to create
     * mobile data connections.
     * TODO - make async and return nothing?
     */
    CARAPI Teardown(
        /* [out] */ Boolean* result);

    /**
     * @return true if this is ready to operate
     */
    CARAPI IsReady(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI CaptivePortalCheckCompleted(
        /* [in] */ Boolean isCaptivePortal);

    CARAPI SetTeardownRequested(
        /* [in] */ Boolean isRequested);

    CARAPI IsTeardownRequested(
        /* [out] */ Boolean* result);

    /**
     * Re-enable mobile data connectivity after a {@link #teardown()}.
     * TODO - make async and always get a notification?
     */
    CARAPI Reconnect(
        /* [out] */ Boolean* result);

    /**
     * Turn on or off the mobile radio. No connectivity will be possible while the
     * radio is off. The operation is a no-op if the radio is already in the desired state.
     * @param turnOn {@code true} if the radio should be turned on, {@code false} if
     */
    CARAPI SetRadio(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI SetInternalDataEnable(
        /* [in] */ Boolean enabled);

    // @Override
    CARAPI SetUserDataEnable(
        /* [in] */ Boolean enabled);

    // @Override
    CARAPI SetPolicyDataEnable(
        /* [in] */ Boolean enabled);

    /**
     * Eanble/disable FailFast
     *
     * @param enabled is DctConstants.ENABLED/DISABLED
     */
    CARAPI SetEnableFailFastMobileData(
        /* [in] */ Int32 enabled);

    /**
     * carrier dependency is met/unmet
     * @param met
     */
    CARAPI SetDependencyMet(
        /* [in] */ Boolean met);

    /**
     *  Inform DCT mobile provisioning has started, it ends when provisioning completes.
     */
    CARAPI EnableMobileProvisioning(
        /* [in] */ const String& url);

    /**
     * Return if this network is the provisioning network. Valid only if connected.
     * @param met
     */
    CARAPI IsProvisioningNetwork(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI AddStackedLink(
        /* [in] */ ILinkProperties* link);

    // @Override
    CARAPI RemoveStackedLink(
        /* [in] */ ILinkProperties* link);

    // @Override
    CARAPI ToString(
        /* [out] */ String* result);

    static CARAPI NetworkTypeToApnType(
        /* [in] */ Int32 netType,
        /* [out] */ String* result);

    /**
     * @see android.net.NetworkStateTracker#getLinkProperties()
     */
    // @Override
    CARAPI GetLinkProperties(
        /* [out] */ ILinkProperties** result);

    CARAPI SupplyMessenger(
        /* [in] */ IMessenger* messenger);

    // @Override
    CARAPI GetLinkQualityInfo(
        /* [out] */ ILinkQualityInfo** result);

    // @Override
    CARAPI StartSampling(
        /* [in] */ ISamplingDataTrackerSamplingSnapshot* s);

    // @Override
    CARAPI StopSampling(
        /* [in] */ ISamplingDataTrackerSamplingSnapshot* s);

    // maintained in DataConnectionTracker.
    Boolean mUserDataEnabled;

    Boolean mPolicyDataEnabled;

private:
    class MobileDataStateReceiver
        : public BroadcastReceiver
    {
    public:
        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    };

    CARAPI UpdateLinkProperitesAndCapatilities(
        /* [in] */ IIntent* intent);

    CARAPI GetPhoneService(
        /* [in] */ Boolean forceRefresh);

    /**
     * Record the detailed state of a network, and if it is a
     * change from the previous state, send a notification to
     * any listeners.
     * @param state the new {@code DetailedState}
     * @param reason a {@code String} indicating a reason for the state change,
     * if one was supplied. May be {@code null}.
     * @param extraInfo optional {@code String} providing extra information about the state change
     */
    CARAPI SetDetailedState(
        /* [in] */ NetworkInfoDetailedState state,
        /* [in] */ const String& reason,
        /* [in] */ const String& extraInfo);

    /**
     * Internal method supporting the ENABLE_MMS feature.
     * @param apnType the type of APN to be enabled or disabled (e.g., mms)
     * @param enable {@code true} to enable the specified APN type,
     * {@code false} to disable it.
     * @return an integer value representing the outcome of the request.
     */
    CARAPI SetEnableApn(
        /* [in] */ const String& apnType,
        /* [in] */ Boolean enable,
        /* [out] */ Int32* result);

    CARAPI Log(
        /* [in] */ const String& s);

    CARAPI Loge(
        /* [in] */ const String& s);

    static CARAPI Sloge(
        /* [in] */ const String& s);

    static CARAPI GetNetworkDataEntry(
        /* [in] */ Int32 networkType,
        /* [out] */ NetworkDataEntry** result);

    static CARAPI GetNormalizedSignalStrength(
        /* [in] */ Int32 networkType,
        /* [in] */ ISignalStrength* ss,
        /* [out] */ Int32* result);

    // added for initialize mTheoreticalBWTable only
    static CARAPI_(AutoPtr<ArrayOf<NetworkDataEntry> >) CreateTheoreticalBWTable();

    static const String TAG;

    static const Boolean DBG;

    static const Boolean VDBG;

    PhoneConstantsDataState mMobileDataState;

    AutoPtr<IITelephony> mPhoneService;

    String mApnType;

    AutoPtr<INetworkInfo> mNetworkInfo;

    Boolean mTeardownRequested;

    AutoPtr<IHandler> mTarget;

    AutoPtr<IContext> mContext;

    AutoPtr<ILinkProperties> mLinkProperties;

    Boolean mPrivateDnsRouteSet;

    Boolean mDefaultRouteSet;

    AutoPtr<IHandler> mHandler;

    AutoPtr<IAsyncChannel> mDataConnectionTrackerAc;

    AutoPtr<IAtomicBoolean> mIsCaptivePortal;

    AutoPtr<ISignalStrength> mSignalStrength;

    AutoPtr<ISamplingDataTracker> mSamplingDataTracker;

    AutoPtr<IPhoneStateListener> mPhoneStateListener;

    static const Int32 UNKNOWN;

    static AutoPtr<ArrayOf<NetworkDataEntry> > mTheoreticalBWTable;

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_MOBILEDATASTATETRACKER_H__

#if 0 // old CMobileDataStateTracker.h
CarClass(CMobileDataStateTracker)
{
public:
    static class MdstHandler
        : public HandlerBase
    {
    };
};
#endif