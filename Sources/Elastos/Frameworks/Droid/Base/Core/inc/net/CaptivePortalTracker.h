
#ifndef __ELASTOS_DROID_NET_CCAPTIVEPORTALTRACKER_H__
#define __ELASTOS_DROID_NET_CCAPTIVEPORTALTRACKER_H__

#include "ext/frameworkext.h"
#include "util/StateMachine.h"
#include "util/State.h"

using Elastos::Net::IInetAddress;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Internal::Utility::StateMachine;
using Elastos::Droid::Internal::Utility::State;
using Elastos::Droid::Internal::Utility::IState;

namespace Elastos {
namespace Droid {
namespace Net {

class CaptivePortalTracker : public StateMachine
{
private:
    class DefaultState : public State
    {
    public:
        DefaultState(
            /* [in] */ CaptivePortalTracker* owner);

        CARAPI Enter();

        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);
    private:
        CaptivePortalTracker* mOwner;
    };

    class NoActiveNetworkState : public State
    {
    public:
        NoActiveNetworkState(
            /* [in] */ CaptivePortalTracker* owner);

        CARAPI Enter();

        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        CaptivePortalTracker* mOwner;
    };

    class ActiveNetworkState : public State
    {
    public:
        ActiveNetworkState(
            /* [in] */ CaptivePortalTracker* owner);

        CARAPI Enter();

        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        CaptivePortalTracker* mOwner;
    };

    class DelayedCaptiveCheckState : public State
    {
    public:
        DelayedCaptiveCheckState(
            /* [in] */ CaptivePortalTracker* owner);

        CARAPI Enter();

        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        CaptivePortalTracker* mOwner;
    };

public:
    static CARAPI_(AutoPtr<CaptivePortalTracker>) MakeCaptivePortalTracker(
        /* [in] */ IContext* context,
        /* [in] */ IIConnectivityManager* cs);

    CARAPI_(void) DetectCaptivePortal(
        /* [in] */ INetworkInfo* info);

private:
    CaptivePortalTracker(
        /* [in] */ IContext* context,
        /* [in] */ IIConnectivityManager* cs);

    CARAPI_(void) NotifyPortalCheckComplete(
        /* [in] */ INetworkInfo* info);

    CARAPI_(Boolean) IsActiveNetwork(
        /* [in] */ INetworkInfo* info);

    CARAPI_(Boolean) IsCaptivePortal(
        /* [in] */ IInetAddress* server);

    CARAPI_(AutoPtr<IInetAddress>) LookupHost(
        /* [in] */ const String& hostname);

    CARAPI_(void) SetNotificationVisible(
        /* [in] */ Boolean visible);

private:
    String mServer;
    String mUrl;
    Boolean mNotificationShown;
    Boolean mIsCaptivePortalCheckEnabled;
    AutoPtr<IIConnectivityManager> mConnService;
//    AutoPtr<ITelephonyManager> mTelephonyManager;
    AutoPtr<IContext> mContext;
    AutoPtr<INetworkInfo> mNetworkInfo;

    Int32 mDelayedCheckToken;

    AutoPtr<State> mDefaultState;//= new DefaultState();
    AutoPtr<State> mNoActiveNetworkState;//= new NoActiveNetworkState();
    AutoPtr<State> mActiveNetworkState;//= new ActiveNetworkState();
    AutoPtr<State> mDelayedCaptiveCheckState;//= new DelayedCaptiveCheckState();

    AutoPtr<IBroadcastReceiver> mReceiver;

    static const String TAG;
    static const Boolean DBG;
    static const String DEFAULT_SERVER;
    static const String NOTIFICATION_ID;
    static const Int32 SOCKET_TIMEOUT_MS;//= 10000;
    static const Int32 CMD_DETECT_PORTAL;//          = 0;
    static const Int32 CMD_CONNECTIVITY_CHANGE ;//   = 1;
    static const Int32 CMD_DELAYED_CAPTIVE_CHECK ;// = 2;

    /* This delay happens every time before we do a captive check on a network */
    static const Int32 DELAYED_CHECK_INTERVAL_MS;// = 10000;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CCAPTIVEPORTALTRACKER_H__

