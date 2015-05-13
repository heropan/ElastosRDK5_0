
#ifndef __ELASTOS_DROID_NET_DHCPSTATEMACHINE_H__
#define __ELASTOS_DROID_NET_DHCPSTATEMACHINE_H__

#include "content/BroadcastReceiver.h"
#include "net/DhcpInfoInternal.h"
#include "util/State.h"
#include "util/StateMachine.h"

using Elastos::Droid::App::IAlarmManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::Utility::IProtocol;
using Elastos::Droid::Internal::Utility::State;
using Elastos::Droid::Internal::Utility::StateMachine;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * StateMachine that interacts with the native DHCP client and can talk to
 * a controller that also needs to be a StateMachine
 *
 * The Dhcp state machine provides the following features:
 * - Wakeup and renewal using the native DHCP client  (which will not renew
 *   on its own when the device is in suspend state and this can lead to device
 *   holding IP address beyond expiry)
 * - A notification right before DHCP request or renewal is started. This
 *   can be used for any additional setup before DHCP. For example, wifi sets
 *   BT-Wifi coex settings right before DHCP is initiated
 *
 * @hide
 */
class DhcpStateMachine : public StateMachine
{
private:
    enum DhcpAction {
        DhcpAction_START,
        DhcpAction_RENEW
    };

public:
    class DefaultState : public State
    {
    public:
        DefaultState(
            /* [in] */ DhcpStateMachine* owner)
            : mOwner(owner)
        {}

        // @Override
        CARAPI Exit();

        // @Override
        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        DhcpStateMachine* mOwner;
    };

    class StoppedState : public State
    {
    public:
        StoppedState(
            /* [in] */ DhcpStateMachine* owner)
            : mOwner(owner)
        {}

        // @Override
        CARAPI Enter();

        // @Override
        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        DhcpStateMachine* mOwner;
    };

    class WaitBeforeStartState : public State
    {
    public:
        WaitBeforeStartState(
            /* [in] */ DhcpStateMachine* owner)
            : mOwner(owner)
        {}

        // @Override
        CARAPI Enter();

        // @Override
        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        DhcpStateMachine* mOwner;
    };

    class RunningState : public State
    {
    public:
        RunningState(
            /* [in] */ DhcpStateMachine* owner)
            : mOwner(owner)
        {}

        // @Override
        CARAPI Enter();

        // @Override
        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        DhcpStateMachine* mOwner;
    };

    class WaitBeforeRenewalState : public State
    {
    public:
        WaitBeforeRenewalState(
            /* [in] */ DhcpStateMachine* owner)
            : mOwner(owner)
        {}

        // @Override
        CARAPI Enter();

        // @Override
        CARAPI ProcessMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI Exit();

    private:
        DhcpStateMachine* mOwner;
    };

private:
    class MyBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        MyBroadcastReceiver(
            /* [in] */ DhcpStateMachine* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* ctx,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("DhcpStateMachine::MyBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        DhcpStateMachine* mOwner;
    };

public:
    static CARAPI_(AutoPtr<DhcpStateMachine>) MakeDhcpStateMachine(
        /* [in] */ IContext* context,
        /* [in] */ StateMachine* controller,
        /* [in] */ const String& intf);

    CARAPI_(void) RegisterForPreDhcpNotification();

    CARAPI_(void) DoQuit();

protected:
    CARAPI_(void) OnQuitting();

private:
    DhcpStateMachine(
        /* [in] */ IContext* context,
        /* [in] */ StateMachine* controller,
        /* [in] */ const String& intf);

    CARAPI_(Boolean) RunDhcp(
        /* [in] */ DhcpAction dhcpAction);

private:
    static const Int32 BASE = IProtocol::BASE_DHCP;

public:
    /* Commands from controller to start/stop DHCP */
    static const Int32 CMD_START_DHCP = BASE + 1;
    static const Int32 CMD_STOP_DHCP = BASE + 2;
    static const Int32 CMD_RENEW_DHCP = BASE + 3;

    /* Notification from DHCP state machine prior to DHCP discovery/renewal */
    static const Int32 CMD_PRE_DHCP_ACTION = BASE + 4;
    /* Notification from DHCP state machine post DHCP discovery/renewal. Indicates
     * success/failure */
    static const Int32 CMD_POST_DHCP_ACTION = BASE + 5;
    /* Notification from DHCP state machine before quitting */
    static const Int32 CMD_ON_QUIT = BASE + 6;

    /* Command from controller to indicate DHCP discovery/renewal can continue
     * after pre DHCP action is complete */
    static const Int32 CMD_PRE_DHCP_ACTION_COMPLETE = BASE + 7;

    /* Message.arg1 arguments to CMD_POST_DHCP notification */
    static const Int32 DHCP_SUCCESS = 1;
    static const Int32 DHCP_FAILURE = 2;

private:
    static const String TAG;
    static const Boolean DBG;

    /* A StateMachine that controls the DhcpStateMachine */
    AutoPtr<StateMachine> mController;

    AutoPtr<IContext> mContext;
    AutoPtr<IBroadcastReceiver> mBroadcastReceiver;
    AutoPtr<IAlarmManager> mAlarmManager;
    AutoPtr<IPendingIntent> mDhcpRenewalIntent;
    AutoPtr<IPowerManagerWakeLock> mDhcpRenewWakeLock;
    static const String WAKELOCK_TAG;

    //Remember DHCP configuration from first request
    AutoPtr<DhcpInfoInternal> mDhcpInfo;

    static const Int32 DHCP_RENEW = 0;
    static const String ACTION_DHCP_RENEW;

    //Used for sanity check on setting up renewal
    static const Int32 MIN_RENEWAL_TIME_SECS = 5 * 60;  // 5 minutes

    String mInterfaceName;
    Boolean mRegisteredForPreDhcpNotification;

    AutoPtr<State> mDefaultState;
    AutoPtr<State> mStoppedState;
    AutoPtr<State> mWaitBeforeStartState;
    AutoPtr<State> mRunningState;
    AutoPtr<State> mWaitBeforeRenewalState;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_DHCPSTATEMACHINE_H__

