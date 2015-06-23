
#ifndef __ELASTOS_DROID_NET_CMOBILEDATASTATETRACKER_H__
#define __ELASTOS_DROID_NET_CMOBILEDATASTATETRACKER_H__

#include "_CMobileDataStateTracker"
#include "os/HandlerBase.h"
#include "util/AsyncChannel.h"

using Elastos::Droid::Internal::Utility::AsyncChannel;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CMobileDataStateTracker)
{
public:
    static class MdstHandler
        : public HandlerBase
    {
    public:
        MdstHandler(
            /* [in] */ Looper looper,
            /* [in] */ MobileDataStateTracker mdst);

        void HandleMessage(
            /* [in] */ Message msg);

    private:
        MobileDataStateTracker mMdst;
    };

private:
    class MobileDataStateReceiver
        : public BroadcastReceiver
    {
    friend class CMobileDataStateTracker;
    public:
        MobileDataStateReceiver(CMobileDataStateTracker* host);
        void OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    private:
        CMobileDataStateTracker* mHost;
    };

public:
    CARAPI constructor(
        /* [in] */ Int32 netType,
        /* [in] */ const String& tag);

    CARAPI StartMonitoring(
        /* [in] */ IContext* context,
        /* [in] */ Handler* target);

    CARAPI IsPrivateDnsRouteSet(
        /* [out] */ Boolean* result);

    CARAPI PrivateDnsRouteSet(
        /* [in] */ Boolean enabled);

    CARAPI GetNetworkInfo(
        /* [out] */ INetworkInfo* result);

    CARAPI IsDefaultRouteSet(
        /* [out] */ Boolean* result);

    CARAPI DefaultRouteSet(
        /* [in] */ Boolean enabled);

    CARAPI ReleaseWakeLock();

    CARAPI IsAvailable(
        /* [out] */ Boolean* result);

    CARAPI GetTcpBufferSizesPropName(
        /* [out] */ String* result);

    CARAPI Teardown(
        /* [out] */ Boolean* result);

    CARAPI CaptivePortalCheckComplete();

    CARAPI SetTeardownRequested(
        /* [in] */ Boolean isRequested);

    CARAPI IsTeardownRequested(
        /* [out] */ Boolean* result);

    CARAPI Reconnect(
        /* [out] */ Boolean* result);

    CARAPI SetRadio(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI SetUserDataEnable(
        /* [in] */ Boolean enabled);

    CARAPI SetPolicyDataEnable(
        /* [in] */ Boolean enabled);

    CARAPI SetDependencyMet(
        /* [in] */ Boolean met);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetLinkProperties(
        /* [out] */ ILinkProperties* result);

    CARAPI GetLinkCapabilities(
        /* [out] */ ILinkCapabilities* result);

    static CARAPI_(String) NetworkTypeToApnType(
        /* [in] */ Int32 netType);

private:
    CARAPI_(void) GetPhoneService(
        /* [in] */ Boolean forceRefresh);

    CARAPI_(void) SetDetailedState(
        /* [in] */ INetworkInfo.DetailedState state,
        /* [in] */ const String& reason,
        /* [in] */ const String& extraInfo);

    CARAPI_(Int32) SetEnableApn(
        /* [in] */ const String& apnType,
        /* [in] */ Boolean enable);

    CARAPI_(void) Log(
        /* [in] */ const String& s);

    CARAPI_(void) Loge(
        /* [in] */ const String& s);

    static CARAPI_(void) Sloge(
        /* [in] */ const String& s);

protected:
    // NOTE: these are only kept for debugging output; actual values are
    // maintained in DataConnectionTracker.
    Boolean mUserDataEnabled = TRUE;
    Boolean mPolicyDataEnabled = TRUE;

private:
    static const String TAG;//= "MobileDataStateTracker";
    static const Boolean DBG = FALSE;
    static const Boolean VDBG = FALSE;

    AutoPtr<PhoneConstants::DataState> mMobileDataState;
    AutoPtr<IITelephony> mPhoneService;

    String mApnType;
    AutoPtr<INetworkInfo> mNetworkInfo;
    Boolean mTeardownRequested = FALSE;
    AutoPtr<Handler> mTarget;
    AutoPtr<IContext> mContext;
    AutoPtr<ILinkProperties> mLinkProperties;
    AutoPtr<ILinkCapabilities> mLinkCapabilities;
    Boolean mPrivateDnsRouteSet = FALSE;
    Boolean mDefaultRouteSet = FALSE;

    AutoPtr<Handler> mHandler;
    AutoPtr<AsyncChannel> mDataConnectionTrackerAc;
    AutoPtr<IMessenger> mMessenger;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CMOBILEDATASTATETRACKER_H__

